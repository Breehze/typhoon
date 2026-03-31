#include <termios.h>
#include <stdlib.h>
#include <unistd.h>

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
