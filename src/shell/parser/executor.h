/**
 * @file executor.h
 * @brief Command execution engine
 *
 * Executes parsed shell commands using fork/exec system calls.
 * Handles built-in commands (help, quit, halt, cd) and external programs.
 */

#pragma once
#include "types.h"

/**
 * Parses and executes a command string.
 *
 * @param string The command string to execute
 * @param err    Pointer to error status (can be NULL)
 */
void executor_exec(char * string, Error * err);
