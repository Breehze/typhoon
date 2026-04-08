#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

struct termios orig_termios;


void reset_terminal() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}


void terminal_setup() {
    tcgetattr(STDIN_FILENO, &orig_termios);

    atexit(reset_terminal);

    struct termios t = orig_termios;
    t.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}


void print_usage(const char *prog_name) {
        printf("Author: Boris Ftorek\n");
        printf("Purpose: A multi-mode interactive shell with networking support (SPAASM Assignment 2).\n\n");
        printf("Usage: %s [options]\n", prog_name);
        printf("Options:\n");
        printf("  -h, --help            Show this help message\n");
        printf("  -s, --server          Run in server mode\n");
        printf("  -c, --client          Run in client mode\n");
        printf("  -p, --port <port>     Specify TCP port\n");
        printf("  -i, --ipaddr <ip>     Specify IP address (default: 127.0.0.1)\n");
        printf("  -u, --usock <path>    Specify Unix domain socket path\n");
        printf("Shell built-ins:\n");
        printf("  quit - terminate current remote session\n");
        printf("  halt - terminate the server process\n");
        printf("  help - show usage and additional info\n");
        printf("  cd - change working directory\n");
}
