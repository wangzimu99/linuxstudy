#pragma once
#include <iostream>
#include <string>
#include  <cstdarg>

#define DEBUG       0
#define NORMAL      1
#define WARRNING    2
#define ERROR       3
#define FATAL       4

void logMessage(int level,const char* format,...)
{
    //std::cout<<"["<<level<<"]"<<message<<std::endl;
    // va_list start;
    // while(*p)
    // va_start(start);
    // va_arg(start,float);
    // va_end(start);
    #define NUM 1024

    char logprefix[NUM];
    snprintf(logprefix,sizeof(logprefix),"[%s][%ld][%d]");
    char logcontent[NUM];
    va_list arg;
    va_start(arg,format);
    vsnprintf(logcontent,sizeof(logcontent),format,arg);
    std::cout<<logprefix<<logcontent<<std::endl;
}