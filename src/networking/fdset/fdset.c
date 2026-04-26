#include <stdlib.h>
#include <poll.h>
#include <unistd.h>
#include <assert.h>

#include "fdset.h"


void fdset_init(FileDescriptorSet * fdset){
    fdset->capacity = 10;
    fdset->len = 0;
    fdset->descriptors = (struct pollfd *)malloc(fdset->capacity* sizeof(struct pollfd));
    assert(fdset->descriptors);
}


void fdset_add(FileDescriptorSet * fdset,int fd,int mode){
    if(fdset->len + 1 > fdset->capacity){
        fdset->capacity *= 2;
        struct pollfd * new_fdset_mem = (struct pollfd *)realloc(fdset->descriptors,fdset->capacity * sizeof(struct pollfd));
        fdset->descriptors = new_fdset_mem;
    }
    fdset->descriptors[fdset->len] = (struct pollfd){
        .fd = fd,
        .events = mode 
    };
    fdset->len++;
}


void fdset_remove(FileDescriptorSet *fdset, int fd) {
    int fd_index = -1;
    for(int i = 0;i<fdset->len;i++){
        if(fdset->descriptors[i].fd == fd){
            fd_index = i;
            break;
        }
    }
    if(fd_index == -1){
        return;
    }
    close(fdset->descriptors[fd_index].fd);
    fdset->descriptors[fd_index] = fdset->descriptors[fdset->len - 1];
    fdset->len--;
}


void fdset_clean(FileDescriptorSet * fdset){
    for(int i = 0; i < fdset->len; i++){
        close(fdset->descriptors[i].fd);
    }
    free(fdset->descriptors);
    fdset->capacity = 0;
    fdset->len = 0;
}
