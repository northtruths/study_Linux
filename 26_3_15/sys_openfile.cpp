#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

int main(){
    int fd = open("bite", O_RDWR | O_CREAT | O_TRUNC, 0666);
    const char* message = "i like linux!";
    write(fd, message, strlen(message));
    lseek(fd, 0, SEEK_SET);
    char send[100];
    read(fd, send, sizeof(send));
    close(fd);
    printf("%s\n", send);
    return 0;
}
