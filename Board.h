#include <iostream>
#include <string>

using namespace std;

class Board
{
private:
    int num_of_players;

public:
    Board(int num_of_players);
    char **create_board(int num_of_players);
    char **set_empty(char **mat);
    void print_board(char **mat);
    int *find_player(char player, char **mat);
    bool valid_move(char **mat, char player, string move);
    char **move_up(char **mat, char player);
    char **move_down(char **mat, char player);
    char **move_left(char **mat, char player);
    char **move_right(char **mat, char player);
};