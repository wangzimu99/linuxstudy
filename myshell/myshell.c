#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include  <assert.h>
#define NUM 1024
#define OPT_NUM 64
char lineCommand[NUM];
char* myargv[OPT_NUM];//指针数组
int lastCode=0;
int lastSig=0;
int main()
{
    while(1)
    {
    //输出提示符
    printf("用户名@主机名 当前路径# ");
    fflush(stdout);

    //获取用户输入
    char* s=fgets(lineCommand,sizeof(lineCommand)-1,stdin);
    assert(s!=NULL);
    (void)s;
    //清楚最后一个\n
    lineCommand[strlen(lineCommand)-1]=0;
    //字符串切割
    myargv[0]=strtok(lineCommand," ");
    int i=1;
    
    if(myargv[0]!=NULL&&strcmp(myargv[0],"ls")==0)
    {
        myargv[i++]=(char*)"--color=auto";
    }
    while(myargv[i++]=strtok(NULL," "));
    //如果是cd命令，不需要创建子进程，shell自己执行对应的命令
    //不需要子进程来执行，二十shell自己执行的命令叫做内建/内置命令
    if(myargv[0]!=NULL&&strcmp(myargv[0],"cd")==0)
    {
        if(myargv[1]!=NULL)
        {
            chdir(myargv[1]);
        }
        continue;
    }

    if(myargv[0]!=NULL&&myargv[1]!=NULL&&strcmp(myargv[0],"echo")==0)
    {
        if(strcmp(myargv[1],"$?")==0)
        {
            printf("%d,%d\n",lastCode,lastSig);    
        }
        else 
        {
            printf("%s\n",myargv[1]);
        }
        continue;
    }

#ifdef DEBUG 
    for(int i=0;myargv[i];++i)
    {
        printf("myargv[%d]:%s\n",i,myargv[i]);
    }
#endif
    
    pid_t id=fork();
    assert(id!=-1);
    if(id==0)
    {
        execvp(myargv[0],myargv);
        exit(1);
    }
    int statue=0;

   pid_t ret=waitpid(id,&statue,0);
   assert(ret>0);
   (void)ret;
    lastCode=((statue>>8)&0xFF);
    lastSig=(statue&0x7F); 
    }
}
