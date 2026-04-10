#pragma once
#include <unistd.h>
#include <time.h>

typedef struct{
    int socket_fd;
    int pty_fd;
    pid_t child_pid;
    char pty_buffer[1024];
    char socket_buffer[1024];
    int remove;
    time_t last_ping;
}Connection;

Connection * spawnConnection(int socket_fd);
void flagInactive(Connection ** connections,size_t arr_size,time_t delta);
void updateActivity(Connection * connection);
