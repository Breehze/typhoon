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
    int i;
    for(i = 0;i<fdset->len;i++){
        if(fdset->descriptors[i].fd == fd) break;
    }
    close(fdset->descriptors[i].fd);
    fdset->descriptors[i] = fdset->descriptors[fdset->len - 1];
    fdset->len--;
}


void fdset_clean(FileDescriptorSet * fdset){
    free(fdset->descriptors);
    fdset->capacity = 0;
    fdset->len = 0;
}
