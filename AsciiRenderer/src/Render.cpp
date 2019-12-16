#include "Render.h"
#include <cstdio>
#include <Windows.h>

void draw(const Buffer& buf)
{
	// reset cursor to top right
	static const auto hOut{ GetStdHandle(STD_OUTPUT_HANDLE) };
	SetConsoleCursorPosition(hOut, { 0, 0 });
	printf(buf.m_data);
}

Buffer::Buffer(unsigned width, unsigned height)
	: m_width{ width + 1 } // add 1 to width so last column can be new lines
	, m_height{ height }
	, m_size{ (unsigned long long)m_width * m_height + 1 } // add 1 for null terminating char
{
	m_data = new char[m_size];
	clear();
}

Buffer::~Buffer()
{
	delete[] m_data;
}

char& Buffer::at(unsigned row, unsigned col) const
{
	return m_data[row * m_width + col];
}

char& Buffer::at(unsigned row, unsigned col)
{
	return m_data[row * m_width + col];
}

void Buffer::clear()
{
	std::fill(m_data, m_data + m_size, ' ');
	for (auto i{ m_width - 1 }; i < m_size; i += m_width)
		m_data[i] = '\n';
	m_data[m_size - 1] = '\0';
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