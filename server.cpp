#include <iostream>
#include <Board.h>
#include <httplib.h>
#include <string>
#include <vector>

#define CPPHTTPLIB_OPENSSL_SUPPORT

using namespace std;

bool include_a_str(string str, string temp);

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
                                        "move list:{up,down,right,left}\nfor puting wall tell me wall and row and column with up or down or left or right for example: wall45up",
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
                        //int temp = turn % num_of_players;

                        //id = "/pl" + to_string(turn % num_of_players + 1);
                        svr.Post(/*id.c_str()*/ "/pl", [&](const httplib::Request &req, httplib::Response &res)
                                 {
                                     string command = req.body;
                                     string test = to_string(turn % num_of_players + 1);
                                     char ii = test[0];
                                     cout << "the player: " << ii << "move is " << command << endl;
                                     if (game.determine_winner(mat, ii, command))
                                     {
                                         cout << " the Winner is:" << ii;
                                         cout << "Game is end" << endl;
                                         res.set_content("congragulation! you win!", "txt/plain");
                                         svr.stop();
                                         return;
                                     }
                                     //--------------------
                                     if (include_a_str(command, "wall"))
                                     {
                                         string row_c = command.substr(4, 1);
                                         string col_c = command.substr(5, 1);
                                         int row = stoi(row_c) - 1;
                                         int col = stoi(col_c) - 1;
                                         cout << row << " " << col << endl;
                                         string temp = command.substr(6);
                                         if (temp == "up" || temp == "down")
                                         {
                                             game.wall(mat, ii, temp, row, col);
                                             game.print_board(mat);
                                         }
                                         else if (temp == "right" || temp == "left")
                                         {
                                             game.wall(mat, ii, temp, row, col);
                                             game.print_board(mat);
                                         }
                                         else
                                         {
                                             res.set_content("it is a not valid move", "txt/plain");
                                         }
                                     }
                                     else
                                     {
                                         if (command == "up")
                                         {
                                             game.move_up(mat, ii);
                                             game.print_board(mat);
                                         }
                                         else if (command == "down")
                                         {
                                             game.move_down(mat, ii);
                                             game.print_board(mat);
                                         }
                                         else if (command == "right")
                                         {
                                             game.move_right(mat, ii);
                                             game.print_board(mat);
                                         }
                                         else if (command == "left")
                                         {
                                             game.move_left(mat, ii);
                                             game.print_board(mat);
                                         }
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
                                     }
                                     res.set_content(str + "\nyour move done", "text.plain");

                                     turn++;
                                     cout << turn << endl;
                                 });
                    }
                }
                //start the game;
            });

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