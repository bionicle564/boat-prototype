/*
	Simple udp client
	SOURCE: https://www.binarytides.com/udp-socket-programming-in-winsock/
*/

#include <stdio.h>
#include <conio.h>
#include <string>
#include <iostream>
#include <winsock2.h>
#include <ip2string.h>
#include <ws2tcpip.h>

#include "../UDPLibrary/Buffer.h"
#include "../UDPLibrary/ProtocolHelper.h"

//#include "TestClass.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define SERVER "127.0.0.1"	//ip address of udp server
#define BUFLEN 512	//Max length of buffer
#define PORT 8888	//The port on which to listen for incoming data

int main(void)
{
	struct addrinfo* infoResult = NULL;			// Holds the address information of our server
	struct addrinfo* ptr = NULL;

	struct sockaddr_in si_other;
	int s, slen = sizeof(si_other);
	char buf[BUFLEN];
	char message[BUFLEN];
	WSADATA wsa;

	int clientId = 0;
	bool isThisTurn = false;
	bool inSession = false;

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//create socket
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	// Set our socket to be nonblocking
	ULONG NonBlock = 1;
	int result = ioctlsocket(s, FIONBIO, &NonBlock);
	if (result == SOCKET_ERROR)
	{
		printf("Non-blocking failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//setup address structure
	memset((char*)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	//si_other.sin_addr.S_un.S_addr = inet_pton(2, SERVER, si_other.sin_addr);
	IN_ADDR* binAddr = new IN_ADDR();
	inet_pton(2, SERVER, binAddr);
	si_other.sin_addr = *binAddr;

	std::string msg = "";
	std::cin >> message;

	Buffer outgoing(BUFLEN);

	while (clientId == 0)
	{
		outgoing = ProtocolMethods::MakeProtocol(TRY_JOIN, message);
		char* payload = outgoing.PayloadToString();

		if (sendto(s, payload, outgoing.GetBufferSize(), 0, (struct sockaddr*)&si_other, slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		delete[] payload;
		outgoing.ClearBuffer();
	}
}