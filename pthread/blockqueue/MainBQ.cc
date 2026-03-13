#include <ctime>
#include <sys/types.h>
#include <unistd.h>
#include "BlockQueue.hpp"
#include "task.hpp"


int myadd(int x,int y)
{
    return x+y;
}
void* consumer(void* bq_)
{
    BlockQueue<Task> *bq=static_cast<BlockQueue<Task>*>(bq_);
    while(true)
    {
        Task t;
        bq->pop(&t);
        std::cout<<"处理任务："<<t()<<std::endl;
        sleep(1);
    }
    return nullptr;
}

void* productor(void* bq_)
{
    BlockQueue<Task> *bq=static_cast<BlockQueue<Task>*>(bq_);
    while(true)
    {
        int x=rand()%10+1;
        int y=rand()%5+1;
        Task t(x,y,myadd);
        bq->push(t);
        //std::cout<<"生产数据："<<data<<std::endl;
        //sleep(1);
    }
    return nullptr;
}

int main()
{
    srand((unsigned long)time(nullptr)^getpid());
    BlockQueue<Task> *bq=new BlockQueue<Task>();
    pthread_t c,p;
    pthread_create(&c,nullptr,consumer, bq);
    pthread_create(&p,nullptr,productor,bq);

    pthread_join(c,nullptr);
    pthread_join(p,nullptr);
    return 0;
}