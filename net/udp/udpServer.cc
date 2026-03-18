#include "udpServer.hpp"
#include <memory>
using namespace std;
using namespace Server;

static void Usage(string proc)
{
	cout<<"Usage:\n\t"<<proc<<"local_port\n\n";
}

void handlmessage(string clientip,uint16_t clientport,string message)
{

}

//.udpServer ip port
int main(int argc,char* argv[])
{
	if(argc!=2)
	{
		Usage(argv[0]);
		exit(USAGE_ERR);
	}
	uint16_t port=atoi(argv[1]);

	std::unique_ptr<udpServer> usvr(new udpServer(handlmessage,port));
	usvr->initServer();
	usvr->start();

	return 0;
}
