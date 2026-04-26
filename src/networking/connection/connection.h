/**
 * @file connection.h
 * @brief Client connection management
 *
 * Manages state for connected clients including socket/PTY file descriptors,
 * I/O buffers, and activity tracking for timeout detection.
 */

#pragma once
#include <unistd.h>
#include <time.h>

/** Represents a single client connection */
typedef struct {
    int socket_fd;            /** Client socket file descriptor */
    int pty_fd;               /** PTY master file descriptor */
    pid_t child_pid;          /** PID of shell child process */
    char pty_buffer[1024];    /** Buffer for PTY output */
    char socket_buffer[1024]; /** Buffer for socket input */
    int remove;               /** Flag indicating connection should be removed */
    time_t last_ping;         /** Timestamp of last activity */
} Connection;

/**
 * Creates a new connection structure.
 *
 * @param socket_fd Client socket file descriptor
 * @return Pointer to newly allocated Connection
 */
Connection * spawnConnection(int socket_fd);

/**
 * Marks inactive connections for removal.
 *
 * @param connections Array of connection pointers
 * @param arr_size    Size of the connections array
 * @param delta       Maximum allowed inactivity time in seconds
 */
void flagInactive(Connection ** connections, size_t arr_size, time_t delta);

/**
 * Updates the last activity timestamp for a connection.
 *
 * @param connection Connection to update
 */
void updateActivity(Connection * connection);
