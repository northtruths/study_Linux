#include"my_stdio.h"
#include"my_stdio.c"
#include<string.h>
#include<stdio.h>

int main(){
    mFILE* mf = mfopen("myfile", "w");
    const char* message = "this is my stdio\n";
    mfwrite(message, strlen(message), 1, mf);
    mfclose(mf);
    mFILE* mf_rd = mfopen("myfile", "r");
    char send[100];
    mfread(send, sizeof(send), mf_rd);
    mfclose(mf_rd);

    printf("%s", send);
    return 0;
}
