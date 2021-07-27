#include <iostream>
#include <httplib.h>

using namespace std;

int main(void)
{
  httplib::Server svr;
  svr.set_keep_alive_max_count(2);
  svr.Get("/game", [](const auto &, auto &res)
          { res.set_content("how many players we have?", "text/plain"); });

  std::cout << "start server..." << std::endl;
  svr.listen("127.0.0.1", 8080);
}