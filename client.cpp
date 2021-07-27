#include <iostream>
#include <httplib.h>

int main()
{
    httplib::Client cli("http://cpp-httplib-server.yhirose.repl.co");

    auto res = cli.Get("/game");

    if (res)
    {
        std::cout << res->status << std::endl;
        std::cout << res->body << std::endl;
    }
}