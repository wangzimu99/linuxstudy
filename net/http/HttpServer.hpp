#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <functional>
#include "Protocol.hpp"
namespace server
{
    enum
    {
        USAGE_ERR = 1,
        SOCKET_ERR,
        BIND_ERR,
        LESSON_ERR
    };
    static const uint16_t gport = 8080;
    static const int gbacklog = 5;

    using func_t=std::function<bool(HttpRequest&,HttpResponse&)>;
    class HttpServer
    {
    public:
        HttpServer(func_t func,const uint16_t &port = gport)
            : _func(func),_listensock(-1), _port(port)
        {
        }
        void initServer()
        {
            // 1、创建套接字
            _listensock = socket(AF_INET, SOCK_STREAM, 0);
            if (_listensock < 0)
            {
                exit(SOCKET_ERR);
            }
            // 2.bind绑定自己的网络信息
            struct sockaddr_in local;
            memset(&local, 0, sizeof(local));
            local.sin_family = AF_INET;
            local.sin_port = htons(_port);
            local.sin_addr.s_addr = htonl(INADDR_ANY);
            if (bind(_listensock, (struct sockaddr *)&local, sizeof(local)) < 0)
            {
                exit(BIND_ERR);
            }
            // 3、设置sockt为监听状态,tcp为面向链接
            if (listen(_listensock, gbacklog) < 0)
            {
                exit(LESSON_ERR);
            }
        }
        void HandlerHttp(int sock)
        {
            //1.读到完成的http请求
            //2.反序列化
            //3.httprequest,httpresponse,_func(req,resp)
            //4.resp序列化
            //5.send
            char buffer[4096];
            HttpRequest req;
            HttpResponse resp;
            ssize_t n=recv(sock,buffer,sizeof(buffer)-1,0);
            if(n>0)
            {
                buffer[n]=0;
                req.inbuffer=buffer;
                req.parse();
                _func(req,resp);
                send(sock,resp.outbuffer.c_str(),resp.outbuffer.size(),0);
            }

        }

        void start()
        {
            for (;;)
            {
                // 4、server获取新连接
                struct sockaddr_in peer;
                socklen_t len = sizeof(peer);
                int sock = accept(_listensock, (struct sockaddr *)&peer, &len); // 和客户端进行通信的套接字
                if (sock < 0)
                {
                    continue;
                }
                std::cout << "sock:" << sock << std::endl;
                pid_t id = fork();
                if (id == 0)
                {
                    close(_listensock);
                    if(fork()>0)exit(0);
                    HandlerHttp(sock);
                    close(sock);
                    exit(0);
                }
                close(sock);
               waitpid(id, nullptr, 0);
            }
        }

        ~HttpServer()
        {}
    private:
        int _listensock; // 不是用来通信的，是用来监听来链接到来，获取新连接的。
        uint16_t _port;
        func_t _func;
    };
}