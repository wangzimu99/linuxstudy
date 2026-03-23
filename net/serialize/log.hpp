#pragma once
#include <iostream>
#include <string>
#include  <cstdarg>
#include <unistd.h>
#include <ctime>

#define DEBUG       0
#define NORMAL      1
#define WARNING    2
#define ERROR       3
#define FATAL       4


const char* to_levelstr(int level)
{
    switch(level)
    {
        case DEBUG  :    return "DEBUG";
        case NORMAL :    return "NORMAL";
        case WARNING:    return "WARING";
        case ERROR  :    return "ERROR";
        case FATAL  :    return "FATAL";
        default     :    return nullptr;
    }
}
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
    snprintf(logprefix,sizeof(logprefix),"[%s][%ld][pid: %d]",to_levelstr(level),(long int)time(nullptr),getpid());
    
    char logcontent[NUM];
    va_list arg;
    va_start(arg,format);
    vsnprintf(logcontent,sizeof(logcontent),format,arg);
    std::cout<<logprefix<<logcontent<<std::endl;
}