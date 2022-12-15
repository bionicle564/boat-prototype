#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#include <vector>

#include "../UDPLibrary/Buffer.h"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"


#define BUFLEN 512	//Max length of buffer
#define PORT 8888	//The port on which to listen for incoming data

char buf[BUFLEN];
int slen, recv_len;

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

	//set up socket
	struct addrinfo* result = NULL, * ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	SOCKET ListenSocket = INVALID_SOCKET;


	//Create a socket
	if ((ListenSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
	}
	printf("Socket created.\n");

	sockaddr_in server;

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	//Bind
	if (bind(ListenSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	puts("Bind done");


	// Set our socket to be nonblocking
	ULONG NonBlock = 1;
	int r = ioctlsocket(ListenSocket, FIONBIO, &NonBlock);
	if (r == SOCKET_ERROR)
	{
		printf("Non-blocking failed with error code : %d", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	//everything should be set up now

	while(1)
	{
		memset(buf, '\0', BUFLEN);

		sockaddr_in client;

		int result = recv_len = recvfrom(ListenSocket, buf, BUFLEN, 0, (struct sockaddr*)&client, &slen);
		if (result == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				continue;
			}

			// For a TCP connection you would close this socket, and remove it from 
			// your list of connections. For UDP we will clear our buffer, and just
			// ignore this.
			memset(buf, '\0', 512);
			//exit(EXIT_FAILURE);
			return 1;
		}
		else
		{
			Buffer incoming(BUFLEN);
			incoming.LoadBuffer(buf, BUFLEN);

			std::string line = incoming.PayloadToString();
			int stop = 0;

		}
	}

	return 0;
}