// Copyright (c) 2015 Cesanta Software Limited
// All rights reserved

#include <stdlib.h>
#include "packed_html.h"
#include "mongoose.h"


static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;
static const char HTTP_CMD_DELETE[] = "delete";
static const char HTTP_CMD_ARCHIVE[] = "archive";
static const char HTTP_CMD_NEW_DIR[] = "newdir";
static const char HTTP_CMD_RENAME[] = "rename";
static const char HTTP_PARAM_NAME[] = "name";
static const char SYS_CMD_DEL[] = "rm -rf \"";
static const char SYS_CMD_ARCHIVE[] = "tar -czf \"";
static const char SYS_ARCHIVE_EXT[] = ".tar.gz\" \"";
static const char SYS_CMD_NEW_DIR[] = "mkdir \"";
static const char SYS_CMD_RENAME[] = "mv \"";
static char abs_path[MG_MAX_PATH] = {0};

#define UPLOAD_AUTH_TIMEOUT   1    // Revoke authorization after this time in s since last file upload.


#ifdef LOGIN_SUPPORT
/* Authenticated user.
 * A user can be authenticated by:
 *   - name:pass pair
 *   - session id
 * When a user is shown a login screen, he/she enters a user and pass. If successful,
 * a server creates a session and returns session id embedded in a cookie. 
 * From that point on, client can use the session id for authentication. 
 */
struct user {
  const char *name; 
  const char *pass; 
};

/* List of users.
 * In production, make passwords strong.
 * User list is kept in RAM. If using a large number of users, 
 * they could be storred in file or database.
 */ 
struct user users[] = {
    {"user1", "pass1"},
    {"user2", "pass2"},
    {NULL, NULL},	// Do not remove. Used to detect end of user list
};

/* This is the name of the cookie carrying the session ID. */
#define SESSION_COOKIE_NAME "fssession"
/* In our example sessions are destroyed after 30min of inactivity. */
#define SESSION_TTL 1800000ul
/* Sessions are checked periodically every 5s */
#define SESSION_CHECK_INTERVAL 5000
/** This example uses a simple in-memory storage for just 10 sessions.
 * An implementation for a large number of users would use persistent storage like a database. */
#define NUM_SESSIONS 10

struct session {
  /* Session ID. Must be unique and hard to guess. */
  uint64_t id;
  /* Time when the session was created and time of last activity. Used to clean up stale sessions. */
  double created;
  double last_used; 

  /* User name this session is associated with. */
  char *user; 
};
struct session s_sessions[NUM_SESSIONS];

/* Variables to support file upload from form or curl. */
char user[50] = {0}, pass[50] = {0}, session_id[21] = {0};
double last_uploaded = 0;   // Timestamp when uploading files to support upload with user and pass

static int check_pass(const char *user, const char *pass) {
  int success = 0;
  struct user *u;
  for (u = users; u->name != NULL; u++){
    if (strcmp(user, u->name) == 0){
      int success = strcmp(pass, u->pass) == 0;
      return success;
    } 
  }

  return success;
}

/* Parses the session cookie and returns a pointer to the session struct or NULL if not found. */
static struct session *get_session(struct http_message *hm) {
  struct mg_str *cookie_header = mg_get_http_header(hm, "cookie");
  if (cookie_header == NULL) return NULL;
  
  char ssid[21];
  int ssid_len = mg_http_parse_header(cookie_header, SESSION_COOKIE_NAME, ssid, sizeof(ssid));

  if (ssid_len == 0) {
    return NULL;
  }

  uint64_t sid = strtoull(ssid, NULL, 16);
  
  for (int i = 0; i < NUM_SESSIONS; i++) {
    if (s_sessions[i].id == sid) {
      s_sessions[i].last_used = mg_time();
      return &s_sessions[i];
    }
  }
  return NULL;
}

static struct session *get_session_by_id(char* ssid) {

  int ssid_len = strlen(ssid);
  if (ssid_len == 0) {
    return NULL;
  }

  uint64_t sid = strtoull(ssid, NULL, 16);
  
  for (int i = 0; i < NUM_SESSIONS; i++) {
    if (s_sessions[i].id == sid) {
      s_sessions[i].last_used = mg_time();
      return &s_sessions[i];
    }
  }
  return NULL;
}

static void check_upload_credentials(){

  if(check_pass(user, pass)) {
    last_uploaded = mg_time();
  }else{
    struct session *s = get_session_by_id(session_id);

    uint64_t sid = strtoull(session_id, NULL, 16);
    if ((s != NULL) && (s->id == sid)) {
      last_uploaded = mg_time();
    }
  }
}

