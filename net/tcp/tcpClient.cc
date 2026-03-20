#include "tcpClient.hpp"
#include <memory>

using namespace std;
using namespace client;
void usage(string argv)
{
    cout<<"\nusage:\n\t"<<argv<<"serverip,serverport"<<endl;
}
int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t serverport=atoi(argv[2]);
    string serverip=argv[1];
    unique_ptr<TcpClient> tcli(new TcpClient(serverip,serverport));
    tcli->initClient();
    tcli->start();

    return 0;
}