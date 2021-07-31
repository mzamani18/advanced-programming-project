#include <iostream>
#include <Board.h>
#include <httplib.h>
#include <string>

using namespace std;

int main()
{
    httplib::Client cli("127.0.0.1", 8000);
    if (auto res = cli.Get("/start"))
    {
        string username = res->body;
        cout << res->body << endl;
        string tmp = username.substr(39, 1);
        int num_of_players = stoi(tmp);
        username = username.substr(12, 4);
        int turn = 0;
        if (res->status == 200)
        {
            //std::cout << res->body << std::endl;
            //username = username.substr(0, 3) + to_string(turn % num_of_players + 1);
            while (true)
            {
                string temp;
                cin >> temp;
                auto res = cli.Post("/pl" /* username.c_str()*/, temp, "text/plain");
                cout << res->body << endl;
            }
            //turn++;
            //start game();
        }
        else
        {
            cout << "Error" << endl;
        }
    }
}