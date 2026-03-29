#pragma once

typedef struct{
    struct pollfd * descriptors;
    int len;
    int capacity;
}FileDescriptorSet;

void fdset_init(FileDescriptorSet * fdset);
void fdset_add(FileDescriptorSet * fdset,int fd,int mode);
void fdset_remove(FileDescriptorSet *fdset, int fd);
void fdset_clean(FileDescriptorSet * fdset);

