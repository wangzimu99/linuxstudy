#include "udpClient.hpp"
#include <memory>
using namespace std;
using namespace Client;

static void Usage(string proc)
{
	cout<<"\nUsage:\n\t"<<proc<<"local_port\n\n";
}

// ./udpClient  server_ip server_port
int main(int argc,char* argv[])
{
    if(argc!=3)
    {
		Usage(argv[0]);
		exit(USAGE_ERR);
    }

    string serverip=argv[1];
	uint16_t port=atoi(argv[2]);

    unique_ptr<udpClient> ucli(new udpClient(serverip,port));

    ucli->initClient();
    ucli->run();

    return 0;
}
