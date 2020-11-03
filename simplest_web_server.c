// Copyright (c) 2015 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"
#include <stdlib.h>

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

struct mg_str cb(struct mg_connection *c, struct mg_str file_name) {
  // Return the same filename. Do not actually do this except in test!
  // fname is user-controlled and needs to be sanitized.
  return file_name;
}

static void ev_handler(struct mg_connection *nc, int ev, void *p) {  
  switch (ev) {
    case MG_EV_HTTP_REQUEST:
      {        
        struct http_message * hm = (struct http_message *) p;        

        int ret;
        char target[1024] = {0};
        char abs_path[1024] = {0};   
        char syscmd[2048] = {0};   
        char relative_path[hm->uri.len];
        int cmd_len, i, path_len;
        
        // HTML decode request url
        mg_url_decode(hm->uri.p, hm->uri.len, relative_path, hm->uri.len, 1);  
       
        // Break at first new line char to leave just folder path 
        for(i=0; i < (int)strlen(relative_path); ++i){
          if(relative_path[i] == '\n'){            
            relative_path[i] = 0;            
            break;
          }
        }

        // Calculate the string length and add trailing slash if missing to help resolve path
        path_len = strlen(relative_path);
        if((path_len == 0) || ((path_len > 0) && (relative_path[path_len-1] != '/'))){
          relative_path[path_len] = '/';
          relative_path[path_len+1] = 0;    
          path_len++;          
        }

        // Get absolute target folder path
        memcpy(abs_path, s_http_server_opts.document_root, strlen(s_http_server_opts.document_root)); 
        memcpy(abs_path + strlen(abs_path), relative_path, path_len);
        // printf("abs_path: %s\n", abs_path);         

        // Check for delete command
        ret = mg_get_http_var(&(hm->message), HTTP_CMD_DELETE, target, sizeof(target));
        if(ret > 0){ 
          // Form command like: rm -rf "<target path>"
          memcpy(syscmd, SYS_CMD_DEL, sizeof(SYS_CMD_DEL));
          memcpy(syscmd + sizeof(SYS_CMD_DEL) - 1, abs_path, strlen(abs_path));  
          memcpy(syscmd + strlen(syscmd), target, strlen(target)); 
          syscmd[strlen(syscmd)] = '"';

          // printf("CMD: %s\n", syscmd);          
          system(syscmd);

          mg_http_send_error(nc, 301, NULL);
          return;
        }

        // Check for rename command
        ret = mg_get_http_var(&(hm->message), HTTP_CMD_RENAME, target, sizeof(target));
        if(ret > 0){ 
          char new_name[256] = {0};
          ret = mg_get_http_var(&(hm->message), HTTP_PARAM_NAME, new_name, sizeof(new_name));
          if(ret > 0){ 
            // printf("new_name: %s\n", new_name); 
            // printf("target: %s\n", target); 
            // Form command like: cd "<target folder path>" && move "<target name>" "<new name>"
            syscmd[0] = 'c';
            syscmd[1] = 'd';
            syscmd[2] = ' ';

            // Add path to chdir to. 
            syscmd[3] = '"';
            memcpy(syscmd + 4, abs_path, strlen(abs_path));

            cmd_len = strlen(syscmd);
            syscmd[cmd_len] = '"';
            syscmd[cmd_len + 1] = ' ';
            syscmd[cmd_len + 2] = '&';
            syscmd[cmd_len + 3] = '&';
            syscmd[cmd_len + 4] = ' ';
            
            memcpy(syscmd + cmd_len + 5, SYS_CMD_RENAME, sizeof(SYS_CMD_RENAME));
            // Add target
            memcpy(syscmd + strlen(syscmd), target, strlen(target));

            cmd_len = strlen(syscmd);
            syscmd[cmd_len] = '"';
            syscmd[cmd_len + 1] = ' ';
            syscmd[cmd_len + 2] = '"';

            // Append new name
            memcpy(syscmd + cmd_len + 3, new_name, strlen(new_name));
            syscmd[strlen(syscmd)] = '"';

            // printf("CMD: %s\n", syscmd);          
            system(syscmd);
         
            mg_http_send_error(nc, 301, NULL);            
          }else{
            mg_http_send_error(nc, 404, "Name not found.");            
          }
          return;
        }

        // Check for archive command
        ret = mg_get_http_var(&(hm->message), HTTP_CMD_ARCHIVE, target, sizeof(target));
        if(ret > 0){
          // Form command like: cd "<absolute target folder path>" tar -czf "<archive name.tgz>" "<target name>"  
          syscmd[0] = 'c';
          syscmd[1] = 'd';
          syscmd[2] = ' ';

          // Add path to chdir to. 
          syscmd[3] = '"';
          memcpy(syscmd + 4, abs_path, strlen(abs_path));

          cmd_len = strlen(syscmd);
          syscmd[cmd_len] = '"';
          syscmd[cmd_len + 1] = ' ';
          syscmd[cmd_len + 2] = '&';
          syscmd[cmd_len + 3] = '&';
          syscmd[cmd_len + 4] = ' ';
          
          memcpy(syscmd + cmd_len + 5, SYS_CMD_ARCHIVE, sizeof(SYS_CMD_ARCHIVE));
          // Add archive name
          memcpy(syscmd + strlen(syscmd), target, ret);

          cmd_len = strlen(syscmd);
          if(syscmd[cmd_len - 1] == '/'){            
            cmd_len--;
          }

          memcpy(syscmd + cmd_len, SYS_ARCHIVE_EXT, sizeof(SYS_ARCHIVE_EXT));           

          // Append target
          memcpy(syscmd + strlen(syscmd), target, ret);
          syscmd[strlen(syscmd)] = '"';

          // printf("CMD: %s\n", syscmd); 
          system(syscmd);

          mg_http_send_error(nc, 301, NULL);
          return;
        }

        // Check new dir command
        ret = mg_get_http_var(&(hm->message), HTTP_CMD_NEW_DIR, target, sizeof(target));
        if(ret > 0){
           // Form command
          syscmd[0] = 'c';
          syscmd[1] = 'd';
          syscmd[2] = ' ';

          // Add path to chdir to. Start with root dir
          syscmd[3] = '"';
          memcpy(syscmd + 4, s_http_server_opts.document_root, strlen(s_http_server_opts.document_root));           
        
          // Append relative target path          
          memcpy(syscmd + strlen(syscmd), relative_path, path_len);

          cmd_len = strlen(syscmd);
          syscmd[cmd_len] = '"';
          syscmd[cmd_len + 1] = ' ';
          syscmd[cmd_len + 2] = '&';
          syscmd[cmd_len + 3] = '&';
          syscmd[cmd_len + 4] = ' ';
          
          memcpy(syscmd + cmd_len + 5, SYS_CMD_NEW_DIR, sizeof(SYS_CMD_NEW_DIR));
          // Add dir name
          memcpy(syscmd + strlen(syscmd), target, ret);
          syscmd[strlen(syscmd)] = '"';

          // printf("CMD: %s\n", syscmd); 
          system(syscmd);

          mg_http_send_error(nc, 301, NULL);
          return;
        }

        mg_serve_http(nc, hm, s_http_server_opts);     
        
      }
      break;
      
    case MG_EV_HTTP_PART_BEGIN:
    case MG_EV_HTTP_PART_DATA:
    case MG_EV_HTTP_PART_END:
      mg_file_upload_handler(nc, ev, p, cb);
      break;      
  }
}

int main(int argc, char **argv) {
  struct mg_mgr mgr;
  struct mg_connection *nc;

  if(argc<2){
    printf("Usage: %s <root dir> [<port>]\n", argv[0]);
    return 1;
  }

  mg_mgr_init(&mgr, NULL);  

  if(argc > 2){
    /* Port also provided */
    printf("Starting web server on port %s\n", argv[2]);
    nc = mg_bind(&mgr, argv[2], ev_handler);
  }else{
    printf("Starting web server on port %s\n", s_http_port);
    nc = mg_bind(&mgr, s_http_port, ev_handler);    
  }  
  
  if (nc == NULL) {
    printf("Failed to create listener\n");
    return 1;
  }

  // Set up HTTP server parameters
  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = argv[1];    
  s_http_server_opts.enable_directory_listing = "yes";

  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