/* Destroys the session state. */
static void destroy_session(struct session *s) {
  free(s->user);
  memset(s, 0, sizeof(*s));
}

/* Creates a new session for the user. */
static struct session *create_session(const char *user, const struct http_message *hm) {
  /* Find first available slot or use the oldest one. */
  struct session *s = NULL;
  struct session *oldest_s = s_sessions;
  for (int i = 0; i < NUM_SESSIONS; i++) {
    if (s_sessions[i].id == 0) {
      s = &s_sessions[i];
      break;
    }
    if (s_sessions[i].last_used < oldest_s->last_used) {
      oldest_s = &s_sessions[i];
    }
  }
  if (s == NULL) {
    destroy_session(oldest_s);
    //printf("Evicted %lx/%s\n", oldest_s->id, oldest_s->user);
    s = oldest_s;
  }
  /* Initialize new session. */
  s->created = s->last_used = mg_time();
  s->user = strdup(user);
  /* Create an ID by putting various volatiles into a pot and stirring. */
  cs_sha1_ctx ctx;
  cs_sha1_init(&ctx);
  cs_sha1_update(&ctx, (const unsigned char *) hm->message.p , hm->message.len);
  cs_sha1_update(&ctx, (const unsigned char *) s, sizeof(*s));
  unsigned char digest[20];
  cs_sha1_final(digest, &ctx);
  s->id = *((uint64_t *) digest);
  return s;
}


/* Logs the user out. Removes cookie and any associated session state. */
static void logout_handler(struct mg_connection *c, struct http_message *hm) {
  mg_printf(c,
            "HTTP/1.0 302 Found\r\n"
            "Set-Cookie: %s=\r\n"
            "Location: /\r\n"
            "\r\n"
            "Logged out",
            SESSION_COOKIE_NAME);
  struct session *s = get_session(hm);
  if (s != NULL) {
    printf("%s logged out, session %lx destroyed\n", s->user, s->id);
    destroy_session(s);
  }
}

/* Cleans up sessions that have been idle for too long. */
void check_sessions() {
  double threshold = mg_time() - SESSION_TTL;
  for (int i = 0; i < NUM_SESSIONS; i++) {
    struct session *s = &s_sessions[i];
    if (s->id != 0 && s->last_used < threshold) {
      printf("Session %ld (%s) closed due to idleness.\n", s->id, s->user);
      destroy_session(s);
    }
  }
}

#endif //LOGIN_SUPPORT

// Handle interrupts, like Ctrl-C
static int s_signo;
static void signal_handler(int signo) {
  s_signo = signo;
}


struct mg_str request_sanitizer(struct mg_connection *c, struct mg_str file_name) {
  // Return the same filename. Do not actually do this except in test!
  // fname is user-controlled and needs to be sanitized.
  // TODO: sanitize file name
  (void)c;
  // char path[MG_MAX_PATH] = {0};
  // strcpy(path, s_http_server_opts.document_root);
  // strcat(path, "/");
  // strncat(path, file_name.p, file_name.len);

  // file_name.p = path;
  // file_name.len = strlen(path);
  return file_name;
}

