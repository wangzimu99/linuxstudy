#pragma once
#include <iostream>
#include <pthread.h>

class Mutex
{
public:
    Mutex(pthread_mutex_t* plock)
    :_plock(plock)
    {}

    void lock()
    {
        if(_plock) pthread_mutex_lock(_plock);
    }

    void unlock()
    {
        if(_plock) pthread_mutex_unlock(_plock);
    }
    ~Mutex()
    {}

private:
    pthread_mutex_t* _plock;
};

class LockGuard
{
public:
    LockGuard(pthread_mutex_t* mutex)
    :_mutex(mutex)
    {
        _mutex.lock();
    }
    ~LockGuard()
    {
        _mutex.unlock();
    }
private:
    Mutex _mutex;
};