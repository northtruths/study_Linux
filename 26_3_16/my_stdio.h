#pragma once

#define SIZE 1024

#define FLUSH_NONE 0
#define FLUSH_LINE 1
#define FLUSH_FULL 2

struct MyFile{
    int flag;//刷新方式
    int fileno;//文件描述符
    char out_buffer[SIZE];
    int cap;//out_buffer容量
    int size;//out_buffer大小
};

typedef struct MyFile mFILE;

mFILE* mfopen(const char* filename, const char* mode);
int mfwrite(const void* ptr, int size, int num, mFILE* stream);
int mfread(void* ptr, int num, mFILE* stream);
void mfflush(mFILE* stream);
void mfclose(mFILE* stream);
