#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
using namespace std;

#include "my_const.h"

string board[9] = {".", "1", "2", "3", "4", "5", "6", "7", "8"};
int p1Choice;
int p2Choice;
string p1Mark = "X";
bool game;
string winner;
string response;

void drawBoard()
{
    cout << "+-+-+-+" << endl;
    for (int i = 0; i < 9; i += 3)
    {
        cout << "|" + board[i] + "|" + board[i + 1] + "|" + board[i + 2] + "|" << endl;
    }
    cout << "+-+-+-+" << endl;
}

void p1Move()
{

    if (board[p1Choice] != "X" && board[p1Choice] != "O")
    {
        if (p1Choice >= 0 && p1Choice <= 8)
        {
            board[p1Choice] = "X";
        }
        else
        {
            cout << "Please pick a value between 0-8" << endl;
        }
    }
    else
    {
        cout << "This move has already been played, please pick a new move" << endl;
    }
}

void p2Move()
{

    if (board[p2Choice] != "X" && board[p2Choice] != "O")
    {
        if (p2Choice >= 0 && p2Choice <= 8)
        {
            board[p2Choice] = "O";
        }
        else
        {
            cout << "Please pick a value between 0-8" << endl;
        }
    }
    else
    {
        cout << "This move has already been played, please pick a new move" << endl;
    }
}

void checkBoard()
{
    // check rows
    for (int i = 0; i < 9; i += 3)
    {
        if (board[i] == board[i + 1] && board[i + 1] == board[i + 2])
        {
            winner = board[i];
        }
    }

    // check columns
    for (int i = 0; i < 9; i++)
    {
        if (board[i] == board[i + 3] && board[i + 3] == board[i + 6])
        {
            winner = board[i];
        }
    }

    // check diagonals
    int i = 0;
    if (board[i] == board[i + 4] && board[i + 4] == board[i + 8])
    {
        winner = board[i];
    }

    int k = 2;
    if (board[k] == board[k + 2] && board[k + 2] == board[k + 4])
    {
        winner = board[k];
    }

    if (winner == "X")
    {
        cout << "Winner winner chicken dinner!" << endl;
        game = false;
    }
    else if (winner == "O")
    {
        cout << "You lose :(" << endl;
        game = false;
    }
}

int main()
{
    cout << "Welcome to Tic Tac Toe Game!" << endl;

    int f1 = mkfifo(myfifo_1to2, 0666);
    int f2 = mkfifo(myfifo_2to1, 0666);

    char rd_data[MAX], wr_data[MAX];

    int fd_wr = open(myfifo_1to2, O_WRONLY);
    int fd_rd = open(myfifo_2to1, O_RDONLY);

    while (true)
    {
        drawBoard();
        printf("Please choose a position [0-8]: ");
        fgets(wr_data, MAX, stdin);
        wr_data[strlen(wr_data) - 1] = '\0'; // '\n' is replaced by NULL ('\0')
        write(fd_wr, wr_data, strlen(wr_data) + 1);
        p1Choice = atoi(wr_data);
        p1Move();
        checkBoard();
        if (strcmp(wr_data, "Q") == 0)
            break;

        read(fd_rd, rd_data, sizeof(rd_data));
        p2Choice = atoi(rd_data);
        p2Move();
        checkBoard();
        drawBoard();
    }
    close(fd_wr);
    close(fd_rd);
    unlink(myfifo_1to2);
    unlink(myfifo_2to1);

    /*cout << "Would you like to play again? [y/n]" << endl;
    cin >> response;

    if (response == "y")
    {
        main();
    }*/
}