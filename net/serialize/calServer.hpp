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
#include "log.hpp"
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
    typedef std::function<bool(const Request &req, Response &resp)> func_t;

    void HandlerEnter(int sock, func_t func)
    {
        std::string inbuffer;
        while (true)
        {
            // 1读取
            // 1.1保证完整
            std::string req_text, req_str;
            // 1.2 req_text里是一个完整的请求
            if (!recvPackage(sock, inbuffer, &req_text))
                return;

            if (!deLength(req_text, &req_str))
                return;
            std::cout<<"带报头的请求：\n"<<req_text<<std::endl;
            // 2反序列化
            // 2.1得到一个结构化的请求对象
            Request req;
            if (!req.deserialize(req_str))
                return;
            std::cout<<"去掉报头的请求：\n"<<req_str<<std::endl;
            
            // 3计算处理
            // 3.1得到一个结构化的响应
            // 4对响应序列化
            Response resp;
            func(req, resp);
            // 4.1得到字符串
            std::string resp_str;
            resp.serialize(&resp_str);
            std::cout<<"计算完成，序列化响应："<<resp_str<<std::endl;
            // 发送相应
            std::string send_string = enLength(resp_str);
            send(sock, send_string.c_str(), send_string.size(), 0);
            std::cout<<"构建完成完整的响应：\n"<<send_string<<std::endl;

        }
    }

    class CalServer
    {
    public:
        CalServer(const uint16_t &port = gport)
            : _listensock(-1), _port(port)
        {
        }
        void initServer()
        {
            // 1、创建套接字
            _listensock = socket(AF_INET, SOCK_STREAM, 0);
            if (_listensock < 0)
            {
                logMessage(FATAL, "create socket error");
                exit(SOCKET_ERR);
            }
            logMessage(NORMAL, "create socket success");
            // 2.bind绑定自己的网络信息
            struct sockaddr_in local;
            memset(&local, 0, sizeof(local));
            local.sin_family = AF_INET;
            local.sin_port = htons(_port);
            local.sin_addr.s_addr = htonl(INADDR_ANY);
            if (bind(_listensock, (struct sockaddr *)&local, sizeof(local)) < 0)
            {
                logMessage(FATAL, "bind socket error");
                exit(BIND_ERR);
            }
            logMessage(NORMAL, "bind socket success");
            // 3、设置sockt为监听状态,tcp为面向链接
            if (listen(_listensock, gbacklog) < 0)
            {
                logMessage(FATAL, "listen socket error");
                exit(LESSON_ERR);
            }
            logMessage(NORMAL, "listen socket success");
        }

        void start(func_t func)
        {

            for (;;)
            {
                // 4、server获取新连接
                struct sockaddr_in peer;
                socklen_t len = sizeof(peer);
                int sock = accept(_listensock, (struct sockaddr *)&peer, &len); // 和客户端进行通信的套接字
                if (sock < 0)
                {
                    logMessage(ERROR, "accept error ,next");
                    continue;
                }
                logMessage(NORMAL, "accept a new link success,get new sock: %d", sock);
                std::cout << "sock:" << sock << std::endl;
                pid_t id = fork();
                if (id == 0)
                {
                    // 子进程
                    close(_listensock);
                    // if(fork()>0)exit(0);
                    // serviceIO(sock);
                    HandlerEnter(sock, func);
                    close(sock);
                    exit(0);
                }
                close(sock);
                pid_t ret = waitpid(id, nullptr, 0);
                if (ret > 0)
                {
                    logMessage(NORMAL, "wait child success ");
                }
            }
        }

        ~CalServer()
        {}
    private:
        int _listensock; // 不是用来通信的，是用来监听来链接到来，获取新连接的。
        uint16_t _port;
    };
}