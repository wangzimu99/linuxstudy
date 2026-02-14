#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include  <assert.h>
#include <ctype.h>
#include <errno.h>
#define NUM          1024
#define OPT_NUM      64
#define NONE_REDIR   0
#define INPUT_REDIR  1
#define OUTPUT_REDIR 2
#define APPEND_REDIR 3

#define trimSpace(start) do{\
                                 while(isspace(*start)) ++start;\
                           }while(0)


char  lineCommand[NUM];
char* myargv[OPT_NUM];//指针数组
char* redirFile=NULL;
int   lastCode=0;
int   lastSig=0;
int   redirType=NONE_REDIR;


//"ls -a -l > myfile.c"->"ls -a -l" "myfile.c"
void commandCheck(char* commands)
{
    assert(commands);
    char* start=commands;
    char* end=commands+strlen(commands);
    while(start<end)
    {
        if(*start=='>')
        {
            *start='\0';
            start++;
           if(*start=='>')
           {
                redirType=APPEND_REDIR;
                start++;
           }
           else 
           {
               redirType=OUTPUT_REDIR;
           }
           trimSpace(start);
           redirFile=start;
           break;
        }
        else if(*start=='<')
        {
            
            *start='\0';
            start++;
            trimSpace(start);
            //填写重定向信息
            redirType=INPUT_REDIR;
            redirFile=start;
            break;
        }
        else 
        {
            start++;
        }
    }
}

int main()
{
    while(1)
    {
    //初始化
    redirType=NONE_REDIR;
    redirFile=NULL;
    errno=0;
    //输出提示符
    printf("用户名@主机名 当前路径# ");
    fflush(stdout);

    //获取用户输入
    char* s=fgets(lineCommand,sizeof(lineCommand)-1,stdin);
    assert(s!=NULL);
    (void)s;
    //清楚最后一个\n
    lineCommand[strlen(lineCommand)-1]=0;

    //"ls -a -l > myfile.c"->"ls -a -l" "myfile.c"
    commandCheck(lineCommand);



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
        //命令是子进程执行的，真正重定向的工作也是由子进程来完成的
        //如何重定向，是父进程要给子进程提供信息的
        //此处的重定向不会影响父进程，子进程的PCB和文件描述符表是对父进程的拷贝，进程具有独立性
        switch(redirType)
        {
            case NONE_REDIR:
                execvp(myargv[0],myargv);//执行程序替换的时候，不会影响曾经程序打开的重定向的文件
                exit(1);
                break;
            case INPUT_REDIR:
                {
                    if(redirFile==NULL||*redirFile=='\0')
                    {
                        fprintf(stderr,"missing redirection filename \n");
                        exit(errno);
                    }
                    int fd=open(redirFile,O_RDONLY);
                    if(fd<0)
                    {
                        perror("open");
                        exit(errno);
                    }
                    //重定向的文件已经成功打开
                    dup2(fd,0);
                execvp(myargv[0],myargv);//执行程序替换的时候，不会影响曾经程序打开的重定向的文件
                exit(1);
                close(fd);
                }
                break;
            case OUTPUT_REDIR:
            case APPEND_REDIR:
                {
                    umask(0);
                    if(redirFile==NULL||*redirFile=='\0')
                    {
                        fprintf(stderr,"missing redirection filename \n");
                        exit(errno);
                    }
                    int flags=O_WRONLY|O_CREAT;
                    if(redirType==APPEND_REDIR)
                    {
                        flags|=O_APPEND;
                    }
                    else 
                    {
                        flags|=O_TRUNC;
                    }
                    int fd=open(redirFile,flags,0666);
                    if(fd<0)
                    {
                        perror("open");
                        exit(errno);
                    }
                    //输出重定向的文件已经打开
                    dup2(fd,1);
                execvp(myargv[0],myargv);//执行程序替换的时候，不会影响曾经程序打开的重定向的文件
                exit(1);
                close(fd);
                }
                break;
            default:
                printf("bug?\n");
                break;
        }
    }
    int statue=0;

   pid_t ret=waitpid(id,&statue,0);
   assert(ret>0);
   (void)ret;
    lastCode=((statue>>8)&0xFF);
    lastSig=(statue&0x7F); 
    }
}
