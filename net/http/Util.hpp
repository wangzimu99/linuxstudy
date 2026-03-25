#pragma once
#include <iostream>
#include <fstream>
#include <string>
class Util
{
public:
    static std::string GetOneLine(std::string& buffer,const std::string& sep)
    {
        auto pos=buffer.find(sep);
        if(pos==std::string::npos) 
        {
            std::cout<<"没有找到\\r\\n"<<std::endl;
            return "";
        }
        std::string sub=buffer.substr(0,pos);
        buffer.erase(0,sub.size()+sep.size());
        return sub;
    }
    static bool readFile(const std::string resource,char* buffer,int size)
    {
        std::ifstream in(resource,std::ios::binary);
        if(!in.is_open()) return false;
        in.read(buffer,size);
        // std::string line;
        // while(std::getline(in,line))
        // {
        //     *out+=line;
        // }
        in.close();
        return true;
    }
};