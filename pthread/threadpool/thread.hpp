// 封装pthread库
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <cstring>
#include <assert.h>
#include <functional>
#include <pthread.h>

namespace ThreadNs{

typedef std::function<void *(void *)> func_t;
const int num = 1024;

class Thread
{
private:
    static void *start_routine(void *args) // 静态成员函数不能调用非静态成员变量，非静态成员函数有this指针，即需要两个参数
    {
        Thread *_this = static_cast<Thread *>(args);
        return  _this->callback();
    }
public:
    Thread()
    {
        char namebuffer[num];
        snprintf(namebuffer, sizeof(namebuffer), "thread-%d", threadnum++);
        _name = namebuffer;
    }

    void start(func_t func, void *args = nullptr)
    {
        _func=func;
        _args=args;
        int n = pthread_create(&_tid, nullptr, start_routine, this);
        assert(n == 0);
        (void)n;
    }

    void join()
    {
        int n = pthread_join(_tid, nullptr);
        assert(n == 0);
        (void)n;
    }

    std::string threadname()
    {
        return _name; 
    }

    void *callback()
    {
        return _func(_args);
    }

    ~Thread()
    {
    }

private:
    std::string _name;
    pthread_t _tid;
    func_t _func;
    void *_args;
    static int threadnum;
};
int Thread::threadnum=1;
}