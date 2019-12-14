#pragma once
#include <Windows.h>
#include "Render.h"
#include "Player.h"

// The constants for each of these keys come from
// the Windows.h header.
// https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
constexpr int KEY_LEFT{ VK_LEFT };
constexpr int KEY_RIGHT{ VK_RIGHT };
constexpr int KEY_UP{ VK_UP };
constexpr int KEY_DOWN{ VK_DOWN };
constexpr int KEY_W{ 0x57 };
constexpr int KEY_A{ 0x41 };
constexpr int KEY_S{ 0x53 };
constexpr int KEY_D{ 0x44 };

struct KeyPress
{
	bool keyDown;
	bool consecutiveDown;
};

KeyPress keyDown(int keyCode);
void playerInput(const Buffer& buf, Player& player);
