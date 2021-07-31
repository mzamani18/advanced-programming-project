#include <iostream>
#include <string>
#include "./httplib.h"

#define CPPHTTPLIB_OPENSSL_SUPPORT
using namespace std;

void startGame(string username, httplib::Client &cli);
string play(httplib::Client &cli, string command);
string update(httplib::Client &cli, string username);

int main()
{
    httplib::Client cli("127.0.0.1", 8080);
    //first time requerst for getting username and join the game.
    auto requestToPlay = cli.Get("/new_game");
    cli.set_keep_alive(true);
    string username;
    if (requestToPlay)
    {
        username = requestToPlay->body;
        cout << username << endl;
        cout << "your id: " << username << endl;
        if (username.at(0) == '/')
        {
            cout << "true" << endl;
            startGame(username, cli);
        }
    }
    else
    {
        cout << "Server not response!" << endl;
        cout << requestToPlay->status;
        return 0;

        // send play command with username to play and get result of command
        string command; // set this command with your desired value
        //auto resultOfPlay = cli.Post(username.c_str(), command, "text/plain");

        //if you want to stop server from client side trigger this:
        //cli.Get("/stop")->body << endl;
    }
}

void startGame(string username, httplib::Client &cli)
{
    string isMyTurn = username + "_turn";
    cout << isMyTurn << endl;
    while (true)
    {
        //cli.set_keep_alive(true); // big thing!!!!! i am the best.
        auto myTurn = cli.Get(isMyTurn.c_str());
        if (myTurn->body == isMyTurn)
        {
            //update board
            string updatedBoard = update(cli, username + "_turn");
            //paly and get result
            string resultOfplay = play(cli, username + "_turn");
            resultOfplay = "true";
            if (resultOfplay == "true")
            {
                cout << "your move done!" << endl;
            }
            else if (resultOfplay == "false")
            {
                cout << "foul move" << endl;
            }
        }
        if (myTurn->body == "end")
        {
            cout << "Game is end" << endl;
            break;
        }
    }
}
string update(httplib::Client &cli, string username)
{
    string update = username + "_update";
    cout << update << endl;
    auto updatedBoard = cli.Get(update.c_str());
    return updatedBoard->body;
}

string play(httplib::Client &cli, string username)
{
    string play = username + "_update_play";
    cout << play << endl;
    cout << "Enter your move: ";
    string command;
    cin >> command;
    auto resultOfplay = cli.Post(play.c_str(), command, "text/plain");
    return resultOfplay->body;
}