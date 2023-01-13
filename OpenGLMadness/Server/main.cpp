#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#include <vector>

#include "cOverworld.h"
#include "world.pb.h"

#include "../UDPLibrary/Buffer.h"
#include "../UDPLibrary/ProtocolHelper.h"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "33600"
#define TCP_PORT "33605"

#define BUFLEN 512	//Max length of buffer
#define PORT 8888	//The port on which to listen for incoming data
#define TCP_PORT 33605

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
	SOCKET DataSocket = INVALID_SOCKET;

	//Create a socket
	if ((ListenSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
	}
	printf("Listen socket created.\n");

	//tcp socket
	if ((DataSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
	}
	printf("Data socket created.\n");

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

	
	//tcp socket setup
	sockaddr_in dataServer;

	//Prepare the sockaddr_in structure
	dataServer.sin_family = AF_INET;
	dataServer.sin_addr.s_addr = INADDR_ANY;
	dataServer.sin_port = htons(TCP_PORT);

	//Bind
	if (bind(DataSocket, (struct sockaddr*)&dataServer, sizeof(dataServer)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	puts("Bind done");


	// Set our socket to be nonblocking
	NonBlock = 1;
	r = ioctlsocket(DataSocket, FIONBIO, &NonBlock);
	if (r == SOCKET_ERROR)
	{
		printf("Non-blocking failed with error code : %d", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	//everything should be set up now


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
		memset(buf, '\0', BUFLEN);

		sockaddr_in client;

		int result = recv_len = recvfrom(ListenSocket, buf, BUFLEN, 0, (struct sockaddr*)&client, &slen);
		if (result == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				continue;
			}
			int error = WSAGetLastError();
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
			sProtocolData data = ProtocolMethods::ParseBuffer(incoming);
			
			if (data.type == ProtocolType::TRY_JOIN) // A client is trying to join
			{
				clients.push_back(client);

				Buffer outgoing(BUFLEN);
				outgoing = ProtocolMethods::MakeProtocol(ProtocolType::JOIN_SUCCESSFULY, "hi :)");

				char* payload = outgoing.PayloadToString();

				if (sendto(ListenSocket, payload, outgoing.GetBufferSize(), 0, (struct sockaddr*)&client, slen) == SOCKET_ERROR)
				{
					printf("sendto() failed with error code : %d", WSAGetLastError());
					exit(EXIT_FAILURE);
				}


				outgoing = ProtocolMethods::MakeProtocol(ProtocolType::HIT_BALL, help);
				
				payload = outgoing.PayloadToString();

				if (sendto(DataSocket, payload, outgoing.GetBufferSize(), 0, (struct sockaddr*)&client, slen) == SOCKET_ERROR)
				{
					printf("sendto() failed with error code : %d", WSAGetLastError());
					exit(EXIT_FAILURE);
				}

				delete[] payload;

			}

		}
	}

	return 0;
}