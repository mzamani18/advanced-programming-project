#include <iostream>
#include <httplib.h>

using namespace std;

int main(void)
{
  httplib::Server svr;

  svr.Get("/hi", [](const auto &, auto &res)
          { res.set_content("how many players we have?", "text/plain"); });

  std::cout << "start server..." << std::endl;
  svr.listen("0.0.0.0", 8080);
}