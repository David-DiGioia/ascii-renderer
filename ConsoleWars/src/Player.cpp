#include "Player.h"
#include "Render.h"
#include "Constants.h"

Bullet::Bullet(int p_row, int p_col, char p_dir)
	: row{ p_row }
	, col{ p_col }
	, dir{ p_dir }
{}


void killPlayer(Player& p, std::vector<unsigned>& addresses, std::mutex& mtxPlayers, std::mutex& mtxIPs)
{
	{
		std::lock_guard<std::mutex> locker{ mtxPlayers };
		p.alive = false;
	}
	if (p.id > 0) {
		std::lock_guard<std::mutex> locker{ mtxIPs };
		addresses[p.id - (unsigned long long)1] = 0;
	}
}

void damageBlock(char& c)
{
	if (c >= '2' && c <= '9') --c;
	else if (c == '1') c = ' ';
}

Coordinate moveBullet(std::vector<Bullet>& b, int idx, Buffer& buf)
{
	char& c{ moveObj(b[idx], buf, b[idx].dir) };
	bool dieBullet{ collidableChar(c) };

	damageBlock(c);
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) dieBullet = true;

	Coordinate coord{ b[idx].row, b[idx].col };
	if (dieBullet) b.erase(b.begin() + idx);
	return coord;
}

void updateBullets(std::vector<Bullet>& b, std::vector<Player>& p, std::vector<unsigned>& IPs, Buffer& buf, std::mutex& mtxPlayers, std::mutex& mtxIPs)
{
	for (int i{ 0 }; i < b.size(); ++i) {
		Coordinate coord{ moveBullet(b, i, buf) };
		for (int i{ 0 }; i < p.size(); ++i) {
			if (p[i].row == coord.row && p[i].col == coord.col && p[i].alive) {
				killPlayer(p[i], IPs, mtxPlayers, mtxIPs);
			}
		}
	}
}

Player::Player(char c, char p_id)
	: character{ c }
	, id{ p_id }
{}

//Player& Player::operator=(Player other)
//{
//	id = other.id;
//	row = other.row;
//	col = other.col;
//	character = other.character;
//	alive = other.alive;
//	facing = other.facing;
//	return other;
//}

bool collidableChar(char c)
{
	return c == WALL || (c >= '1' && c <= '9');
}
