#pragma once
#include "Render.h"

enum class Direction
{
	UP, DOWN, RIGHT, LEFT
};

class Player
{
private:
	int m_row{ 0 };
	int m_col{ 0 };
	int m_speed{ 1 };
public:
	// Move player with bounds checking
	void move(const Buffer& buf, Direction dir);

	unsigned getRow() const;

	unsigned getCol() const;
};
