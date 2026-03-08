#include<iostream>
#include<unistd.h>
using namespace std;

int main(){
    int id = fork();
    while(1){
    if(id){
        cout << "这是父进程，它的子进程id为：" << id << endl;
    }
    else{
        cout << "这是子进程" << endl;
    }
    sleep(1);
    }
    return 0;
}

