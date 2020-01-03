#include "MainClient.h"
#include <cstdio>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <queue>
#include "Network.h"
#include "Input.h"
#include "Render.h"
#include "Constants.h"

#pragma comment(lib, "ws2_32.lib")

constexpr int MAP_BUFFER_SIZE{ 4096 };

// returns id
int joinServer(const std::string& address, int port, char character)
{
	SOCKET server;
	SOCKADDR_IN addr;

	server = socket(AF_INET, SOCK_STREAM, 0);

	inet_pton(AF_INET, address.c_str(), &addr.sin_addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if (connect(server, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		printf("Couldn't connect to server.\nPress any key to quit.\n");
		std::cin.get();
		exit(1);
	}
	printf("Connected to server!\n");

	char buffer[SMALL_BUFFER]{ 'j', character };

	send(server, buffer, sizeof(buffer), 0);
	printf("Message sent!\n");

	int id{ -1 };
	while (recv(server, buffer, sizeof(buffer), 0)) {
		id = buffer[0];
	}
	printf("Server has responded with id %d\n", id);

	closesocket(server);
	return id;
}

void sendInputs(const std::string& address, int port, std::queue<PlayerInput>& inputs)
{
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	inet_pton(AF_INET, address.c_str(), &serverAddr.sin_addr);

	// Socket creation
	SOCKET server{ socket(AF_INET, SOCK_DGRAM, 0) };

	while (!inputs.empty())
	{
		PlayerInput input{ inputs.front() };
		inputs.pop();
		char buf[4]{ input.id, input.move, input.shoot, input.place };
		if (sendto(server, buf, sizeof(buf), 0, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
			printf("Message failed to send\n");
		}
	}

	closesocket(server);
}

void listenMap(int port)
{
	// Bind socket to ip address and port
	SOCKET client{ socket(AF_INET, SOCK_DGRAM, 0) };

	SOCKADDR_IN clientAddr;
	clientAddr.sin_addr.S_un.S_addr = ADDR_ANY;
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(port);

	if (bind(client, (sockaddr*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
		printf("Can't bind socket! Error code: %d\n", WSAGetLastError());
		__debugbreak();
		exit(2);
	}

	SOCKADDR_IN serverAddr;
	int serverLength{ sizeof(serverAddr) };
	ZeroMemory(&serverAddr, serverLength);

	// Message from server will be stored here
	char buf[MAP_BUFFER_SIZE]{};

	// Wait for message
	int bytesIn{ recvfrom(client, buf, sizeof(buf), 0, (sockaddr*)&serverAddr, &serverLength) };

	draw(buf);

	closesocket(client);
}

void hideConsoleCursor(bool visible);

int mainClient(char c)
{
	printf("Enter server IP address (local ip is 127.0.0.1):\n");
	std::string address;
	getline(std::cin, address);

	printf("Enter the port (maybe %d):\n", PORT);
	int port;
	std::cin >> port;

	hideConsoleCursor(false);
	startNetwork();
	int id{ joinServer(address, port, c) };

	std::queue<PlayerInput> inputs;

	while (true)
	{
		localInput(inputs, id);
		sendInputs(address, port, inputs);
		listenMap(port + 1);
	}
	
	endNetwork();
}