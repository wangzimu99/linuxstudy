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
#include "Protocol.hpp"

namespace client
{
#define NUM 1024
    enum
    {
        USAGE_ERR = 1,
        SOCKET_ERR,
        BIND_ERR,
        LESSON_ERR
    };

    class CalClient
    {
    public:
        CalClient(const std::string &serverip, const uint16_t &serverport)
            : _sock(-1), _serverip(serverip), _serverport(serverport)
        {
        }
        void initClient()
        {
            // 1、创建套接字
            _sock = socket(AF_INET, SOCK_STREAM, 0);
            if (_sock < 0)
            {
                std::cerr << "socket create error" << std::endl;
                exit(2);
            }
            // 客户端不需要listen、bind(操作系统隐式的执行)、accept
        }

        void start()
        {
            struct sockaddr_in server;
            memset(&server, 0, sizeof(server));
            server.sin_family = AF_INET;
            server.sin_port = htons(_serverport);
            server.sin_addr.s_addr = inet_addr(_serverip.c_str());

            if (connect(_sock, (struct sockaddr *)&server, sizeof(server)) != 0)
            {
                std::cerr << "sock connect error" << std::endl;
            }
            else
            {
                std::string line;
                std::string inbuffer;
                while (true)
                {
                    std::cout << "mycal>>>";
                    std::getline(std::cin, line);

                    Request req = ParseLine(line);
                    std::string content;
                    req.serialize(&content);
                    std::string send_str = enLength(content);

                    send(_sock, send_str.c_str(), send_str.size(), 0);

                    std::string package, text;
                    if (!recvPackage(_sock, inbuffer, &package))
                        continue;
                    if (!deLength(package, &text))
                        continue;
                    Response resp;
                    resp.deserialize(text);
                    std::cout << "exitcode:" << resp.exitcode << std::endl;
                    std::cout << "result:" << resp.result << std::endl;
                }
            }
        }

        Request ParseLine(const std::string &line)
        {
            int status = 0;
            int cnt = line.size();
            int i = 0;
            std::string left, right;
            char op;
            while (i < cnt)
            {
                switch (status)
                {
                case 0:
                {
                    if (!isdigit(line[i]))
                    {
                        op = line[i];
                        status = 1;
                    }
                    else
                        left.push_back(line[i++]);
                }
                break;
                case 1:
                    i++;
                    status = 2;
                    break;
                case 2:
                    right.push_back(line[i++]);
                    break;
                }
            }
            return Request(std::stoi(left), std::stoi(right), op);
        }

        ~CalClient()
        {
            if (_sock > 0)
                close(_sock);
        }

    private:
        int _sock;
        std::string _serverip;
        uint16_t _serverport;
    };
}