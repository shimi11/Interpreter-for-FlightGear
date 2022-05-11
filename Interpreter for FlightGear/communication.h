#pragma once

#include <iostream>
#include <winsock2.h>
#include <thread> 
#include <mutex>
#include <queue>

#include "data.h"

using namespace std;


#define MAXSOCKETS 3
#define SIZERECV 400
#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable : 4996)



class Client
{
public:
	
	static Client* getInstance(const char* serverIp = " ", int serverPort = 0);
	~Client();
	bool sendCommend(string comm);
private:

	Client(const char* sIp, int sPort);
	static Client* instance;

	const char* serverip;// "127.0.0.1";
	int myport;//5400
	int serverport;// 5402
	SOCKET Socket;
	bool createConnect();
};


class Server
{
//The server is singleton so that the server will not be 
//destroyed when the object that started it is destroyed.
public:
	~Server();
	static Server* getInstance(int myport, int rate);

	void startThread();
private:
	static Server* instance;
	Server(int myport, int rate);

	thread th0;
	bool openServer();
	int myPort;//5400
	int time_sleep;
	SOCKET listenSocket;
	SOCKET acceptSocket;
	bool createListening();
	bool myAccept();
	int recvData(char *baf,int size);

};