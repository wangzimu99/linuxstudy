//条件变量演示
#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>


pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
int tickets=1000;
void* start_routine(void* args)
{
    std::string threadname=static_cast<const char*>(args); 
    while(true)
    {
        pthread_mutex_lock(&lock);
        pthread_cond_wait(&cond,&lock);
        std::cout<<threadname<<"->"<<tickets<<std::endl;
        tickets--;
        pthread_mutex_unlock(&lock);

    }

}
int main()
{
    pthread_t p1,p2;
    pthread_create(&p1,nullptr,start_routine,(void*)"thread 1");
    pthread_create(&p2,nullptr,start_routine,(void*)"thread 2");

    while(true)
    {
        sleep(1);
        pthread_cond_signal(&cond);
        std::cout<<"mian thread wakeup one thread..."<<std::endl;
    }
    pthread_join(p1,nullptr);
    pthread_join(p2,nullptr);
    return 0;
}
