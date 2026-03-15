#include<stdio.h>
#include<string.h>

int main(){
    FILE* fp = fopen("./bite", "w+");
    const char* message = "linux so easy!";
    fwrite(message, strlen(message), 1, fp);
    char send[1024];
    fseek(fp, 0, SEEK_SET);
    fread(send, 1, sizeof(send), fp);
    printf("%s\n", send);
    fclose(fp);
    return 0;
}
