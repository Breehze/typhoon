/**
 * @file client.h
 * @brief Shell client mode implementation
 *
 * Connects to a remote shell server and forwards stdin/stdout
 * over TCP or Unix domain sockets.
 */

#pragma once

/**
 * Runs the shell in client mode.
 * Connects to a server and relays input/output.
 *
 * @param ip_addr          Server IP address (for TCP connections)
 * @param port             Server port number (for TCP connections)
 * @param unix_socket_path Path to Unix domain socket (alternative to TCP)
 */
void run_client(char * ip_addr, short int port, char * unix_socket_path);
