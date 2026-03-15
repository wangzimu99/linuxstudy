#include "RingQueue.hpp"
#include "task.hpp"
#include <pthread.h>
#include <ctime>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>

std::string ThreadName()
{
    char name[128];
    snprintf(name,sizeof(name),"thread[0x%x]",pthread_self());
    return name;
}

void* ProductorRoutine(void* args)
{
    //RingQueue<int> *rq=static_cast<RingQueue<int>*>(args);
    RingQueue<Task> *rq=static_cast<RingQueue<Task>*>(args);
    while(true)
    {
        // //version1
        // sleep(3);
        // int data=rand()%10+1;
        // Task tk;
        // rq->push(data);
        // std::cout<<"生产完成，生产的数据是："<<data<<std::endl;

        //version2
        int x=rand()%10;
        int y=rand()%15;
        char op=oper[rand()%oper.size()];
        Task t(x,y,op,mymath);
        rq->push(t);
        std::cout<<ThreadName()<<",生产者派发了一个任务："<<t.totaskstring()<<std::endl;
    }
}

void* ConsumerRoutine(void* args)
{
    RingQueue<Task> *rq=static_cast<RingQueue<Task>*>(args);
    while(true)
    {
        //version1
        // int data;
        // rq->pop(&data);
        // std::cout<<"消费完成，消费的数据是："<<data<<std::endl;

        //version2
        Task t;
        rq->pop(&t);
        std::string result=t();
        std::cout<<ThreadName()<<",消费者消费了一个任务："<<result<<std::endl;
        sleep(1);
    }
}

int main()
{
    srand((unsigned int)time(nullptr)^getpid()^pthread_self());
    RingQueue<Task> *rq=new RingQueue<Task>();
    pthread_t c[8],p[4];
    for(int i=0;i<8;++i)
    pthread_create(c+i,nullptr,ConsumerRoutine,rq);
    for(int i=0;i<4;++i)
    pthread_create(p+i,nullptr,ProductorRoutine,rq);

    for(int i=0;i<8;++i)
    pthread_join(c[i],nullptr);
    for(int i=0;i<4;++i)
    pthread_join(p[i],nullptr);

    delete rq;
    return 0; 
}