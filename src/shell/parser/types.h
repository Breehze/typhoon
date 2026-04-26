/**
 * @file types.h
 * @brief Common type definitions for the shell parser
 *
 * Defines data structures used throughout the parser and executor,
 * including the AST node structure and error types.
 */

#pragma once

#include <sys/types.h>

/** Error status codes */
typedef enum err {
    NO_ERR,
    SOME_ERROR,
} Error;

/** Information about a child process with PTY */
typedef struct {
    pid_t pid;       /** Process ID of the child */
    int master_fd;   /** Master file descriptor of the PTY */
} ChildProcessInfo;

/** Types of nodes in the Abstract Syntax Tree */
typedef enum {
    COMMAND,  /** Single command with arguments */
    PIPE,     /** Pipe connecting two commands */
    LIST      /** Sequential list of commands (;) */
} ASTnodeType;

/**
 * Abstract Syntax Tree node for parsed commands.
 * Uses a union to store either command data or child nodes.
 */
typedef struct ASTnode {
    ASTnodeType type;
    union {
        /* COMMAND node data */
        struct {
            char ** command;     /** NULL-terminated argument array */
            char * input_file;   /** Input redirection file (< file) */
            char * output_file;  /** Output redirection file (> file) */
        };
        /* PIPE and LIST node data */
        struct {
            struct ASTnode *left;   /** Left child node */
            struct ASTnode *right;  /** Right child node */
        };
    };
} ASTnode;
