#include"Fifo.hpp"

int main(){
    char buf[1024];
    ff.Read(buf);
    printf("%s\n", buf);
    return 0;
}