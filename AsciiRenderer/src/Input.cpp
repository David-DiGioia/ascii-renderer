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
