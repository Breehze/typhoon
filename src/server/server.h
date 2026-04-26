/**
 * @file server.h
 * @brief Shell server mode implementation
 *
 * Accepts incoming connections and spawns PTY-backed shell sessions.
 * Supports multiple simultaneous clients with inactivity timeout.
 */

#pragma once
#include <time.h>

/**
 * Runs the shell in server mode.
 * Listens for connections and spawns shell sessions for each client.
 *
 * @param ip_addr          IP address to bind to (for TCP)
 * @param port             Port number to listen on (for TCP)
 * @param unix_socket_path Path for Unix domain socket (alternative to TCP)
 * @param conn_timeout     Inactivity timeout in seconds
 * @return 0 on success, non-zero on error
 */
int run_server(char * ip_addr, short int port, char * unix_socket_path, time_t conn_timeout);
