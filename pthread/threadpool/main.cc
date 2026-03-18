
#include "ThreadPool.hpp"
#include "task.hpp"
#include <memory>
#include <unistd.h>
int main()
{
    std::unique_ptr<ThreadPool<Task>> tp(new ThreadPool<Task>());
    tp->run();
    int x,y;
    char op;
    while(1)
    {
       std::cout<<"请输入数据1#";
       std::cin>>x;
       std::cout<<"请输入数据2#";
       std::cin>>y;
       std::cout<<"请输入处理方法#";
       std::cin>>op;
       Task t;(x,y,op,mymath);
       tp->push(t);

    }
 

    return 0;
}

// #include "ThreadPool.hpp"
// #include "thread.hpp"
// # include <unistd.h>
// void* run(void* args)
// {
//     std::string message=static_cast<const char*>(args);
//     while(true)
//     {
//         std::cout<<message<<std::endl;
//         sleep(1);
//     }
// }

// int main()
// {
//     ThreadNs::Thread t1(run,(void*)"thread 1 run");
//     ThreadNs::Thread t2(run,(void*)"thread 2 run");

//     t1.start();
//     t2.start();

//     t1.join();
//     t2.join();

//     return 0;
// }