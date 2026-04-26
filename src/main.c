#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "client/client.h"
#include "server/server.h"
#include "terminal/terminal.h"

#define NOT_SET 0
#define SERVER 1
#define CLIENT 2

int main(int argc,char *argv[]){
    int mode = NOT_SET; 
    char * ip = "127.0.0.1";
    short int port = -1;
    char * unix_socket = NULL;
    time_t timeout = 120;
    
    static struct option long_options[] = {
        {"help", no_argument, 0,'h'},
        {"usock", required_argument, 0,'u'},
        {"port", required_argument,0,'p'},
        {"ipaddr",required_argument,0,'i'},
        {"server",no_argument,0,'s'},
        {"client",no_argument,0,'c'},
        {"timeout",required_argument,0,'t'},
        {0, 0, 0, 0}
    };
    
    int c;
    while ((c = getopt_long(argc, argv, "hu:p:i:sct:", long_options, NULL)) != -1) {
        switch (c) {
            case 'h':
                print_usage("typhoon");
                return 0;
            case 'p': {
                char *endptr;
                errno = 0;
                long val = strtol(optarg, &endptr, 10);
                if (errno != 0 || *endptr != '\0' || val < 1 || val > 65535) {
                    fprintf(stderr, "Invalid port: %s\n", optarg);
                    return 1;
                }
                port = (short)val;
                break;
            }
            case 'i':
                ip = strdup(optarg);
                break;
            case 'c':
                mode = 2;
                break;
            case 's':
                mode = 1;
                break;
            case 'u':
                unix_socket = strdup(optarg);
                break;
            case 't': {
                char *endptr;
                errno = 0;
                long val = strtol(optarg, &endptr, 10);
                if (errno != 0 || *endptr != '\0' || val < 1) {
                    fprintf(stderr, "Invalid timeout: %s\n", optarg);
                    return 1;
                }
                timeout = val;
                break;
            }
            case '?':
                break;
        }
    }
    if(!mode){
        mode = SERVER;
    }
    
    if(mode == SERVER){
        if(port == -1 && !unix_socket){
            printf("Please select a port to run on!");
            return 1;
        }
        run_server(ip,port,unix_socket,timeout);
    }else if(mode == CLIENT){
        if(port == -1 && !unix_socket){
            printf("Please select a port to run on!");
            return 1;
        }
        run_client(ip,port,unix_socket);
    }

    return 0;
}
