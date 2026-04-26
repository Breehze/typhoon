/**
 * @file fdset.h
 * @brief Dynamic file descriptor set for poll()
 *
 * Provides a dynamically growing array of pollfd structures
 * for managing multiple file descriptors with poll().
 */

#pragma once

/** Dynamic array of poll file descriptors */
typedef struct {
    struct pollfd * descriptors;  /** Array of pollfd structures */
    int len;                      /** Current number of descriptors */
    int capacity;                 /** Allocated capacity */
} FileDescriptorSet;

/**
 * Initializes a file descriptor set.
 *
 * @param fdset Set to initialize
 */
void fdset_init(FileDescriptorSet * fdset);

/**
 * Adds a file descriptor to the set.
 *
 * @param fdset Set to add to
 * @param fd    File descriptor to add
 * @param mode  Poll events to monitor (e.g., POLLIN)
 */
void fdset_add(FileDescriptorSet * fdset, int fd, int mode);

/**
 * Removes a file descriptor from the set.
 *
 * @param fdset Set to remove from
 * @param fd    File descriptor to remove
 */
void fdset_remove(FileDescriptorSet *fdset, int fd);

/**
 * Frees all memory associated with the set.
 *
 * @param fdset Set to clean up
 */
void fdset_clean(FileDescriptorSet * fdset);
