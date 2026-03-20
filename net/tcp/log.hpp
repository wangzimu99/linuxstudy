#pragma once
#include <iostream>
#include <string>

#define DEBUG       0
#define NORMAL      1
#define WARRNING    2
#define ERROR       3
#define FATAL       4

void logMessage(int level,const std::string& message)
{
    std::cout<<"["<<level<<"]"<<message<<std::endl;

}