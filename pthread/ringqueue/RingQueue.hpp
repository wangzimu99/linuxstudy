#pragma once
#include <iostream>
#include <vector>
#include <semaphore.h>
#include <cassert>
#include <pthread.h>
static const int gcap=10;
template<class T>
class RingQueue
{
private:
    void P(sem_t &sem)
    {
        int n=sem_wait(&sem);
        assert(n==0);
        (void)n;
    }

    void V(sem_t &sem)
    {
        int n=sem_post(&sem);
        assert(n==0);
        (void)n;
    }

public:
    RingQueue(const int &cap=gcap)
    :_queue(cap)
    ,_cap(cap)
    {
        sem_init(&_spacesem,0,_cap);
        sem_init(&_datasem,0,0);
        _productorstep=_consumerstep=0;
        pthread_mutex_init(&_pmutex,nullptr);
        pthread_mutex_init(&_cmutex,nullptr);
    }

    void push(const T& in)
    {
        P(_spacesem);//申请到了空间信号量，可以正常生产
        pthread_mutex_lock(&_pmutex);
        _queue[_productorstep++]=in;
        _productorstep%=_cap;
        pthread_mutex_unlock(&_pmutex);
        V(_datasem);
    }

    void pop(T* out)
    {
        P(_datasem);
        pthread_mutex_lock(&_cmutex);
        *out=_queue[_consumerstep++];

        _consumerstep%=_cap;
        pthread_mutex_unlock(&_cmutex);
        V(_spacesem);
    }

    ~RingQueue()
    {
        sem_destroy(&_spacesem);
        sem_destroy(&_datasem);
        pthread_mutex_destroy(&_pmutex);
        pthread_mutex_destroy(&_cmutex);
    }

private:
    std::vector<T>  _queue;
    int             _cap;
    sem_t           _spacesem;//生产者对应的空间资源信号量
    sem_t           _datasem;//消费者对应的数据资源信号量
    int             _productorstep;
    int             _consumerstep;
    pthread_mutex_t _pmutex;
    pthread_mutex_t _cmutex;
};
