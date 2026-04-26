#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

#include "utils.h"


int server_listen(const char *ip_addr, unsigned short port){
    struct sockaddr_in address;
    int server_fd = socket(AF_INET,SOCK_STREAM,0);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    if (ip_addr) {
        inet_pton(AF_INET, ip_addr, &address.sin_addr);
    } else {
        address.sin_addr.s_addr = INADDR_ANY;
    }
    address.sin_port = htons(port);
    
    (void)bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd,10);

    return server_fd;
}


int connect_TCP(const char *ip_address, int port) {
    int sock_fd;
    struct sockaddr_in serv_addr;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    inet_pton(AF_INET, ip_address, &serv_addr.sin_addr);

    if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        return -1;
    }

    return sock_fd;
}


int server_listen_UDS(const char *socket_path) {
        struct sockaddr_un address;
        int server_fd;
   
        server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        
   
        memset(&address, 0, sizeof(struct sockaddr_un));
        address.sun_family = AF_UNIX;
        
        strncpy(address.sun_path, socket_path, sizeof(address.sun_path) - 1);
   
        unlink(socket_path);
   
        (void)bind(server_fd, (struct sockaddr *)&address, sizeof(struct sockaddr_un));

        listen(server_fd, 10);
               
        return server_fd;
    }


int connect_UDS(const char *path) {
    int sock_fd;
    struct sockaddr_un serv_addr;

    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd < 0) return -1;

    memset(&serv_addr, 0, sizeof(struct sockaddr_un));
    serv_addr.sun_family = AF_UNIX;
    strncpy(serv_addr.sun_path, path, sizeof(serv_addr.sun_path) - 1);

    if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        close(sock_fd);
        return -1;
    }

    return sock_fd;
}
