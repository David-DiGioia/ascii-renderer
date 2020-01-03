#include "Network.h"
#include <iostream>
#include <cstdlib>
#include <winsock2.h>
#include <WS2tcpip.h>
#include "Constants.h"
#include "Render.h"

#pragma comment(lib, "ws2_32.lib")

constexpr int MAP_BUFFER_SIZE{ 8 };

void startNetwork()
{
	// Startup Winsock
	WSADATA data;
	if (WSAStartup(MAKEWORD(2, 0), &data) != 0) {
		printf("Can't start Winsock!\n");
		__debugbreak();
		exit(1);
	}
}

void endNetwork()
{
	WSACleanup();
}

void receiveUDP(int port, std::queue<PlayerInput>& inputs, std::mutex& mtx)
{
	// Bind socket to ip address and port
	SOCKET server{ socket(AF_INET, SOCK_DGRAM, 0) };

	SOCKADDR_IN serverAddr;
	serverAddr.sin_addr.S_un.S_addr = ADDR_ANY;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);

	if (bind(server, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		printf("Can't bind socket!\n");
		__debugbreak();
		exit(2);
	}

	SOCKADDR_IN clientAddr;
	int clientLength{ sizeof(clientAddr) };
	ZeroMemory(&clientAddr, clientLength);

	// Message from client will be stored here
	char buf[MAP_BUFFER_SIZE]{};

	while (true)
	{
		ZeroMemory(buf, MAP_BUFFER_SIZE);

		// Wait for message
		int bytesIn{ recvfrom(server, buf, sizeof(buf), 0, (sockaddr*)&clientAddr, &clientLength) }; // recvfrom is like recv but for UDP

		// client has sent their input
		if (bytesIn == 4) {
			std::lock_guard<std::mutex> locker{ mtx };
			inputs.emplace(buf[0], buf[1], buf[2], buf[3]);
		}
	}

	closesocket(server);
}

void receiveTCP(int port, std::vector<Player>& players, std::vector<unsigned>& addresses, std::mutex& mtxPlayers, std::mutex& mtxIPs)
{
	SOCKET server;
	SOCKADDR_IN serverAddr, clientAddr;

	server = socket(AF_INET, SOCK_STREAM, 0);

	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);

	bind(server, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	listen(server, 0);

	char buffer[MAP_BUFFER_SIZE];
	int clientAddrSize = sizeof(clientAddr);

	while (true)
	{
		SOCKET client;
		if ((client = accept(server, (SOCKADDR*)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)
		{
			memset(buffer, 0, sizeof(buffer));
			recv(client, buffer, sizeof(buffer), 0);
			char playerId{ -1 };

			// client is requesting to join server
			if (buffer[0] == 'j') {
				{
					std::lock_guard<std::mutex> locker{ mtxPlayers };
					for (auto i{ 1 }; i < players.size(); ++i)
						if (!players[i].alive) {
							players[i] = Player{ buffer[1], (char)i };
							playerId = (char)i;
							break;
						}
					if (playerId == -1) {
						playerId = (char)(players.size());
						players.emplace_back(buffer[1], playerId); // buffer[1] is character chosen by player
					}
				}
				{
					std::lock_guard<std::mutex> locker{ mtxIPs };
					bool foundDeletedAddress{ false };
					for (auto& ip : addresses)
						if (ip == 0) {
							ip = clientAddr.sin_addr.S_un.S_addr;
							foundDeletedAddress = true;
						}
					if (!foundDeletedAddress) addresses.push_back(clientAddr.sin_addr.S_un.S_addr);
				}
			}

			// return player index
			char newBuffer[SMALL_BUFFER]{ playerId };
			send(client, newBuffer, sizeof(newBuffer), 0);
			closesocket(client);
		}
	}

}

void broadcastMap(unsigned address, int port, const Buffer& buf)
{
	// client plays the usual role of the server here since it's receiving data
	SOCKADDR_IN clientAddr;
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(port);
	clientAddr.sin_addr.S_un.S_addr = address;

	// Socket creation
	SOCKET client{ socket(AF_INET, SOCK_DGRAM, 0) };

	// Write out to that socket
	if (sendto(client, buf.m_data, buf.m_size, 0, (sockaddr*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
		printf("Message failed to send\n");
	}

	closesocket(client);
}
