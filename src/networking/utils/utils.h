#pragma once

int server_listen(const char *ip_addr, unsigned short port);
int server_listen_UDS(const char *socket_path);

int connect_TCP(const char *ip_address, int port);
int connect_UDS(const char *path);
