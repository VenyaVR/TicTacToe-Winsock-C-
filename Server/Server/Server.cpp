#include <WinSock2.h>
#include <iostream>

using namespace std;

#pragma comment (lib, "ws2_32.lib")
#pragma warning (disable: 4996)

char Map[9];

int GNumberUser;
int GTurn;

const short MaxUsers = 2;

struct USRE
{
	int Number;
	SOCKET Connection;

}User[2];

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

void ClientHandler(int index)
{
	while (true)
	{
		if (recv(User[index].Connection, Map, sizeof(Map), NULL))
		{
			GTurn++;

			for (int counter = 0; counter < GNumberUser; counter++)
			{
				if (counter == index)
					continue;

				send(User[counter].Connection, Map, sizeof(Map), NULL);
			}
		}
	}
}	

const char TypePlayer[2] = {'x', '0'};

int main()
{
	PrintInConsole("\t   Server\n\n", true);

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

	//system("cls");

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr(IP);
	addr.sin_port		 = htons(Port);
	addr.sin_family		 = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)& addr, sizeofaddr);

	listen(sListen, 2/*SOMAXCONN*/);

	SOCKET newConnection;

	for (int counterUser = 0; counterUser < MaxUsers; counterUser++)
	{
		while (!(newConnection = accept(sListen, (SOCKADDR*)& addr, &sizeofaddr)))
			PrintInConsole("Error: user failed connect to server !", false);
		
		PrintInConsole("A player has joined with a number: ", true);
		cout << ++GNumberUser << endl;

		User[counterUser].Connection = newConnection;
		User[counterUser].Number     = GNumberUser;

		switch (User[counterUser].Number)
		{
			case 1: send(User[counterUser].Connection, &TypePlayer[0], sizeof(TypePlayer[0]), NULL); break;
			case 2: send(User[counterUser].Connection, &TypePlayer[1], sizeof(TypePlayer[1]), NULL); break;
		}

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(counterUser), NULL, NULL);
	}

	system("pause");
}