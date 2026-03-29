#include <unistd.h>
#include <string.h>

#include "parser/executor.h"

void run_shell(){
    char buffer[1024] = {0};
    int pos = 0;
    write(1, "boris@zephyr ~/ ", 16);
    while(0xB00BA){
        char c;
        int n = read(0, &c, 1);
        if(n <= 0) break;

        if(c == '\r' || c == '\n'){
            buffer[pos] = '\0';
            write(1, "\r\n", 2);
            if(pos > 0){
                executor_exec(buffer, NULL);
            }
            pos = 0;
            memset(buffer, 0, sizeof(buffer));
            write(1, "boris@zephyr ~/ ", 16);
        } else if(c == 127 || c == '\b'){
            if(pos > 0){
                pos--;
                write(1, "\b \b", 3);
            }
        } else {
            buffer[pos++] = c;
            write(1, &c, 1);
        }
    }
}
