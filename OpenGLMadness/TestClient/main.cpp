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
#define TCP_PORT 33605
#define TCP_PORT_S "33605"

int main(void)
{
	struct addrinfo* infoResult = NULL;			// Holds the address information of our server
	struct addrinfo* ptr = NULL;

	struct sockaddr_in si_other;
	struct sockaddr_in si_tcp;
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

	//Make second socket
	SOCKET tcpSock;
	//if ((tcpSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_ERROR)
	//{
	//	printf("socket() failed with error code : %d", WSAGetLastError());
	//	exit(EXIT_FAILURE);
	//}


	// Set our socket to be nonblocking
	ULONG NonBlock = 1;
	int result = ioctlsocket(s, FIONBIO, &NonBlock);
	 //result = ioctlsocket(tcpSock, FIONBIO, &NonBlock);
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

	//setup address structure for tcp
	memset((char*)&si_tcp, 0, sizeof(si_tcp));
	si_tcp.sin_family = AF_INET;
	si_tcp.sin_port = htons(TCP_PORT);
	//si_other.sin_addr.S_un.S_addr = inet_pton(2, SERVER, si_other.sin_addr);
	IN_ADDR* binAddr2 = new IN_ADDR();
	inet_pton(2, SERVER, binAddr2);
	si_tcp.sin_addr = *binAddr2;

	//set up socket
	struct addrinfo* result2 = NULL,  hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	int iResult = getaddrinfo(NULL, TCP_PORT_S, &hints, &result2);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	ptr = result2;

	for (ptr = result2; ptr != NULL; ptr = ptr->ai_next)
	{

		// Create a SOCKET for connecting to server
		tcpSock = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (tcpSock == INVALID_SOCKET)
		{
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(tcpSock, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			
			printf("socket failed with error: %ld\n", WSAGetLastError());
			closesocket(tcpSock);
			tcpSock = INVALID_SOCKET;
			continue;
		}
		break;
	}

	//iResult = connect(tcpSock, ptr->ai_addr, (int)ptr->ai_addrlen);
	//if (iResult == SOCKET_ERROR)
	//{
	//	printf("tcp connect failed with error code : %d", WSAGetLastError());
	//	closesocket(tcpSock);
	//	tcpSock = INVALID_SOCKET;
	//}



	std::string msg = "";
	std::cin >> message;

	Buffer outgoing(BUFLEN);

	//while (clientId == 0)
	{
		outgoing = ProtocolMethods::MakeProtocol(TRY_JOIN, message);
		char* payload = outgoing.PayloadToString();
		int amountSent = sendto(s, payload, outgoing.GetBufferSize(), 0, (struct sockaddr*)&si_other, slen);
		if (amountSent == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		delete[] payload;
		outgoing.ClearBuffer();
	}

	iResult = send(tcpSock, message, (int)strlen(message), 0);

	while (clientId == 0)
	{


		

		//if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*)&si_other, &slen) == SOCKET_ERROR)
		//{
		//	if (WSAGetLastError() == WSAEWOULDBLOCK)
		//	{
		//		continue;
		//	}
		//}
		//else
		//{
		//	Buffer incoming(BUFLEN);
		//	incoming.LoadBuffer(buf, BUFLEN);
		//	sProtocolData data = ProtocolMethods::ParseBuffer(incoming);
		//	int asdf = 9;
		//}

		//int tLen = sizeof(si_tcp);
		//if (recvfrom(tcpSock, buf, BUFLEN, 0, (struct sockaddr*)&si_tcp, &tLen) == SOCKET_ERROR)
		//{
		//	if (WSAGetLastError() == WSAEWOULDBLOCK)
		//	{
		//		continue;
		//	}
		//}
		//else
		//{
		//	Buffer incoming(BUFLEN);
		//	incoming.LoadBuffer(buf, BUFLEN);
		//	sProtocolData data = ProtocolMethods::ParseBuffer(incoming);
		//	int asdf = 9;
		//}

	}

}