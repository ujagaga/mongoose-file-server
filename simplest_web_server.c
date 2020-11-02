// Copyright (c) 2015 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"
#include <stdlib.h>

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;
static const char HTTP_CMD_DELETE[] = "delete";
static const char HTTP_CMD_ARCHIVE[] = "archive";
static const char HTTP_CMD_NEW_DIR[] = "newdir";
static const char SYS_CMD_DEL[] = "rm -rf ";
static const char SYS_CMD_ARCHIVE[] = "tar -czf ";
static const char SYS_ARCHIVE_EXT[] = ".tar.gz\" \"";
static const char SYS_CMD_NEW_DIR[] = "mkdir \"";

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
        char value[1024]={0};
        char cmd[2048]={0};   
        char url_decoded[hm->uri.len];
        int cmd_len;
        int i;
        
        mg_url_decode(hm->uri.p, hm->uri.len, url_decoded, hm->uri.len, 1);  
       
        // Break at first new line char to leave just folder path 
        for(i=0; i < (int)strlen(url_decoded); ++i){
          if(url_decoded[i] == '\n'){            
            url_decoded[i] = 0;            
            break;
          }
        }

        int decode_len = strlen(url_decoded);
        if(url_decoded[decode_len-1] != '/'){
          url_decoded[decode_len] = '/';
          url_decoded[decode_len+1] = 0;    
          decode_len++;          
        }
            
        ret = mg_get_http_var(&(hm->message), HTTP_CMD_DELETE, value, sizeof(value));
        if(ret > 0){ 
          memcpy(cmd, SYS_CMD_DEL, sizeof(SYS_CMD_DEL));
          cmd[strlen(cmd)] = '"';
          memcpy(cmd + strlen(cmd), s_http_server_opts.document_root, strlen(s_http_server_opts.document_root));  
          cmd_len = strlen(cmd);
        
          // Append relative target path
          memcpy(cmd + cmd_len, url_decoded, decode_len);         

          // Append target
          memcpy(cmd + cmd_len + decode_len, value, ret);
          cmd[strlen(cmd)] = '"';

          // printf("CMD: %s\n", cmd);          
          system(cmd);

          mg_http_send_error(nc, 301, NULL);
          return;
        }

        // Check archive command
        ret = mg_get_http_var(&(hm->message), HTTP_CMD_ARCHIVE, value, sizeof(value));
        if(ret > 0){
          // Form command
          cmd[0] = 'c';
          cmd[1] = 'd';
          cmd[2] = ' ';

          // Add path to chdir to. Start with root dir
          cmd[3] = '"';
          memcpy(cmd + 4, s_http_server_opts.document_root, strlen(s_http_server_opts.document_root));           
        
          // Append relative target path          
          memcpy(cmd + strlen(cmd), url_decoded, decode_len);

          cmd_len = strlen(cmd);
          cmd[cmd_len] = '"';
          cmd[cmd_len + 1] = ' ';
          cmd[cmd_len + 2] = '&';
          cmd[cmd_len + 3] = '&';
          cmd[cmd_len + 4] = ' ';
          
          memcpy(cmd + cmd_len + 5, SYS_CMD_ARCHIVE, sizeof(SYS_CMD_ARCHIVE));
          // Add archive name
          cmd_len = strlen(cmd);
          cmd[cmd_len] = '"';
          memcpy(cmd + cmd_len + 1, value, ret);

          cmd_len = strlen(cmd);
          if(cmd[cmd_len - 1] == '/'){            
            cmd_len--;
          }

          memcpy(cmd + cmd_len, SYS_ARCHIVE_EXT, sizeof(SYS_ARCHIVE_EXT));           

          // Append target
          memcpy(cmd + strlen(cmd), value, ret);
          cmd[strlen(cmd)] = '"';

          // printf("CMD: %s\n", cmd); 
          system(cmd);

          mg_http_send_error(nc, 301, NULL);
          return;
        }

        // Check new dir command
        ret = mg_get_http_var(&(hm->message), HTTP_CMD_NEW_DIR, value, sizeof(value));
        if(ret > 0){
           // Form command
          cmd[0] = 'c';
          cmd[1] = 'd';
          cmd[2] = ' ';

          // Add path to chdir to. Start with root dir
          cmd[3] = '"';
          memcpy(cmd + 4, s_http_server_opts.document_root, strlen(s_http_server_opts.document_root));           
        
          // Append relative target path          
          memcpy(cmd + strlen(cmd), url_decoded, decode_len);

          cmd_len = strlen(cmd);
          cmd[cmd_len] = '"';
          cmd[cmd_len + 1] = ' ';
          cmd[cmd_len + 2] = '&';
          cmd[cmd_len + 3] = '&';
          cmd[cmd_len + 4] = ' ';
          
          memcpy(cmd + cmd_len + 5, SYS_CMD_NEW_DIR, sizeof(SYS_CMD_NEW_DIR));
          // Add dir name
          memcpy(cmd + strlen(cmd), value, ret);
          cmd[strlen(cmd)] = '"';

          // printf("CMD: %s\n", cmd); 
          system(cmd);

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