static void request_handler(struct mg_connection *nc, int ev, void *p) {  
  struct http_message * hm = (struct http_message *) p;  
    
  switch (ev) {
    case MG_EV_HTTP_REQUEST:
      { 
        char relative_path[MG_MAX_PATH] = {0};
        // HTML decode request url
        mg_url_decode(hm->uri.p, hm->uri.len, relative_path, hm->uri.len + 1, 1);         
        
        if (strcmp(relative_path, "/about.html") == 0){
          mg_http_reply(nc, 200, "", about_html); 
        
#ifdef LOGIN_SUPPORT

	      }else if (strcmp(relative_path, "/logout.html") == 0){
          logout_handler(nc, hm);

        }else if (strcmp(relative_path, "/login.html") == 0){
          char page_html[MG_MAX_PATH];

          if (mg_vcmp(&hm->method, "GET") == 0){
            /* Get request. Serve the login page. */            
            sprintf(page_html, login_html, "");

            mg_http_reply(nc, 200, "", page_html); 
          }else{
            /* POST request. Perform password check.*/            
            int ul = mg_get_http_var(&hm->body, "user", user, sizeof(user));
            int pl = mg_get_http_var(&hm->body, "pass", pass, sizeof(pass));
            if (ul > 0 && pl > 0) {
              if (check_pass(user, pass)) {
                /* Username and password are OK. Create session. */
                struct session *s = create_session(user, hm);

                /* Redirect to root */
                mg_printf(nc, "HTTP/1.0 302 Found\r\n");

                mg_printf(nc, "Set-Cookie: %s=%lx; path=/\r\n", SESSION_COOKIE_NAME, s->id);   

                mg_printf(nc, "Location: /\r\n\r\n");
                printf("%s logged in, sid %lX\n", s->user, s->id);
              } else {
                sprintf(page_html, login_html, "ERROR: Wrong username or password.");
                mg_http_reply(nc, 200, "", page_html);
              }
            } else {
              sprintf(page_html, login_html, "ERROR: Both password and username are required.");
              mg_http_reply(nc, 200, "", page_html);
            }
          }        
        }else{
          struct session *s = get_session(hm);
          /* Ask the user to log in if they did not present a valid cookie. */
          if (s == NULL) {
            mg_printf(nc, "HTTP/1.0 302 Found\r\nLocation: /login.html\r\n\r\n");      
          
#endif          //LOGIN_SUPPORT
          }else{
            int ret;
            char target[MG_MAX_PATH] = {0};
            char abs_path[MG_MAX_PATH] = {0};   
            char syscmd[MG_MAX_PATH] = {0};   

            // Get absolute target folder path
            strcat(abs_path, s_http_server_opts.document_root);        
            strcat(abs_path, relative_path); 

            // Check for delete command
            ret = mg_get_http_var(&(hm->message), HTTP_CMD_DELETE, target, sizeof(target));
            if(ret > 0){ 
              // Form command like: rm -rf "<target path>"
              strcat(abs_path, target);
              memcpy(syscmd, SYS_CMD_DEL, sizeof(SYS_CMD_DEL));
              strcat(syscmd, abs_path);  
              strcat(syscmd, "\"");

              ret = system(syscmd);
              if(ret == 0){
                mg_http_reply(nc, 200, "", "Done");
              }else{
                mg_http_reply(nc, 301, "", "Error deleting file!");
              }       
              return;
            }

            // Check for rename command
            ret = mg_get_http_var(&(hm->message), HTTP_CMD_RENAME, target, sizeof(target));
            if(ret > 0){
              char new_name[256] = {0};
              ret = mg_get_http_var(&(hm->message), HTTP_PARAM_NAME, new_name, sizeof(new_name));
              if(ret > 0){ 
                strcpy(syscmd, SYS_CMD_RENAME);
                strcat(syscmd, abs_path);  
                strcat(syscmd, target);
                strcat(syscmd, "\" \"");
                strcat(syscmd, abs_path);    
                strcat(syscmd, new_name);
                strcat(syscmd, "\"");
                
                ret = system(syscmd);
                if(ret == 0){
                  mg_http_reply(nc, 200, "", "Done");
                }else{
                  mg_http_reply(nc, 301, "", "Error renaming file!");
                }          
                
              }else{
                mg_http_reply(nc, 301, "", "Error renaming file!");
              }
              return;
            }

            // Check for archive command
            ret = mg_get_http_var(&(hm->message), HTTP_CMD_ARCHIVE, target, sizeof(target));
            if(ret > 0){
              // Form command like: cd "<absolute target folder path>" tar -czf "<archive name.tgz>" "<target name>" 
              strcat(syscmd, "cd \"");
              strcat(syscmd, abs_path);
              strcat(syscmd, "\" && ");
              strcat(syscmd, SYS_CMD_ARCHIVE);
              strcat(syscmd, target);
              
              int cmd_len = strlen(syscmd);
              if(syscmd[cmd_len - 1] == '/'){            
                syscmd[cmd_len - 1] = 0;
              }

              strcat(syscmd, SYS_ARCHIVE_EXT);
              strcat(syscmd, target);
              strcat(syscmd, "\"");

              ret = system(syscmd);
              if(ret == 0){
                mg_http_reply(nc, 200, "", "Done");
              }else{
                mg_http_reply(nc, 301, "", "Error archiving!");
              } 
              return;
            }

            // Check new dir command
            ret = mg_get_http_var(&(hm->message), HTTP_CMD_NEW_DIR, target, sizeof(target));
            if(ret > 0){
              strcat(syscmd, SYS_CMD_NEW_DIR);
              strcat(syscmd, abs_path);
              strcat(syscmd, target);
              strcat(syscmd, "\"");

              //printf("SYS_CMD: %s\n", syscmd);
              ret = system(syscmd);
              if(ret == 0){
                mg_http_reply(nc, 200, "", "Done");
              }else{
                mg_http_reply(nc, 301, "", "Error creating new directory!");
              } 
              return;
            }

            mg_serve_http(nc, hm, s_http_server_opts);    
          } 
#ifdef LOGIN_SUPPORT         
        }
#endif
      }
      break;
  /* File download section */
    case MG_EV_HTTP_MULTIPART_REQUEST:
        {
          strcpy(abs_path, s_http_server_opts.document_root);
          strncat(abs_path, hm->uri.p, (int)hm->uri.len); 
#ifdef LOGIN_SUPPORT 
          last_uploaded = 0;
          user[0] = 0;
          pass[0] = 0;  
#endif        
        }       
        break;
    case MG_EV_HTTP_PART_BEGIN: 
    case MG_EV_HTTP_PART_DATA: 
    case MG_EV_HTTP_PART_END:
      {
#ifdef LOGIN_SUPPORT 
        struct mg_http_multipart_part *mp = (struct mg_http_multipart_part *) p;

        if(strcmp(mp->var_name, "file") == 0){           
          if((last_uploaded > 0) && ((mg_time() - last_uploaded) < UPLOAD_AUTH_TIMEOUT)){
            mg_file_upload_handler(nc, ev, p, abs_path, request_sanitizer);
            // Refresh authentication
            check_upload_credentials();
          }else{
            nc->flags |= MG_F_SEND_AND_CLOSE;
            char* errMsg;
            if((user[0] == 0) || (pass[0] == 0)){
              errMsg = "ERROR: Please provide a valid user name and password!\r\n";
            }else{
              errMsg = "ERROR: Invalid user name or password!\r\n";
            }
            mg_http_reply(nc, 401, "Connection: close\r\n\r\n", errMsg);                 
          }
        }else{

          if((int)mp->data.len > 0){
            // printf("MG_EV_HTTP_PART_END VarName: %s=%.*s\n", mp->var_name, (int)mp->data.len, mp->data.p);
            if(strcmp(mp->var_name, "user") == 0){
              strncpy(user, mp->data.p, (int)mp->data.len);
              user[(int)mp->data.len] = 0;
            }else if(strcmp(mp->var_name, "pass") == 0){
              strncpy(pass, mp->data.p, (int)mp->data.len);
              pass[(int)mp->data.len] = 0;
            }else if(strcmp(mp->var_name, "session") == 0){
              strncpy(session_id, mp->data.p, (int)mp->data.len);
              session_id[(int)mp->data.len] = 0;
            }else if(strcmp(mp->var_name, "dst") == 0){
              int abs_path_len = strlen(abs_path);
              if(abs_path[abs_path_len - 1] != '/'){
                abs_path[abs_path_len] = '/';
                abs_path[abs_path_len + 1] = 0;
              }

              strncat(abs_path, mp->data.p, (int)mp->data.len);
              abs_path[abs_path_len + 1 + (int)mp->data.len] = 0;
            }
            check_upload_credentials();
          }
        }
#else
        mg_file_upload_handler(nc, ev, p, abs_path, request_sanitizer);
#endif
      }
      break;

    case MG_EV_HTTP_MULTIPART_REQUEST_END:
      {
      }       
      break;   
  }  
}

