#pragma once
#include <iostream>
#include <queue>
#include <pthread.h>

const int gmaxcap=5;
template <class T>
class BlockQueue
{
public:
    BlockQueue(const int maxcap=gmaxcap)
    :_maxcap(maxcap)
    {
        pthread_mutex_init(&_mutex,nullptr);
        pthread_cond_init(&_pcond,nullptr);
        pthread_cond_init(&_ccond,nullptr);

    }

    void push(const T& in)
    {
        pthread_mutex_lock(&_mutex);//加锁
        //1.判断
        //充当条件判断的语法必须是while，不能是if
        while(is_full())
        //pthread_cond_wait这个函数的第二个参数，必须是我们正在使用的互斥锁
        //pthread_cond_wait:该函数在调用的时候，会以原子性的方式，将锁释放，并将自己挂起
        //pthread_cond_wait:该函数在被唤醒返回的时候，会自动的重新获取传入的锁
            pthread_cond_wait(&_pcond,&_mutex);//生产条件不满足，生产者进行等待
        //2.放数据
        _q.push(in);
        //3.保证队列里一定有数据
        //pthread_cond_signal:此函数可以放在临界区内部，也可以放在外部
        pthread_cond_signal(&_ccond);
        pthread_mutex_unlock(&_mutex);

    }

    void pop(T* out)//一般而言，输出型参数：*，输入输出型参数：&
    {
        pthread_mutex_lock(&_mutex);
        while(is_empty())
            pthread_cond_wait(&_ccond,&_mutex);
        *out=_q.front();
        _q.pop();
        pthread_cond_signal(&_pcond);//保证队列里至少有一个空位置
        pthread_mutex_unlock(&_mutex);
    }

    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_pcond);
        pthread_cond_destroy(&_ccond);
    }
private:
    bool is_empty()
    {
        return _q.empty();
    }

    bool is_full()
    {
        return _q.size()==_maxcap;
    }

private:
    std::queue<T> _q;
    int _maxcap;//丢列中元素上限
    pthread_mutex_t _mutex;//保证该临界资源的安全
    pthread_cond_t _pcond;//生产者对应的条件变量
    pthread_cond_t _ccond;//消费者对应的条件变量
};