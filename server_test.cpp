#include <iostream>
#include <string>
#include "./httplib.h"
#include <Board.h>

#define CPPHTTPLIB_OPENSSL_SUPPORT
using namespace std;

string playerIdGenerator(int numberOfConnected);
void startGame(bool isGameStart, int numberOfPlayers, string players[], httplib::Server &svr);
bool include_a_str(string str, string temp);
int main(void)
{
  httplib::Server svr;
  cout << "enter number of palyers" << endl;
  int numberOfPlayers;
  cin >> numberOfPlayers;
  int numberOfConnected = 0;
  bool isGameStart = false;
  string players[numberOfPlayers];

  svr.Post("/pl", [&](const httplib::Request &req, httplib::Response &res)
           { cout << req.body << endl; });

  std::cout << "start server..." << std::endl;
  svr.Get("/new_game", [&](const httplib::Request &req, httplib::Response &res)
          {
            if (numberOfPlayers <= numberOfConnected)
            {
              res.set_content("you can't join the game", "text/plain");
              return;
            }
            string id = playerIdGenerator(numberOfConnected);
            res.set_content(id, "text/plain");
            players[numberOfConnected] = id;
            numberOfConnected++;
            if (numberOfConnected == numberOfPlayers)
            {
              startGame(true, numberOfPlayers, players, svr);
            }
          });

  svr.listen("127.0.0.1", 8080);
  cout << "after" << endl;
}

string playerIdGenerator(int numberOfConnected)
{
  string pl = "/pl";
  string id = std::to_string(numberOfConnected + 1);
  return pl + id;
}

void startGame(bool isGameStart, int numberOfPlayers, string players[], httplib::Server &svr)
{
  if (isGameStart)
  {
    cout << "lets go.." << endl;
  }
  if (isGameStart)
  {
    Board game(numberOfPlayers);
    static int n = 11;
    static char **mat = new char *[n];
    for (int i = 0; i < n; i++)
      mat[i] = new char[n];
    static int turn = 0;
    static int flag = 0;
    while (isGameStart)
    {
      mat = game.create_board(numberOfPlayers);
      string turnCommand = players[turn % numberOfPlayers] + "_turn";
      string update = players[turn % numberOfPlayers] + "_turn_update";
      string play = players[turn % numberOfPlayers] + "_turn_update_play";
      svr.Get(turnCommand, [&](const httplib::Request &req, httplib::Response &res)
              { res.set_content(turnCommand, "text/plain"); });
      svr.Get(update, [&](const httplib::Request &req, httplib::Response &res)
              { res.set_content("in this section we should wright the board", "text/plain"); });
      cout << "Player " << turn % numberOfPlayers + 1 << "is your turn." << endl;
      svr.Post(play, [&](const httplib::Request &req, httplib::Response &res)
               {
                 string command = req.body;
                 string ii = to_string(turn % numberOfPlayers + 1);

                 if (game.determine_winner(mat, ii[0], req.body))
                 {
                   cout << "Winner is:" << ii[0];
                   cout << "Game is end" << endl;
                   flag = 1;
                   svr.stop();
                   return;
                 }
                 cout << players[turn % numberOfPlayers] << " command is: " << command << endl;
                 //--------------------
                 if (include_a_str(req.body, "wall"))
                 {
                   int row, col;
                   if (req.body == "wallup" || req.body == "walldown")
                   {
                     mat = game.wall(mat, ii[0], "up", row, col);
                     game.print_board(mat);
                   }
                   else if (req.body == "wallright" || req.body == "wallleft")
                   {
                     mat = game.wall(mat, ii[0], "right", row, col);
                     game.print_board(mat);
                   }
                   else
                   {
                     res.set_content("it is a not valid move", "txt/plain");
                   }
                 }
                 else
                 {
                   if (req.body == "up")
                   {
                     mat = game.move_up(mat, ii[0]);
                     game.print_board(mat);
                   }
                   else if (req.body == "down")
                   {
                     mat = game.move_down(mat, ii[0]);
                     game.print_board(mat);
                   }
                   else if (req.body == "right")
                   {
                     mat = game.move_right(mat, ii[0]);
                     game.print_board(mat);
                   }
                   else if (req.body == "left")
                   {
                     mat = game.move_left(mat, ii[0]);
                     game.print_board(mat);
                   }
                 }
                 //get command from client and use it to play
                 //play
                 turn++;
                 cout << turn << endl;
                 res.set_content("result of this player move", "text.plain");
               });
      if (flag == 1)
      {
        break;
      }
      svr.listen("127.0.0.1", 8080);
    }
  }
}
bool include_a_str(string str, string temp)
{
  if (str.length() < temp.length())
    return false;
  for (int i = 0; i < str.length(); i++)
  {
    if (str[i] == temp[0])
    {
      int k = 0;
      while (str[i + k] == temp[k])
      {
        k++;
        if (k == temp.length())
          return true;
      }
    }
  }
  return false;
}
