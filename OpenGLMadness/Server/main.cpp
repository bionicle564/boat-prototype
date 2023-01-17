#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#include <thread>

#include <vector>

#include "cOverworld.h"
#include "world.pb.h"

#include "../UDPLibrary/Buffer.h"
#include "../UDPLibrary/ProtocolHelper.h"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "33600"
#define TCP_PORT_S "33605"

#define BUFLEN 512	//Max length of buffer
#define PORT 8888	//The port on which to listen for incoming data
#define TCP_PORT 33605

char buf[BUFLEN];
int slen, recv_len;

void LaunchSearch();
void Search();

SOCKET ListenSocket = INVALID_SOCKET;
SOCKET DataSocket = INVALID_SOCKET;
SOCKET ClientSocket = INVALID_SOCKET;
std::thread* thread = NULL;

int main()
{
	WSADATA wsaData;

	std::vector<sockaddr_in> clients;
	int iResult;


	slen = sizeof(clients);

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	struct addrinfo* result = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(NULL, TCP_PORT_S, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}


	// Create a SOCKET for the server to listen for client connections.
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	std::cout << "found somthing\n";

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET)
	{
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}



	std::cout << "connected\n";
	//says I wouldn't need this anymore
	//closesocket(ListenSocket);

	cOverworld world;
	world.GenerateIslands(10);

	overworld::World buffWorld;

	for (int i = 0; i < 10; i++)
	{
		overworld::Island* isl = buffWorld.add_islands();
		isl->set_x(world.points[i].x);
		isl->set_y(world.points[i].y);
		isl->set_type(world.points[i].type);
	}
	std::string help = buffWorld.SerializeAsString();

	while(1)
	{

		iResult = recv(ClientSocket, buf, BUFLEN, 0);
		if (iResult > 0)
		{
			printf("Bytes received: %d\n", iResult);
		}
		
	}
	delete thread;
	return 0;
}

void LaunchSearch()
{
	thread = new std::thread(Search);

}

void Search()
{
	int error;

	//this needs to be on another thread
	error = listen(DataSocket, SOMAXCONN);
	if (error == SOCKET_ERROR)
	{
		printf("Listen failed with error: %ld\n", WSAGetLastError());
	}

	
	ClientSocket = accept(DataSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET)
	{
			
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
	}
		

	
	return;
}