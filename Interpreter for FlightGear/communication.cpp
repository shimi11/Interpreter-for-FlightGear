#include "communication.h"



Server* Server::instance = 0;

Server* Server::getInstance(int myport, int rate)
{
	if (instance == 0)
	{
		instance = new Server( myport,  rate);
	}

	return instance;
}

Server::Server(int myport, int rate)
{
	myPort = myport;
	time_sleep = 1000 / rate;
	acceptSocket = 0;
	listenSocket = 0;
}
void Server::startThread()
{
	th0 = thread(&Server::openServer, this);
}

Server::~Server()
{
	if(th0.joinable())
		th0.join();
}

bool Server::openServer()
{
	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;

	fd_set waitRecv/*, waitSend*/, exceptfds;
	if (!createListening())
		return 0;
	while (1) {
		FD_ZERO(&waitRecv);
		//FD_ZERO(&waitSend);
		FD_ZERO(&exceptfds);

		SOCKET theSocket = listenSocket;
		if (acceptSocket)
			theSocket = acceptSocket;

		FD_SET(theSocket, &waitRecv);
		//FD_SET(theSocket, &waitSend);
		FD_SET(theSocket, &exceptfds);

		int result = select(0, &waitRecv, 0 /* &waitSend*/, &exceptfds, &timeout);
		if (result == SOCKET_ERROR)// ERROR 
		{
			cout << "server error ";
			cout << "the server crashed";
			return 0;
		}
		if (result == 0) // tine out
		{
			cout << "Server: No login / received information\n";
			Sleep(time_sleep);
			continue;
		}

		if (FD_ISSET(theSocket, &exceptfds))
		{
			cout << "socket error\n";
			if (theSocket == listenSocket)
			{
				cout << "the server crashed";
				return 0;
			}
			acceptSocket = 0;

			continue;
		}
		if (FD_ISSET(theSocket, &waitRecv))
		{
			if (theSocket == listenSocket)
			{
				if (myAccept())
				{
					cout << "Server: aircraft (client) connected to (server)control  " << endl;
				}
				continue;
			}

			char buf[SIZERECV];
			ZeroMemory(buf, SIZERECV);

			int bytesIn = recvData(buf, SIZERECV - 1);
			if (bytesIn > 0)
			{
				buf[bytesIn] = '\0';
				Data* Data = Data::getInstance();
				Data->updateBaf(buf, bytesIn);
			}
		}
		//Sleep(time_sleep);
	}
}

bool Server::createListening()

{
	WSAData wsaData;

	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "Server: Error at WSAStartup()\n";
		return false;
	}
	SOCKET lSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == lSocket)
	{
		cout << "Server: Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return false;
	}

	sockaddr_in serverService;
	serverService.sin_family = AF_INET;
	serverService.sin_addr.s_addr = INADDR_ANY;
	serverService.sin_port = htons(myPort);

	if (SOCKET_ERROR == bind(lSocket, (SOCKADDR*)&serverService, sizeof(serverService)))
	{
		cout << "Server: Error at bind(): " << WSAGetLastError() << endl;
		closesocket(lSocket);
		WSACleanup();
		return false;
	}

	if (SOCKET_ERROR == listen(lSocket, 5))
	{
		cout << "Server: Error at listen(): " << WSAGetLastError() << endl;
		closesocket(lSocket);
		WSACleanup();
		return false;
	}

	cout << "the server is turned on" << endl;
	listenSocket = lSocket;

	return true;
}

bool Server::myAccept()
{
	struct sockaddr_in from;		// Address of sending partner
	int fromLen = sizeof(from);

	SOCKET aSocket = accept(listenSocket, (struct sockaddr*)&from, &fromLen);
	if (INVALID_SOCKET == aSocket)
	{
		cout << "Server: Error at accept(): " << WSAGetLastError() << endl;
		return false;
	}

	// Set the socket to be in non-blocking mode.
	unsigned long flag = 1;
	if (ioctlsocket(aSocket, FIONBIO, &flag) != 0)
	{
		cout << "Server: Error at ioctlsocket(): " << WSAGetLastError() << endl;
	}

	acceptSocket = aSocket;
	return true;
}

int Server::recvData(char* buf, int size)
{
	int bytesIn = recv(acceptSocket, buf, size - 1, 0);
	
	return bytesIn;
}




Client* Client::instance = 0;
Client* Client::getInstance(const char* serverIp, int serverPort )
{
	if (instance == 0)
	{
		instance = new Client(serverIp, serverPort);
	}

	return instance;
}

Client::Client(const char* sIp, int sPort)
{
	Socket = 0;
	serverip = sIp;
	serverport = sPort;
	createConnect();

}
Client::~Client()
{
	WSACleanup();
}

bool Client::sendCommend(string comm)
{
	if (!Socket)
	{
		if (!createConnect())
		{
			return false;
		}
	}
	comm.append("\r\n");
	int r = send(Socket, comm.c_str(), comm.size(), 0);
	cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
	cout << "send " << r <<"/" << comm.size() << " byts" << ",\n";
	cout << " the message is " << comm << endl;
	cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;

	return (r > 0) ? true : false;
}

bool Client::createConnect()
{
	WSAData wsaData;

	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "client: Error at WSAStartup()\n";
		return false;
	}
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET)
	{
		cout << "client: Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return false;
	}

	sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_address.sin_port = htons(serverport);
	memset(server_address.sin_zero, 0, sizeof(server_address.sin_zero));

	//non-blocking mode.
	/*unsigned long flag = 1;
	if (ioctlsocket(ConnectSocket, FIONBIO, &flag) != 0)
	{
		cout << "Client: Error at ioctlsocket(): " << WSAGetLastError() << endl;
	}*/
	if (connect(ConnectSocket, (SOCKADDR*)&server_address, sizeof(server_address)) == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		switch (error)
		{
		case  WSAEWOULDBLOCK://is conected in non-blocking mode. 
			Socket = ConnectSocket;
			break;
		case WSAEISCONN://is conected
			Socket = ConnectSocket;
			cout << "Client: control (client) connected to (server)aircraft " << endl;
			break;
		default:
			closesocket(ConnectSocket);
			Socket = 0;
			WSACleanup();
			cout << "Client: Error at connect(): " << WSAGetLastError() << endl;
			return false;
		}
	}
	else
	{
		Socket = ConnectSocket;
		cout << "Client: control (client) connected to (server)aircraft " << endl;
	}
	return true;
}

