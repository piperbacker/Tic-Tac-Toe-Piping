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

void playerMove()
{
    cout << "Please choose a position [0-8]: ";
    cin >> p2Choice;

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
}

int main()
{
    //game = true;
    cout << "Welcome to Tic Tac Toe Game!" << endl;

    int f1 = mkfifo(myfifo_1to2, 0666);
	int f2 = mkfifo(myfifo_2to1, 0666);
	printf("@p2: f1 = %d  f2 = %d\n", f1, f2);

	char rd_data[MAX], wr_data[MAX];

    int fd_rd = open(myfifo_1to2, O_RDONLY);
	int	fd_wr = open(myfifo_2to1, O_WRONLY);

    while (true)
	{
		read(fd_rd, rd_data, sizeof(rd_data));
		printf("received: %s\n", rd_data);
		if (strcmp(rd_data, "Q") == 0)
			break;

		//printf("Enter a message: ");
        drawBoard();
        playerMove();
        checkBoard();
		fgets(wr_data, MAX, stdin);
		wr_data[strlen(wr_data) - 1] = '\0';
		write(fd_wr, wr_data, strlen(wr_data) + 1);
	}
	close(fd_rd);
	close(fd_wr);
	//printf("Q received from Prog1; Prog2 exits\n");

    /*while (game == true)
    {
        drawBoard();
        playerMove();
        checkBoard();
    }*/

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