static void usage(const char *prog) {
  fprintf(stderr,
          APP_NAME
          "\n"
          "Usage: %s OPTIONS\n"
          "  -d DIR    - directory to serve, default: current dir\n"
          "  -p PORT   - listening port, default: %s\n",
          prog, s_http_port);
  exit(EXIT_FAILURE);
}


int main(int argc, char **argv) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  int i;

  s_http_server_opts.document_root = ".";

  // Parse command-line flags
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-d") == 0) {
      s_http_server_opts.document_root = argv[++i];
    } else if (strcmp(argv[i], "-p") == 0) {
      s_http_port = argv[++i];
    } else {
      usage(argv[0]);
    }
  }

  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  mg_mgr_init(&mgr, NULL);  

  printf("Serving: '%s' on 'http://localhost:%s'\n", s_http_server_opts.document_root, s_http_port);
  nc = mg_bind(&mgr, s_http_port, request_handler);    
    
  if (nc == NULL) {
    fprintf(stderr, "Failed to create listener\n");
    return 1;
  }

  // Set up HTTP server parameters
  mg_set_protocol_http_websocket(nc);      
  s_http_server_opts.enable_directory_listing = "yes";

  printf("Mongoose version: v%s\n", MG_VERSION);

  while (s_signo == 0){
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
