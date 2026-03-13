#pragma once
#include <iostream>
#include <functional>
class Task
{
    using func_t=std::function<int(int,int)>;
public:
    Task()
    {}
    Task(int x,int y,func_t func)
    :_x(x)
    ,_y(y)
    ,_callback(func)
    {}

    int operator()()
    {
        int result=_callback(_x,_y);
        return result;
        //std::cout<<"result:"<<result<<std::endl;
    }

    // int print()
    // {

    // }

private:
    int _x;
    int _y;
    func_t _callback;
};