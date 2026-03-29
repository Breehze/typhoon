#include <stdlib.h>

#include "connection.h"


Connection * spawnConnection(int socket_fd){
    Connection * newConnection = (Connection *)malloc(sizeof(Connection));
    *newConnection = (Connection){
        .socket_fd = socket_fd,
        .pty_fd = -1,
        .child_pid = -1,
        .remove = 0
    };
    return newConnection;
}
