#include "Input.h"

KeyPress keyDown(int keyCode)
{
	// Windows function to check press
	auto key{ GetAsyncKeyState(keyCode) };
	KeyPress result;

	// Most significant bit: key is currently pressed
	result.keyDown = key && (1 << (sizeof(key) * 8 - 1));
	// Least significant bit: key is down and was down last time
	result.consecutiveDown = key && 1;
	return result;
}

void playerInput(const Buffer& buf, Player& player)
{
	if (keyDown(KEY_W).keyDown)
		player.move(buf, Direction::UP);
	if (keyDown(KEY_A).keyDown)
		player.move(buf, Direction::LEFT);
	if (keyDown(KEY_S).keyDown)
		player.move(buf, Direction::DOWN);
	if (keyDown(KEY_D).keyDown)
		player.move(buf, Direction::RIGHT);
}
