#include "process.h"
void Processon()
{
  int cnt=0;
  char bar[NUM];
  memset(bar,'\0',sizeof(bar));

const char* lable="|\\-/";

  while(cnt<=100)
  {
   // printf("[%-100s][%3d%%][%c]\r",bar,cnt,lable[cnt%4]);
    printf("\033[1;34;41m[%-100s][%3d%%][%c]\033[0m\r",bar,cnt,lable[cnt%4]);
    fflush(stdout);
    bar[cnt++]=STYLE;
    usleep(50000);
  }
  printf("\n");
}

