#include <WinSock2.h>
#include <iostream>
#include <conio.h>

using namespace std;

#pragma comment (lib, "ws2_32.lib")
#pragma warning (disable: 4996)

#define Enter 13

char Number[2];
char Command[256];

SOCKET Connection;

void PrintInConsole(const char* pStr, bool type)
{
	HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	int Color;

	switch (type)
	{
		case  true: Color = 10; break;
		case false: Color = 12; break;
	}

	SetConsoleTextAttribute(hConsoleHandle, Color);
	cout << pStr;
	SetConsoleTextAttribute(hConsoleHandle, 7);
}

namespace TicTacToe 
{
	char Map[9];
	char TP;

	enum TYPEPLAYER
	{
		cross,
		zero
	};

	TYPEPLAYER TypePlayer = cross;
	
	int Turn = 0;
	int Move;

	void INIT()
	{
		for (int nCounter = 0; nCounter < 9; nCounter++)
			Map[nCounter] = char(49 + nCounter);
	}

	void DrawMap()
	{
		PrintInConsole("\t      Client\n\n", true);

		cout << "\t  *Tic Tak Toe* \n" << endl;
		
		cout << "\t " << "    |" <<  "     |"  << endl;
		cout << "\t  " << Map[0] << "  |  " << Map[1] << "  |  " << Map[2] << endl;
		cout << "\t_____|_____|_____" << endl;
		cout << "\t " << "    |" << "     |" << endl;
		cout << "\t  " << Map[3] << "  |  " << Map[4] << "  |  " << Map[5] << endl;
		cout << "\t_____|_____|_____" << endl;
		cout << "\t " << "    |" << "     |" << endl;
		cout << "\t  " << Map[6] << "  |  " << Map[7] << "  |  " << Map[8] << endl;
		cout << "\t " << "    |" << "     |" << endl;
	}

	void Moving()
	{
		ReplaceCinMove:
		switch (TypePlayer)
		{
			case cross: cout << "\tYou - x" << endl; break;
			case zero:  cout << "\tYou - 0" << endl; break;
		}

		cout << "\tEnter the cell number: ";
		cin >> Move;
		
		if (Map[Move - 1] == 'x' || Map[Move - 1] == '0')
		{
			PrintInConsole("\tError: this cell is already taken !\n", false);
			goto ReplaceCinMove;
		}

		if (Move > 9)
		{
			PrintInConsole("\tError: there are 9 cells on the field !\n", false);
			goto ReplaceCinMove;
		}

		if (Turn % 2 != 0 || Turn == 0)
			if (TypePlayer == cross)
			{
				Map[Move - 1] = 'x';
			}

		if (Turn % 2 == 0)
			if (TypePlayer == zero)
			{
				Map[Move - 1] = '0';
			}

		system("cls");

	}
	
	int drawCounter;
	
	void GameOverWin()
	{
		drawCounter = 0;

		for(int counter = 0; counter < strlen(Map); counter++)
			if ((Map[counter] == 'x') || (Map[counter] == '0'))
				if (++drawCounter == 9)
				{
					cout << "\t=====> DRAW <=====" << endl;
					system("pause");
					exit(0);
				}
			

		if ((Map[0] == 'x' && Map[1] == 'x' && Map[2] == 'x') ||
			(Map[3] == 'x' && Map[4] == 'x' && Map[5] == 'x') ||
			(Map[6] == 'x' && Map[7] == 'x' && Map[8] == 'x') ||
			(Map[0] == 'x' && Map[3] == 'x' && Map[6] == 'x') ||
			(Map[1] == 'x' && Map[4] == 'x' && Map[7] == 'x') ||
			(Map[2] == 'x' && Map[5] == 'x' && Map[8] == 'x') ||
			(Map[0] == 'x' && Map[4] == 'x' && Map[8] == 'x') ||
			(Map[2] == 'x' && Map[4] == 'x' && Map[6] == 'x'))
			{
			if (TypePlayer == cross)
					PrintInConsole("\n\t==>Cross(x) win !\n \t==>You win !\n", true);
				else
					PrintInConsole("\n\t==>Cross(x) win !\n \t==>You lose !\n", false);

				system("pause"); 
				exit(0);
			}

		if ((Map[0] == '0' && Map[1] == '0' && Map[2] == '0') ||
			(Map[3] == '0' && Map[4] == '0' && Map[5] == '0') ||
			(Map[6] == '0' && Map[7] == '0' && Map[8] == '0') ||
			(Map[0] == '0' && Map[3] == '0' && Map[6] == '0') ||
			(Map[1] == '0' && Map[4] == '0' && Map[7] == '0') ||
			(Map[2] == '0' && Map[5] == '0' && Map[8] == '0') ||
			(Map[0] == '0' && Map[4] == '0' && Map[8] == '0') ||
			(Map[2] == '0' && Map[4] == '0' && Map[6] == '0'))
			{
				if (TypePlayer == zero)
					PrintInConsole("\n\t==>Zero(0) win !\n \t==>You win !\n", true);
				else
					PrintInConsole("\n\t==>Zero(0) win !\n \t==>You lose !\n", false);
				
				system("pause");
				exit(0);
			}
	}
}

void ClientHandler() 
{
	recv(Connection, &TicTacToe::TP, sizeof(TicTacToe::TP), NULL);

	if (TicTacToe::TP == 'x')
		TicTacToe::TypePlayer = TicTacToe::cross;
	if (TicTacToe::TP == '0')
		TicTacToe::TypePlayer = TicTacToe::zero;

	while (true)
		if (recv(Connection, TicTacToe::Map, sizeof(TicTacToe::Map), NULL))
			if (Command[0] == Enter)
			{	
				system("cls");

				TicTacToe::DrawMap();
				TicTacToe::GameOverWin();

				switch (TicTacToe::TypePlayer)
				{
					case TicTacToe::cross: cout << "\tYou - x" << endl; break;
					case TicTacToe::zero:  cout << "\tYou - 0" << endl; break;
				}

				cout << "\tEnter the cell number: ";
			}
}

int main()
{
	PrintInConsole("\t      Client\n\n", true);

	WSAData wsaData;
	WORD Version = MAKEWORD(2, 1);
	if (WSAStartup(Version, &wsaData))
	{
		PrintInConsole("Error: failed load library !", false);
		exit(1);
	}

	char IP[256];
	int Port;

	cout << "Enter the IP: ";
	cin >> IP;
	cout << "Enter the Port: ";
	cin >> Port;

	system("cls");

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr(IP);
	addr.sin_port	     = htons(Port);
	addr.sin_family		 = AF_INET;
	
	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	while (connect(Connection, (SOCKADDR*)& addr, sizeofaddr))
		PrintInConsole("Error: failed connect to server !\n", false);

	system("cls");
	PrintInConsole("Connect to server!\n", true);
	
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

	while (cout << "If you want to start the game press 'Enter'", Command[0] = getch(), Command[0] != Enter) system("cls");
	
	system("cls");
	TicTacToe::INIT();
	TicTacToe::DrawMap();
	
	while (true)
	{	
		TicTacToe::Moving();
		send(Connection, TicTacToe::Map, sizeof(TicTacToe::Map), NULL);
		
		TicTacToe::DrawMap();
		TicTacToe::GameOverWin();
	}
	
	system("pause");
}