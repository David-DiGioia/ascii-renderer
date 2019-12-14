#include "Player.h"

void Player::move(const Buffer& buf, Direction dir)
{
	if (dir == Direction::UP) {
		m_row -= m_speed;
		m_row = m_row < 0 ? 0 : m_row;
	}
	if (dir == Direction::DOWN) {
		m_row += m_speed;
		m_row = m_row >= buf.getHeight() ? buf.getHeight() - 1 : m_row;
	}
	if (dir == Direction::RIGHT) {
		m_col += m_speed;
		m_col = m_col >= buf.getWidth() ? buf.getWidth() - 1 : m_col;
	}
	if (dir == Direction::LEFT) {
		m_col -= m_speed;
		m_col = m_col < 0 ? 0 : m_col;
	}
}

unsigned Player::getRow() const
{
	return m_row;
}

unsigned Player::getCol() const
{
	return m_col;
}
