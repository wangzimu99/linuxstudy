#pragma once
#include "thread.hpp"
#include "LockGuard.hpp"
#include <unistd.h>

using namespace ThreadNs;

const int gnum=5;
template<class T>
class ThreadPool;

template<class T>
class ThreadData
{
public:
    ThreadPool<T>* threadpool;
    std::string name;
public:
    ThreadData(ThreadPool<T>*tp,const std::string &n)
    :threadpool(tp)
    ,name(n)
    {}
};


template<class T>
class ThreadPool
{
private:
        static void* handlertask(void* args)
    {
        ThreadData<T>* td=static_cast<ThreadData<T>*>(args);
        while(true)
        {
            // sleep(1);
            // std::cout<<"handlertask thread"<<pthread_self()<<" running..."<<std::endl;
            td->threadpool->lockQueue();
            while(td->threadpool->isQueueEmpty())
            {
                td->threadpool->threadWait();
            }
            T t=td->threadpool->pop();
            td->threadpool->unlockQueue();
            std::cout<<td->name<<"获取了一个任务"<<t.totaskstring()<<" 并处理完成，结果是"<<t()<<std::endl;;//处理任务
        }
        return nullptr;
    }
public:
    void lockQueue()
    {
        pthread_mutex_lock(&_mutex);
    }

    void unlockQueue()
    {
        pthread_mutex_unlock(&_mutex);
    }

    bool isQueueEmpty()
    {
        return _task_queue.empty();
    }

    void threadWait()
    {
        pthread_cond_wait(&_cond,&_mutex);

    }
public:
    ThreadPool(const int& num=gnum)
    :_num(num)
    {
        pthread_mutex_init(&_mutex,nullptr);
        pthread_cond_init(&_cond,nullptr);
        for(int i=0;i<_num;++i)
        {
            _threads.push_back(new Thread());
        }
    }

    void run()
    {
        for(const auto& t:_threads)
        {
            ThreadData<T> *td=new ThreadData<T>(this,t->threadname());
            t->start(handlertask,td);
            std::cout<<t->threadname()<<"start..."<<std::endl;
        }
    }

    void push(const T& in)
    {
        pthread_mutex_lock(&_mutex);
        _task_queue.push(in);
        pthread_cond_signal(&_cond);
        pthread_mutex_unlock(&_mutex);
    }

    T pop()
    {
        T t=_task_queue.front();
        _task_queue.pop();
        return t;
    }

    ~ThreadPool()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
        for(const auto&t:_threads)
        {
            delete t;
        }
    }
private:
    int _num;
    std::vector<Thread*>  _threads;
    std::queue<T>         _task_queue;
    pthread_mutex_t       _mutex;
    pthread_cond_t        _cond;
};

