#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <assert.h>
#include <arpa/inet.h>

namespace client
{
    #define NUM 1024
	enum {USAGE_ERR=1,SOCKET_ERR,BIND_ERR,LESSON_ERR};

    class TcpClient
    {
    public:
        TcpClient(const std::string& serverip,const uint16_t& serverport)
        :_sock(-1)
        ,_serverip(serverip)
        ,_serverport(serverport)
        {}
        void initClient()
        {
            //1、创建套接字
            _sock=socket(AF_INET,SOCK_STREAM,0);
            if(_sock<0)
            {
                std::cerr<<"socket create error"<<std::endl;
                exit(2);
            }
            //客户端不需要listen、bind(操作系统隐式的执行)、accept


        }

        void start()
        {
            struct sockaddr_in server;
            memset(&server,0,sizeof(server));
            server.sin_family=AF_INET;
            server.sin_port=htons(_serverport);
            server.sin_addr.s_addr=inet_addr(_serverip.c_str());

            if(connect(_sock,(struct sockaddr*)&server,sizeof(server))!=0)
            {
                std::cerr<<"sock connect error"<<std::endl;
            }
            else
            {
                std::string message;
                while(1)
                {
                    std::cout<<"Enter#";
                    std::getline(std::cin,message);
                    write(_sock,message.c_str(),message.size());

                    char buffer[NUM];
                    int n=read(_sock,buffer,sizeof(buffer)-1);
                    if(n>0)
                    {
                        buffer[n]=0;
                        std::cout<<"Server 回显#"<<buffer<<std::endl;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }

        ~TcpClient()
        {
            if(_sock>0)
            close(_sock);
        }
    private:
        int         _sock;
        std::string _serverip;
        uint16_t    _serverport;
    };
}