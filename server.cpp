#include <iostream>
#include <string>
#include <httplib.h>
#include <Board.h>

using namespace std;

int main(void)
{
  httplib::Server svr;
  int num_of_players;
  cout << "Enter number of players";
  cin >> num_of_players;
  string ii;
  for (int i = 0; i < num_of_players; i++)
  {
    ii = to_string(i);

    svr.Post("/p" + ii, [&](const httplib::Request &req, httplib::Response &res)
             {
               cout << ii << endl;
               cout << req.body << endl;
             });
  }

  svr.Get("/hi", [](const httplib::Request &req, httplib::Response &res)
          {
            res.set_content("Hello World!", "text/plain");
            cout << req.content_length_ << endl;
          });
  /*svr.Post("/pl", [&](const httplib::Request &req, httplib::Response &res)
           { cout << req.body << endl; });*/

  svr.Get("/stop", [&svr](const httplib::Request &req, httplib::Response &res)
          {
            res.set_content("server stoped", "text/plain");
            svr.stop();
          });
  std::cout << "start server..." << std::endl;
  svr.listen("127.0.0.1", 8080);
}
