#include <ctime>
#include <sys/types.h>
#include <unistd.h>
#include "BlockQueue.hpp"
#include "task.hpp"


//C:计算，S：存储
template<class C,class S>
class BlockQueues
{
    public:
    BlockQueue<C>* c_bq;
    BlockQueue<S>* s_bq;

};

void *productor(void *bq_)
{
#define OPER
    BlockQueue<CalTask> *cal_bq = (static_cast<BlockQueues<CalTask,SaveTask> *>(bq_))->c_bq;
    while (true)
    {
        int x = rand() % 10 + 1;
        int y = rand() % 5;
        int opcode=rand()%oper.size();

        CalTask t(x, y,oper[opcode], mymath);
        cal_bq->push(t);
        std::cout<<"producer thread,生成计算任务："<<t.totaskstring()<<std::endl;
        sleep(1);
    }
    return nullptr;
}

void *consumer(void *bq_)
{
    BlockQueue<CalTask> *cal_bq = (static_cast<BlockQueues<CalTask,SaveTask> *>(bq_))->c_bq;
    BlockQueue<SaveTask> *save_bq = (static_cast<BlockQueues<CalTask,SaveTask> *>(bq_))->s_bq;
    while (true)
    {
        CalTask t;
        cal_bq->pop(&t);
        std::string result=t();
        std::cout << "cal thread,完成计算任务：" << result<<"...done" << std::endl;

        SaveTask s(result,Save);
        save_bq->push(s);
        std::cout<<"cal thread,完成推送任务..."<<std::endl;

        //sleep(1);
    }
    return nullptr;
}

void* saver(void* bq_)
{
    BlockQueue<SaveTask> *save_bq = (static_cast<BlockQueues<CalTask,SaveTask> *>(bq_))->s_bq;
    while(true)
    {
        SaveTask s;
        save_bq->pop(&s);
        s();
        std::cout<<"save thread,完成保存任务..."<<std::endl;
    }
    return nullptr;
}

int main()
{

    BlockQueues<CalTask,SaveTask> bqs;
    srand((unsigned long)time(nullptr) ^ getpid());
    bqs.c_bq = new BlockQueue<CalTask>();
    bqs.s_bq = new BlockQueue<SaveTask>();

    pthread_t c, p,s;
    pthread_create(&c, nullptr, consumer, &bqs);
    pthread_create(&p, nullptr, productor,&bqs);
    pthread_create(&s, nullptr, saver,    &bqs);

    pthread_join(c, nullptr);
    pthread_join(p, nullptr);
    pthread_join(s, nullptr);

    delete bqs.c_bq;
    delete bqs.s_bq;
    return 0;
}