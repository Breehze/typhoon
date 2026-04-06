#pragma once

int server_listen(unsigned short port);
int server_listen_UDS(const char *socket_path);
