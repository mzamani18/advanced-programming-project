#include <iostream>
#include <httplib.h>
#include <Board.h>

string valid_move_array[4] = {"up", "down", "left", "right"};
bool valid_move(string move)
{
    for (int i = 0; i < 4; i++)
    {
        if (move == valid_move_array[i])
            return true;
    }
    cout << "your input is not valid , EROOR!\n";
    return false;
}

int main()
{
    httplib::Client cli("127.0.0.1", 8080);
    string temp = "up";
    while (valid_move(temp))
    {
        cin >> temp;
        auto res = cli.Post("/p1", temp + "......" + "p1", "text/plain");

        if (res)
        {
            cout << res->status << std::endl;
            cout << res->body << std::endl;
        }
    }
    while (valid_move(temp))
    {
        cin >> temp;
        auto res = cli.Post("/p2", temp + ".........." + "p2", "text/plain");

        if (res)
        {
            cout << res->status << std::endl;
            cout << res->body << std::endl;
        }
    }
    while (valid_move(temp))
    {
        cin >> temp;
        auto res = cli.Post("/p3", temp + "......" + "p3", "text/plain");

        if (res)
        {
            cout << res->status << std::endl;
            cout << res->body << std::endl;
        }
    }
    while (valid_move(temp))
    {
        cin >> temp;
        auto res = cli.Post("/p4", temp + "......" + "p4", "text/plain");

        if (res)
        {
            cout << res->status << std::endl;
            cout << res->body << std::endl;
        }
    }
    //cout << cli.Get("/stop")->body << endl;
}