#include <cstdio>
#include "Input.h"
#include "Render.h"
#include "Player.h"

// A Test Comment

// width and height (in characters) of buffer
constexpr unsigned WIDTH{ 80 };
constexpr unsigned HEIGHT{ 20 };

// buf stores all the characters that will be drawn to screen
Buffer buf{ WIDTH, HEIGHT };
// player is the 'X' which appears in the terminal when run
Player player;

int main()
{
	while (true)
	{
		playerInput(buf, player);
		buf.clear();
		buf.at(player.getRow(), player.getCol()) = 'X';
		draw(buf);
	}
}