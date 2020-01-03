#include "Render.h"
#include <cstdio>
#include <Windows.h>
#include <fstream>
#include <algorithm>
#include <iostream>
#include "Constants.h"
#include "Player.h"

void draw(char* buf)
{
	// reset cursor to top right
	static const auto hOut{ GetStdHandle(STD_OUTPUT_HANDLE) };
	SetConsoleCursorPosition(hOut, { 0, 0 });
	printf(buf);
}

Buffer::Buffer(const std::string& mapFile, unsigned width, unsigned height)
	: m_width{ width + 1 } // add 1 to width so last column can be new lines
	, m_height{ height }
	, m_size{ (unsigned long long)m_width * m_height + 1 } // add 1 for null terminating char
{
	m_data = new char[m_size];
	m_map = new char[m_size];

	std::ifstream file(mapFile);
	file.seekg(0, std::ios::beg);

	if (!file.read(m_map, m_size - 1)) {
		printf("Error reading map file.\nMake sure there's a map called 'current.txt' in the maps folder.\n");
		std::cin.get();
		exit(4);
	}
	m_map[m_size - 1] = '\0';

	clear();
}

void drawPlayers(Buffer& buf, const std::vector<Player> players)
{
	for (const Player& p : players)
		if (p.alive) buf.at(p.row, p.col) = p.character;
}

void drawBullets(Buffer& buf, const std::vector<Bullet> bullets)
{
	for (const Bullet& b : bullets)
		buf.at(b.row, b.col) = BULLET;
}

Buffer::~Buffer()
{
	delete[] m_data;
	delete[] m_map;
}

const char& Buffer::at(unsigned row, unsigned col) const
{
	return m_data[row * m_width + col];
}

char& Buffer::at(unsigned row, unsigned col)
{
	return m_data[row * m_width + col];
}

char& Buffer::mapAt(unsigned row, unsigned col)
{
	return m_map[row * m_width + col];
}

void Buffer::clear()
{
	std::copy(m_map, m_map + m_size, m_data);

	//std::fill(m_data, m_data + m_size, ' ');
	//for (auto i{ m_width - 1 }; i < m_size; i += m_width)
	//	m_data[i] = '\n';
	//m_data[m_size - 1] = '\0';
}

unsigned Buffer::getWidth() const
{
	// subtract 1, since there's a hidden column of newlines
	return m_width - 1;
}

unsigned Buffer::getHeight() const
{
	return m_height;
}

char* Buffer::getData()
{
	return m_data;
}
