#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_const.h"

#include <iostream>
#include <string>
using namespace std;

string board[9] = {".", "1", "2", "3", "4", "5", "6", "7", "8"};
string position;
int p2Choice;
int p1Choice;
bool game = true;
string winner;

void drawBoard()
{
    cout << "+-+-+-+" << endl;
    for (int i = 0; i < 9; i += 3)
    {
        cout << "|" + board[i] + "|" + board[i + 1] + "|" + board[i + 2] + "|" << endl;
    }
    cout << "+-+-+-+" << endl;
}

void move()
{
    board[p1Choice] = "O";
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

    if (winner == "O")
    {
        drawBoard();
        cout << "Winner winner chicken dinner!" << endl;
        game = false;
    }
    else if (winner == "X")
    {
        drawBoard();
        cout << "You lose :(" << endl;
        game = false;
    }

    // check for tie
    int l = 0;
    for (int i = 0; i < 9; i++)
    {
        if (board[i] == "X" || board[i] == "O")
        {
            l++;
        }
    }

    if (l == 9)
    {
        drawBoard();
        cout << "It's a tie :)" << endl;
        game = false;
    }
}

int main()
{
    cout << "Welcome to Tic Tac Toe Game!" << endl;

    int f1 = mkfifo(myfifo_1to2, 0666);
    int f2 = mkfifo(myfifo_2to1, 0666);

    char rd_data[MAX], wr_data[MAX];

    int fd_rd = open(myfifo_1to2, O_RDONLY);
    int fd_wr = open(myfifo_2to1, O_WRONLY);

    while (game)
    {
        drawBoard();
        read(fd_rd, rd_data, sizeof(rd_data));
        p1Choice = atoi(rd_data);
        board[p1Choice] = "X";
        checkBoard();
        if (game == false)
            break;

        drawBoard();

        do
        {
            printf("Please choose an open position [0-8]: ");
            fgets(wr_data, MAX, stdin);
            wr_data[strlen(wr_data) - 1] = '\0'; // '\n' is replaced by NULL ('\0')
            position = wr_data;
            p2Choice = atoi(wr_data);
        } while ((position != "0") && (position != "1") && (position != "2") && (position != "3") && (position != "4") &&
                     (position != "5") && (position != "6") && (position != "7") && (position != "8") ||
                 (board[p2Choice] == "X" || board[p2Choice] == "O"));

        write(fd_wr, wr_data, strlen(wr_data) + 1);
        board[p2Choice] = "O";
        checkBoard();
    }
    close(fd_rd);
    close(fd_wr);
}