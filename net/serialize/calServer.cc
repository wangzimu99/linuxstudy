#include "calServer.hpp"
#include <memory>

using namespace std;
using namespace server;

void usage(string proc)
{
    cout<<"\nusahe:\n\t"<<proc<<"local_port"<<endl;

}

bool cal(const Request& req,Response& resp)
{
    resp.exitcode=OK;
    resp.result=OK;
    switch(req.op)
    {
        case '+':resp.result=req.x+req.y;break;
        case '-':resp.result=req.x-req.y;break;
        case '*':resp.result=req.x*req.y;break;
        case '/':{
            if(req.y==0) resp.exitcode=DIV_ZERO;
            else resp.result=req.x/req.y;
        }break;
        case '%':{
             if(req.y==0) resp.exitcode=MOD_ZERO;
             else resp.result=req.x%req.y;
        }break;
        default: 
        resp.exitcode=OP_ERROR;
        break;
    }
    return true;
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
    unique_ptr<CalServer> tsvr(new CalServer(port));
    tsvr->initServer();
    //daemonSelf();
    tsvr->start(cal);

    return 0;
}