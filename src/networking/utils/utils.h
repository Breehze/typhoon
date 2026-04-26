/**
 * @file utils.h
 * @brief Socket creation and connection utilities
 *
 * Provides helper functions for creating TCP and Unix domain sockets
 * in both server (listening) and client (connecting) modes.
 */

#pragma once

/**
 * Creates a TCP listening socket.
 *
 * @param ip_addr IP address to bind to (NULL for INADDR_ANY)
 * @param port    Port number to listen on
 * @return Socket file descriptor
 */
int server_listen(const char *ip_addr, unsigned short port);

/**
 * Creates a Unix domain socket for listening.
 *
 * @param socket_path Path for the socket file
 * @return Socket file descriptor
 */
int server_listen_UDS(const char *socket_path);

/**
 * Connects to a TCP server.
 *
 * @param ip_address Server IP address
 * @param port       Server port number
 * @return Socket file descriptor, or -1 on error
 */
int connect_TCP(const char *ip_address, int port);

/**
 * Connects to a Unix domain socket server.
 *
 * @param path Path to the socket file
 * @return Socket file descriptor, or -1 on error
 */
int connect_UDS(const char *path);
