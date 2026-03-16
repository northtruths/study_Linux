#include"my_stdio.h"
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

mFILE* mfopen(const char* filename, const char* mode){
    int fd = -1;
    if(strcmp(mode, "w") == 0){
        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    }else if(strcmp(mode, "r") == 0){
        fd = open(filename, O_RDONLY);
    }else if(strcmp(mode, "a")){
        fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
    }else if(strcmp(mode, "w+") == 0){
        fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
    }else if(strcmp(mode, "r+") == 0){
        fd = open(filename, O_RDWR);
    }else if(strcmp(mode, "a+")){
        fd = open(filename, O_RDWR | O_CREAT | O_APPEND, 0666);
    }else{
        return NULL;
    }   
   if(fd < 0)
       return NULL;

    mFILE* mf = (mFILE*)malloc(sizeof(mFILE));
    if(mf == NULL)
        return NULL;

    mf->flag = FLUSH_NONE;
    mf->fileno = fd;
    mf->cap = SIZE;
    mf->size = 0;

    return mf;
}

int mfwrite(const void* ptr, int size, int num, mFILE* stream){
    if(size <= 0 || num <= 0)
        return 0;
    memcpy(stream->out_buffer + stream->size, ptr, size * num);
    stream->size += size * num;

    //检查是否刷新缓存(当下只管行刷新)
    if(stream->flag == FLUSH_LINE && stream->size && stream->out_buffer[stream->size - 1] == '\n'){
        mfflush(stream);
    }

    return size * num;
}

int mfread(void* ptr, int num, mFILE* stream){
    if(num <= 0)
        return 0;
    int n = read(stream->fileno, ptr, num);
    if(n <= 0)
        return 0;
    return n;
}

void mfflush(mFILE* stream){
    if(stream->size <= 0)
        return;

    write(stream->fileno, stream->out_buffer, stream->size);
    stream->size = 0;
    //刷新系统缓冲区
    fsync(stream->fileno);
}

void mfclose(mFILE* stream){
    mfflush(stream);
    close(stream->fileno);
    free(stream);
}
