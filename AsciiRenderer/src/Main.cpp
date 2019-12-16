#include <cstdio>
#include <chrono>
#include <thread>
#include "Input.h"
#include "Render.h"
#include "Player.h"

// width and height (in characters) of buffer
constexpr unsigned WIDTH{ 80 };
constexpr unsigned HEIGHT{ 20 };
constexpr int FPS{ 30 };
const std::chrono::milliseconds frameTime{ 1000 / FPS }; // 1000 bc we're using milliseconds

// buf stores all the characters that will be drawn to screen
Buffer buf{ WIDTH, HEIGHT };
// player is the 'X' which appears in the terminal when run
Player player;

int main()
{



	while (true)
	{
		auto start{ std::chrono::system_clock::now() };

		playerInput(buf, player);
		buf.clear();
		buf.at(player.getRow(), player.getCol()) = 'X';
		draw(buf);

		// Sleep for number of milliseconds which puts us at our target FPS
		auto end{ std::chrono::system_clock::now() };
		auto elapsed{ std::chrono::duration_cast<std::chrono::milliseconds>(end - start) };
		std::this_thread::sleep_for(frameTime - elapsed);
	}
}