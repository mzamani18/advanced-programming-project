#include <iostream>
#include <Board.h>
#include <httplib.h>
#include <string>
#include <vector>
#include <ctype.h>

#define CPPHTTPLIB_OPENSSL_SUPPORT

using namespace std;

bool include_a_str(string str, string temp);
string seprate_numbers(string str);
int determine_row_and_col(string str, string req);

int main(void)
{
  httplib::Server svr;
  int num_of_players;
  cout << "Enter number of players" << endl;
  cin >> num_of_players;
  if (num_of_players > 4 || num_of_players <= 1)
  {
    cout << "Number of players should be in range(2,4)\n";
    exit(1);
  }
  int num_of_connections = 0;
  string players[num_of_players];
  svr.Get("/start", [&](const httplib::Request &req, httplib::Response &res)
          {
            if (num_of_players <= num_of_connections)
            {
              res.set_content("you cant join this game! please wait to finish!", "text/plain");
              return;
            }
            else
            {
              players[num_of_connections] = "/pl" + to_string(num_of_connections + 1);
              num_of_connections++;
              string id = "/pl" + to_string(num_of_connections);
              string str_of_num_of_players = to_string(num_of_players);
              res.set_content("your id is: " + id + "\nnumber of players is: " +
                                  str_of_num_of_players + "\nplease wait to all clients connect to game...\n" +
                                  "move list:{up,down,right,left}\nfor puting wall tell me wall and row and column with up or down or left or right for example: *wall45up*",
                              "text_plain");
              if (num_of_players == num_of_connections)
              {
                //game section.
                Board game(num_of_players);
                int n = 11;
                static char **mat = new char *[n];
                for (int i = 0; i < n; i++)
                  mat[i] = new char[n];
                mat = game.create_board(num_of_players);
                ////////////////////////////////
                cout << "lets go...!" << endl;
                static int turn = 0;
                static int turn2 = 1;
                svr.Post(/*id.c_str()*/ "/pl", [&](const httplib::Request &req, httplib::Response &res)
                         {
                           string command = req.body;
                           string test = to_string(((turn2 - 1) % num_of_players) + 1);
                           char ii = test[0];
                           //cout << "the player: " << ii << "move is " << command << endl;
                           if (game.determine_winner(mat, ii, command))
                           {
                             cout << " the Winner is:" << ii << endl;
                             cout << "Game is end" << endl;
                             res.set_content("congragulation! you win!", "text/plain");
                             svr.stop();
                             return;
                           }
                           //--------------------
                           if (command == "next")
                           {
                             turn2++;
                             if (turn2 > num_of_players)
                             {
                               turn2 = 1;
                             }
                             //cout << "turn2: " << turn2 << endl;
                             char i = turn2 + 48;
                             string sss = "";
                             sss += i;
                             sss += "A";

                             //cout << "sss:" << sss << endl;
                             res.set_content(sss, "text/plain");
                           }
                           else if (command == "now")
                           {
                             char i = turn2 + 48;
                             string sss = "";
                             sss += i;
                             sss += "A";
                             //cout << "sss:" << sss << endl;
                             res.set_content(sss, "text/plain");
                           }
                           else if (command == "map")
                           {
                             string str = "";
                             for (int i = 0; i < 11; i++)
                             {
                               for (int j = 0; j < 11; j++)
                               {
                                 str += mat[i][j];
                                 str += " ";
                               }
                               str += "\n";
                               string str2 = str + "\nyour move done";
                               res.set_content(str2, "text.plain");
                             }
                           }
                           else if (include_a_str(command, "wall"))
                           {
                             string rowandcol = seprate_numbers(req.body);
                             cout << rowandcol << endl;
                             int row = determine_row_and_col(rowandcol, "row");
                             int col = determine_row_and_col(rowandcol, "col");
                             string temp;
                             if (row < 9 && col < 9)
                             {
                               temp = command.substr(6);
                             }
                             else if (row >= 9 && col >= 9)
                             {
                               temp = command.substr(8);
                             }
                             else
                             {
                               temp = command.substr(7);
                             }
                             cout << row << " " << col << " " << temp << endl;
                             if (temp == "up" || temp == "down")
                             {
                               game.wall(mat, ii, temp, row, col);
                               game.print_board(mat);
                               //turn++;
                             }
                             else if (temp == "right" || temp == "left")
                             {
                               game.wall(mat, ii, temp, row, col);
                               game.print_board(mat);
                               //turn++;
                             }
                             else
                             {
                               res.set_content("it is a not valid move", "text/plain");
                               ////turn++;
                             }
                             string str = "";
                             for (int i = 0; i < 11; i++)
                             {
                               for (int j = 0; j < 11; j++)
                               {
                                 str += mat[i][j];
                                 str += " ";
                               }
                               str += "\n";
                               string str2 = str + "\nyour move done";
                               res.set_content(str2, "text.plain");
                             }
                           }
                           else
                           {
                             if (command == "up")
                             {
                               game.move_up(mat, ii);
                               game.print_board(mat);
                               //turn++;
                             }
                             else if (command == "down")
                             {
                               game.move_down(mat, ii);
                               game.print_board(mat);
                               //turn++;
                             }
                             else if (command == "right")
                             {
                               game.move_right(mat, ii);
                               game.print_board(mat);
                               //turn++;
                             }
                             else if (command == "left")
                             {
                               game.move_left(mat, ii);
                               game.print_board(mat);
                               //turn++;
                             }
                             string str = "";
                             for (int i = 0; i < 11; i++)
                             {
                               for (int j = 0; j < 11; j++)
                               {
                                 str += mat[i][j];
                                 str += " ";
                               }
                               str += "\n";
                               string str2 = str + "\nyour move done";
                               res.set_content(str2, "text.plain");
                             }
                           }
                           /*string str = "";
                           for (int i = 0; i < 11; i++)
                           {
                             for (int j = 0; j < 11; j++)
                             {
                               str += mat[i][j];
                               str += " ";
                             }
                             str += "\n";
                           }*/

                           //turn = turn % num_of_players + 1;
                           //char turn2 = turn + 48;
                           /*cout << "turn:" << turn << endl;
                           cout << "turn2 " << turn2 << endl;*/
                           /*string str2 = str + "\nyour move done";
                           //cout << str;

                           res.set_content(str2, "text.plain");*/
                           //cout << turn << endl;
                         });
              }
            }

            //start the game;
          });

  /*svr.Post("/turn", [&](const httplib::Request &req, httplib::Response &res)
           {
             static int i = 1;
             char temp = req.body[0];
             if (temp == '1')
             {
               i++;
             }
             char tmp = i + 48;
             cout << "i: " << i << endl;
             string test = temp + " turn";
             res.set_content(test, "text_plain");
           });*/

  svr.listen("127.0.0.1", 8000);
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
string seprate_numbers(string str)
{
  string num = "";
  for (int i; i < str.length(); i++)
  {
    if (isdigit(str[i]))
    {
      num += str[i];
    }
  }
  return num;
}
int determine_row_and_col(string str, string req)
{
  if (str.length() == 2)
  {
    string row_c = str.substr(0, 1);
    string col_c = str.substr(1, 1);
    if (req == "row")
    {
      int row = stoi(row_c) - 1;
      return row;
    }
    else if (req == "col")
    {
      int col = stoi(col_c) - 1;
      return col;
    }
  }
  else if (str.length() == 3)
  {
    int k = 0;
    for (int i = 0; i < 3; i++)
    {
      if (str[i] == '0')
      {
        k = (int)str[i] - 47;
        break;
      }
    }
    cout << k << endl;
    if (k == 1)
    {
      string row_c = str.substr(0, 2);
      string col_c = str.substr(2, 1);
      if (req == "row")
      {
        int row = stoi(row_c) - 1;
        return row;
      }
      else if (req == "col")
      {
        int col = stoi(col_c) - 1;
        return col;
      }
    }
    else
    {
      string row_c = str.substr(0, 1);
      string col_c = str.substr(1, 2);
      if (req == "row")
      {
        int row = stoi(row_c) - 1;
        return row;
      }
      else if (req == "col")
      {
        int col = stoi(col_c) - 1;
        return col;
      }
    }
  }
  else if (str.length() == 4)
  {
    string row_c = str.substr(0, 2);
    string col_c = str.substr(2, 2);
    if (req == "row")
    {
      int row = stoi(row_c) - 1;
      return row;
    }
    else if (req == "col")
    {
      int col = stoi(col_c) - 1;
      return col;
    }
  }
  return 0;
}
