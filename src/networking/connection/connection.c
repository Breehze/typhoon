#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "connection.h"


Connection * spawnConnection(int socket_fd){
    Connection * newConnection = (Connection *)malloc(sizeof(Connection));
    if(!newConnection) return NULL;
    *newConnection = (Connection){
        .socket_fd = socket_fd,
        .pty_fd = -1,
        .child_pid = -1,
        .remove = 0,
        .last_ping = time(NULL)

    };
    return newConnection;
}

void flagInactive(Connection ** connections,size_t arr_size,time_t delta){
    time_t snapshot_now = time(NULL);
    for(size_t i = 0;i < arr_size;i++){
        if(!connections[i]){ continue; }
        if(connections[i]->socket_fd == 0) continue;
        if(snapshot_now > connections[i]->last_ping + delta){
            connections[i]->remove = 1;
        }
    }
}

void updateActivity(Connection * connection){
    connection->last_ping = time(NULL);
}
