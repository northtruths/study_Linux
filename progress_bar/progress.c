#include"progress.h"

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

int base = 100;//下载基础速率
double total = 5000;//下载目标大小
int cnt = 0;

int NUM = 50;//条形图长度

//条形增长函数
void display(double per){
  char s[NUM + 1];
  char wait[3];
  memset(s, '\0', sizeof(s));
  memset(wait, '\0', sizeof(wait));
  int i = 0;
  for(i = 0; i < (int)(per * NUM); ++i){
    s[i] = '=';
  }
  for(i = 0; i <= cnt; ++i){
    wait[i] = '.';
  }
  printf("[%-50s][%.1lf%%][%-3s]\r", s, per * 100, wait);
  fflush(stdout);
}


//下载的进度条程序
void progress(){
  double cur = 0;//当前下载量
  double rate = rand() %  base * 1.1;//当前下载速率
  while(cur < total){
    cur += rate;
    cnt = (cnt + 1) % 3;
    if(cur >= total)
      cur = total;
    display(cur / total);
    usleep(50000);
  }
  printf("\n");
}
