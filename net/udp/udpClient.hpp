#pragma once
#include <iostream>
#include <string>
#include <cerrno>
#include <strings.h>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

namespace Client
{
	enum {USAGE_ERR=1,SOCKET_ERR,BIND_ERR};

    using namespace std;
	class udpClient
	{
	public:
		udpClient(const string& serverip,const uint16_t& serverport)
        :_sockfd(-1)
        ,_serverip(serverip)
        ,_serverport(serverport)
        ,_quit(false)
		{
		}
        void initClient()
        {
            _sockfd=socket(AF_INET,SOCK_DGRAM,0);
           	if(_sockfd==-1)
			{
				cerr<<"socket error"<<errno<<":"<<strerror(errno)<<endl;
				exit(SOCKET_ERR);
			}
				cout<<"socket success"<<":"<<_sockfd<<endl;

            //2.client必须要bind，但是不需要程序员显式地Bind,由os自动形成端口进行bind！


        }
        void run()
        {
            struct sockaddr_in server;
            string message;
            memset(&server,0,sizeof(server));
            server.sin_addr.s_addr=inet_addr(_serverip.c_str());
            server.sin_port=htons(_serverport);
            while(!_quit)
            {
                cout<<"请输入#";
                cin>>message;
                sendto(_sockfd,message.c_str(),message.size(),0,(struct sockaddr*)&server,sizeof(server));
            }
        }
		
		~udpClient()
		{}
    private:
        int      _sockfd;
        uint16_t _ip;
        uint16_t _serverport;
        string   _serverip;
        bool     _quit;

	};
}