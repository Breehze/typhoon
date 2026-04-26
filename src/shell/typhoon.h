/**
 * @file typhoon.h
 * @brief Main shell loop interface
 *
 * Provides the interactive shell loop that displays the prompt,
 * reads user input, and dispatches commands for execution.
 */

#pragma once

/**
 * Runs the interactive shell loop.
 * Displays prompt with time, username, and hostname.
 * Reads commands and executes them until EOF or exit.
 */
void run_shell();
