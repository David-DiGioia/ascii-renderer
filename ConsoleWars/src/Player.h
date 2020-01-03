#pragma once
#include <mutex>
#include "Render.h"
#include "Constants.h"

// id:     index of player vector
// move:   0=stay, 1=up, 2=down, 3=right, 4=left
// shoot:  0=none, 1=shoot
// place:  0=none, 1=place
struct PlayerInput
{
	char id;
	char move;
	char shoot;
	char place;

	PlayerInput(char p_id, char p_move, char p_shoot, char p_place)
		: id{ p_id }
		, move{ p_move }
		, shoot{ p_shoot }
		, place{ p_place }
	{}
};

struct Coordinate
{
	int row;
	int col;
};

struct Bullet
{
	int row;
	int col;
	char dir;

	Bullet(int p_row, int p_col, char p_dir);
};

struct Player
{
	int row{ 1 };
	int col{ 1 };
	char id;
	char character;
	bool alive{ true };
	char facing{ UP };

	Player(char c, char p_id);
	Player& operator=(const Player& other) = default;
	//Player& operator=(Player other);
};

void killPlayer(Player& p, std::vector<unsigned>& addresses, std::mutex& mtxPlayers, std::mutex& mtxIPs);

void damageBlock(char& c);

Coordinate moveBullet(std::vector<Bullet>& b, int idx, Buffer& buf);

void updateBullets(std::vector<Bullet>& b, std::vector<Player>& p, std::vector<unsigned>& IPs, Buffer& buf, std::mutex& mtxPlayers, std::mutex& mtxIPs);

bool collidableChar(char c);

// returns char that was collided with, otherwise the char that is landed on
template<typename T>
char& moveObj(T& t, Buffer& buf, char dir)
{
	if (dir & UP) {
		char& c{ buf.mapAt(t.row - 1, t.col) };
		if (collidableChar(c)) return c;
		--t.row;
		t.row = t.row < 0 ? 0 : t.row;
	}
	if (dir & DOWN) {
		char& c{ buf.mapAt(t.row + 1, t.col) };
		if (collidableChar(c)) return c;
		++t.row;
		t.row = t.row >= buf.getHeight() ? buf.getHeight() - 1 : t.row;
	}
	if (dir & RIGHT) {
		char& c{ buf.mapAt(t.row, t.col + 1) };
		if (collidableChar(c)) return c;
		++t.col;
		t.col = t.col >= buf.getWidth() ? buf.getWidth() - 1 : t.col;
	}
	if (dir & LEFT) {
		char& c{ buf.mapAt(t.row, t.col - 1) };
		if (collidableChar(c)) return c;
		--t.col;
		t.col = t.col < 0 ? 0 : t.col;
	}
	return buf.at(t.row, t.col);
}
