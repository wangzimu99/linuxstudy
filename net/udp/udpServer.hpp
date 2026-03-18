#pragma once
#include <iostream>
#include <string>
#include <cerrno>
#include <strings.h>
#include <cstring>
#include <cstdlib>
#include <functional>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
namespace Server
{
	using namespace std;
	static const std::string defaultIp="0.0.0.0";
	static const int GNUM=1024;
	enum {USAGE_ERR=1,SOCKET_ERR,BIND_ERR};
	typedef function<void(string,uint16_t,string)> func_t;
	class udpServer
	{
	public:
		udpServer( const func_t& cb, uint16_t& port,const string& ip=defaultIp)
		:_port(port)
		,_ip(ip)
		,_sockfd(-1)
		,_callback(cb)
		{
		}
		void initServer()
		{
			//1.创建套接字
			_sockfd=socket(AF_INET,SOCK_DGRAM,0);
			if(_sockfd==-1)
			{
				cerr<<"socket error"<<errno<<":"<<strerror(errno)<<endl;
				exit(SOCKET_ERR);
			}
			cout<<"socket success"<<":"<<_sockfd<<endl;
			//2.绑定ip和port
			struct sockaddr_in local;
			bzero(&local,sizeof(local));
			local.sin_family=AF_INET;
			local.sin_port=htons(_port);
			//local.sin_addr.s_addr=inet_addr(_ip.c_str());
			local.sin_addr.s_addr=INADDR_ANY;//任意地址绑定，服务器的真实写法
			int n=bind(_sockfd,(struct sockaddr*)&local,sizeof(local));
			if(n==-1)
			{
				cerr<<"bind error"<<errno<<":"<<strerror(errno)<<endl;
				exit(BIND_ERR);
			}
			//UDP Server的准备工作完成

		}

		void start()
		{
			//服务器的本质就是一个死循环，常驻内存的进程
			char buffer[GNUM];
			for(;;)
			{
				//读取数据
				struct sockaddr_in peer;
				socklen_t len=sizeof(peer);
				ssize_t s=recvfrom(_sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&peer,&len);
				//1、数据是什么？2、谁发送的？
				if(s>0)
				{
					buffer[s]=0;
					string clientip=inet_ntoa(peer.sin_addr);
					uint16_t clientport=ntohs(peer.sin_port);
					string message=buffer;
					cout<<clientip<<"["<<clientport<<"]#"<<message<<endl;
					_callback(clientip,clientport,message);
				}
			}
		}
		
		~udpServer()
		{}
	private:
		uint16_t _port;
		string _ip;//实际上，一款网络服务器，不建议指明一个ip
		int _sockfd;
		func_t _callback;
	};
}