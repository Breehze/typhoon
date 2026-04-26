/**
 * @file terminal.h
 * @brief Terminal configuration and help display
 *
 * Provides terminal setup (raw mode) and program usage information.
 */

#pragma once

/**
 * Configures terminal for raw input mode.
 * Disables echo and canonical mode for character-by-character input.
 * Registers cleanup handler to restore settings on exit.
 */
void terminal_setup();

/**
 * Prints program usage information.
 * Displays author, purpose, command-line options, and built-in commands.
 *
 * @param prog_name Name of the program (for usage string)
 */
void print_usage(const char *prog_name);
