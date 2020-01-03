#include "Input.h"
#include <utility>

KeyPress keyDown(int keyCode)
{
	// Windows function to check press
	auto key{ GetAsyncKeyState(keyCode) };
	KeyPress result;

	// Most significant bit: key is currently pressed
	result.keyDown = key && (1 << (sizeof(key) * 8 - 1));
	// Least significant bit: key is down and was down last time
	//result.consecutiveDown = key && 1; // maybe not reliable??
	static bool spaceConsecutiveDown{ false };
	static bool lshiftConsecutiveDown{ false };

	if (keyCode == KEY_SPACE) {
		if (GetAsyncKeyState(KEY_SPACE) && (1 << (sizeof(key) * 8 - 1))) {
			result.consecutiveDown = spaceConsecutiveDown;
			spaceConsecutiveDown = true;
		}
		else {
			result.consecutiveDown = false;
			spaceConsecutiveDown = false;

		}
	}

	if (keyCode == KEY_F) {
		if (GetAsyncKeyState(KEY_F) && (1 << (sizeof(key) * 8 - 1))) {
			result.consecutiveDown = lshiftConsecutiveDown;
			lshiftConsecutiveDown = true;
		}
		else {
			result.consecutiveDown = false;
			lshiftConsecutiveDown = false;
		}
	}

	return result;
}

void localInput(std::queue<PlayerInput>& inputs, int id)
{
	KeyPress spacePress{ keyDown(KEY_SPACE) };
	KeyPress shiftPress{ keyDown(KEY_F) };

	char shoot{ spacePress.keyDown && !spacePress.consecutiveDown };
	char place{ shiftPress.keyDown && !shiftPress.consecutiveDown };

	char dir{ STAY };
	if (keyDown(KEY_W).keyDown) dir |= UP;
	if (keyDown(KEY_S).keyDown) dir |= DOWN;
	if (keyDown(KEY_D).keyDown) dir |= RIGHT;
	if (keyDown(KEY_A).keyDown) dir |= LEFT;

	inputs.emplace(id, dir, shoot, place);
}

void processInputs(std::queue<PlayerInput>& inputs, std::vector<Player>& players, std::vector<Bullet>& bullets, Buffer& buf)
{
	while (!inputs.empty()) {
		PlayerInput input{ inputs.front() };
		inputs.pop();
		if (!players[input.id].alive) continue;

		// movement
		if (input.move & UP) {
			moveObj(players[input.id], buf, UP);
			players[input.id].facing = UP;
		}
		if (input.move & DOWN) {
			moveObj(players[input.id], buf, DOWN);
			players[input.id].facing = DOWN;
		}
		if (input.move & RIGHT) {
			moveObj(players[input.id], buf, RIGHT);
			players[input.id].facing = RIGHT;
		}
		if (input.move & LEFT) {
			moveObj(players[input.id], buf, LEFT);
			players[input.id].facing = LEFT;
		}

		// bullet
		if (input.shoot) {
			char facing{ players[input.id].facing };
			if (facing == UP) {
				char& c{ buf.mapAt(players[input.id].row - 1, players[input.id].col) };
				if (collidableChar(c))
					damageBlock(c);
				else
					bullets.emplace_back(players[input.id].row - 1, players[input.id].col, facing);
			}
			else if (facing == DOWN) {
				char& c{ buf.mapAt(players[input.id].row + 1, players[input.id].col) };
				if (collidableChar(c))
					damageBlock(c);
				else
					bullets.emplace_back(players[input.id].row + 1, players[input.id].col, facing);
			}
			else if (facing == RIGHT) {
				char& c{ buf.mapAt(players[input.id].row, players[input.id].col + 1) };
				if (collidableChar(c))
					damageBlock(c);
				else
					bullets.emplace_back(players[input.id].row, players[input.id].col + 1, facing);
			}
			else if (facing == LEFT) {
				char& c{ buf.mapAt(players[input.id].row, players[input.id].col - 1) };
				if (collidableChar(c))
					damageBlock(c);
				else
					bullets.emplace_back(players[input.id].row, players[input.id].col - 1, facing);
			}
		}

		// place block
		if (input.place) {
			buf.mapAt(players[input.id].row, players[input.id].col) = BLOCK;
		}
	}
}
