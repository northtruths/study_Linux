#include<stdio.h>
#include<unistd.h>

int main(){
    execl("/bin/ls", "-a", "-l", NULL);
    return 0;
}
