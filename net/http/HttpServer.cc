#include "HttpServer.hpp"
#include "Util.hpp"
#include <memory>
using namespace server;
using namespace std;
#define NUM 1024*1024*10
void Usage(std::string proc)
{
    cerr << "Usage:\n\t" << proc << "port\r\n\r\n";
}
    std::string suffixToDesc(const std::string suffix)
    {
         std::string ct="Content-Type: ";
         if(suffix==".html") ct+="text/html";
         else if(suffix==".jpg") ct+="application/x-jpg";
         ct+="\r\n";
         return ct;
    }
//1.服务器和网页分离，html
//2.url->/:web根目录
//3.要正确地给客户端返回资源类型，首先要自己知道！所有的资源都有后缀
bool Get(HttpRequest &req, HttpResponse &resp)
{
    cout << "----------------------------http stsrt-------------------------------" << endl;
    cout << req.inbuffer << endl;
    std::cout << "method: " << req.method << std::endl;
    std::cout << "url: " << req.url << std::endl;
    std::cout << "httpversion: " << req.httpversion << std::endl;
    std::cout << "path: " << req.path << std::endl;
    std::cout << "suffix: " << req.suffix << std::endl;
    std::cout << "siz: " << req.size << std::endl;
    cout << "----------------------------http end-------------------------------" << endl;
    std::string respline="HTTP/1.1 200 OK\r\n";
    std::string respheader=suffixToDesc(req.suffix);//"Content-Type: text/html\r\n";
    if(req.size>0)
    {
        respheader+="Content-Length: ";
        respheader+=std::to_string(req.size);
        respheader+="\r\n";
    }
    //respheader+="Set-Cookie: 1234567abcdefg\r\n";

    std::string respblank="\r\n";

    std::string body;
    body.resize(req.size+1);
    if(!Util::readFile(req.path,(char*)body.c_str(),req.size))
    {
       Util::readFile(html_404,(char*)body.c_str(),req.size); //一定会成功
    }
    resp.outbuffer+=respline;
    resp.outbuffer+=respheader;
    resp.outbuffer+=respblank;
    resp.outbuffer+=body;

    return true;
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(0);
    }
    uint16_t port = atoi(argv[1]);
    unique_ptr<HttpServer> httpsvr(new HttpServer(Get, port));
    httpsvr->initServer();
    httpsvr->start();
    return 0;
}