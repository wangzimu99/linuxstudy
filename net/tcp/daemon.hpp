#pragma once
#include <unistd.h>
#include<signal.h>
#include <cstdlib>
#include <cassert>
#include <fcntl.h>

#define DEV "dev/null"
void daemonSelf()
{
    //1、让调用进程忽略掉异常的信号
    signal(SIGPIPE,SIG_IGN);
    //2、如何让自己不是组长
    if(fork()>0)exit(0);
    //子进程---守护进程，精灵进程，本质就是孤儿进程的一种！
    pid_t n=setsid();
    assert(n!=-1);
    //守护进程是脱离终端的，关闭或重定向以前默认打开的文件
    int fd=open(DEV,O_RDWR);
    if(fd>=0)
    {
        dup2(fd,0);
        dup2(fd,1);
        dup2(fd,2);
        close(fd);
    }
    else
    {
        close(0);
        close(1);
        close(2);
    }
    //执行路径是否更改
}