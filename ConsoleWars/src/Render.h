#pragma once
#include <algorithm>
#include <vector>
#include <string>

class Player;
class Bullet;

class Buffer
{
private:
	const unsigned m_width;
	const unsigned m_height;
	const unsigned long long m_size;
	char* m_data;
	char* m_map;
public:

	Buffer(const std::string& mapFile, unsigned width, unsigned height);

	~Buffer();

	// Access const element at (row, col)
	const char& at(unsigned row, unsigned col) const;

	// Access element at (row, col)
	char& at(unsigned row, unsigned col);

	char& mapAt(unsigned row, unsigned col);

	// Clear buffer to be all spaces, with column of newlines and null terminating char
	void clear();

	unsigned getWidth() const;

	unsigned getHeight() const;

	char* getData();

	// make draw a friend so it can access m_data
	friend void broadcastMap(unsigned address, int port, const Buffer& buf);
};

void draw(char* buf);

void drawPlayers(Buffer& buf, const std::vector<Player> players);

void drawBullets(Buffer& buf, const std::vector<Bullet> bullets);
