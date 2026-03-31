#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <stdlib.h>

#include "parser/executor.h"

struct tm * getCurrTime(){
    time_t now = time(NULL);
    struct tm * time = localtime(&now);
    return time;
}

char * getUsername(){
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);

    return pw->pw_name;  
}

char * getCwd(char * buffer,size_t buffer_size){
    return getcwd(buffer,buffer_size);
}

void run_shell(){
    char buffer[1024] = {0};
    char cwd_buff[1024] = {0};
    int pos = 0;
    struct tm * time = getCurrTime();
    fprintf(stdout,"%d:%d %s %s>",time->tm_hour,time->tm_min,getUsername(),getCwd(cwd_buff,sizeof(cwd_buff)));
    fflush(stdout);
    while(0xB00BA){
        char c;
        time = getCurrTime();
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
            fprintf(stdout,"\x1b[30m\x1b[41m%d:%d◥\x1b[0m %s ➤  ",time->tm_hour,time->tm_min,getUsername());
            fflush(stdout);
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
