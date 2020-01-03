#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <functional>
#include <string>
#include "Input.h"
#include "Render.h"
#include "Player.h"
#include "Network.h"
#include "Constants.h"
#include "MainClient.h"

const std::chrono::milliseconds frameTime{ 1000 / FPS }; // 1000 bc we're using milliseconds
std::mutex mtxInputs;
std::mutex mtxPlayers;
std::mutex mtxIPs;

// buf stores all the characters that will be drawn to screen
Buffer buf{ "maps/current.txt", WIDTH, HEIGHT };
// player is the letter which appears in the terminal when run
std::vector<Player> players;
// stores all active bullets in scene
std::vector<Bullet> bullets;
// stores inputs from all players
std::queue<PlayerInput> inputs;
// store IP addresses of players
std::vector<unsigned> addresses;

void hideConsoleCursor(bool visible)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;
	cursorInfo.dwSize = 2;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = visible;
	SetConsoleCursorInfo(out, &cursorInfo);
}

int mainServer(char c)
{
	hideConsoleCursor(false);
	players.emplace_back(c, 0);
	startNetwork();
	std::thread threadTCP{ receiveTCP, PORT, std::ref(players), std::ref(addresses), std::ref(mtxPlayers), std::ref(mtxIPs) };
	std::thread threadUDP{ receiveUDP, PORT, std::ref(inputs), std::ref(mtxInputs) };
	while (true)
	{
		auto start{ std::chrono::system_clock::now() };

		updateBullets(bullets, players, addresses, buf, mtxPlayers, mtxIPs);
		localInput(inputs, 0);
		{
			std::lock_guard<std::mutex> locker{ mtxInputs };
			processInputs(inputs, players, bullets, buf);
		}
		buf.clear();
		{
			std::lock_guard<std::mutex> locker{ mtxPlayers };
			drawPlayers(buf, players);
		}
		drawBullets(buf, bullets);
		draw(buf.getData());
		{
			std::lock_guard<std::mutex> locker{ mtxIPs };
			for (unsigned ip : addresses)
				if (ip) broadcastMap(ip, PORT + 1, buf);
		}

		// Sleep for number of milliseconds which puts us at our target FPS
		auto end{ std::chrono::system_clock::now() };
		auto elapsed{ std::chrono::duration_cast<std::chrono::milliseconds>(end - start) };
		std::this_thread::sleep_for(frameTime - elapsed);
	}
	threadUDP.join();
	threadTCP.join();
	endNetwork();
}

bool inAlphabet(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int main()
{
	printf("Welcome to Console Wars!\nThe goal is to kill all the enemy letters.\n\n");
	printf("Controls\nMove:\t\tW, A, S, D\nShoot:\t\tSPACE\nPlace block:\tF\n\n");
	printf("Choose your character (a single character from the alphabet):\n");

	bool validCharacter{ true };
	char c{ -1 };
	do {
		validCharacter = true;
		std::string charString;
		getline(std::cin, charString);
		c = charString[0];
		if (!inAlphabet(c)) {
			validCharacter = false;
			printf("That character is not allowed. Choose something in the alphabet.\n");
		}
	} while (!validCharacter);

	printf("Would you like to host the server? (y/n):\n");
	std::string response;
	getline(std::cin, response);
	system("cls");
	if (response[0] == 'y') return mainServer(c);
	else return mainClient(c);
}
