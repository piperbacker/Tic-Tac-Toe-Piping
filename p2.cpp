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
int p2Choice;
int p1Choice;
string p2Mark = "O";
bool game;
string winner = "";
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

    if (winner == "O")
    {
        cout << "Winner winner chicken dinner!" << endl;
        game = false;
    }
    else if (winner == "X")
    {
        cout << "You lose :(" << endl;
        game = false;
    }
}

int main()
{
    game = true;
    cout << "Welcome to Tic Tac Toe Game!" << endl;

    int f1 = mkfifo(myfifo_1to2, 0666);
    int f2 = mkfifo(myfifo_2to1, 0666);
    //printf("@p2: f1 = %d  f2 = %d\n", f1, f2);

    char rd_data[MAX], wr_data[MAX];

    int fd_rd = open(myfifo_1to2, O_RDONLY);
    int fd_wr = open(myfifo_2to1, O_WRONLY);

    while (true)
    {
        drawBoard();
        read(fd_rd, rd_data, sizeof(rd_data));
        p1Choice = atoi(rd_data);
        p1Move();
        checkBoard();
        drawBoard();
        if (strcmp(rd_data, "Q") == 0)
            break;

        printf("Please choose a position [0-8]: ");
        fgets(wr_data, MAX, stdin);
        wr_data[strlen(wr_data) - 1] = '\0';
        write(fd_wr, wr_data, strlen(wr_data) + 1);
        p2Choice = atoi(wr_data);
        p2Move();
        checkBoard();
    }
    close(fd_rd);
    close(fd_wr);


    /*cout << "Would you like to play again? [y/n]" << endl;
    cin >> response;

    if (response == "y")
    {
        for(int i = 0; i < 9; i++) {
            board[i] = i;
        }
        main();
    }
    else
    {
        cout << "Goodbye!" << endl;
    }*/
}