#include <iostream>
#include "Input.h"

int main()
{
	std::cout << "Application starting...\n";
	while (true)
	{
		if (keyDown(KEY_W).keyDown)
			std::cout << "W pressed!\n";
		if (keyDown(KEY_A).keyDown)
			std::cout << "A pressed!\n";
		if (keyDown(KEY_S).keyDown)
			std::cout << "S pressed!\n";
		if (keyDown(KEY_D).keyDown)
			std::cout << "D pressed!\n";
	}
	std::cout << "Application exiting...\n";
}