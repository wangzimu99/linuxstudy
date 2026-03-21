#include "tcpServer.hpp"
#include <memory>

using namespace std;
using namespace server;

void usage(string proc)
{
    cout<<"\nusahe:\n\t"<<proc<<"local_port"<<endl;

}
//tcp服务器启动和udp一模一样
int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        usage(argv[0]);
        exit(USAGE_ERR);
    }
    
    uint16_t port=atoi(argv[1]); 
    unique_ptr<TcpServer> tsvr(new TcpServer(port));
    tsvr->initServer();
    daemonSelf();
    tsvr->start();

    return 0;
}