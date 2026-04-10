#pragma once
#include <time.h>

int run_server(char * ip_addr, short int port, char * unix_socket_path,time_t conn_timeout);
