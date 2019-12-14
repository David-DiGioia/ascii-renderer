#pragma once
#include <algorithm>

class Buffer
{
private:
	const unsigned m_width;
	const unsigned m_height;
	const unsigned long long m_size;
	char* m_data;
public:

	Buffer(unsigned width, unsigned height);

	~Buffer();

	// Access const element at (row, col)
	char& at(unsigned row, unsigned col) const;

	// Access element at (row, col)
	char& at(unsigned row, unsigned col);

	// Clear buffer to be all spaces, with column of newlines and null terminating char
	void clear();

	unsigned getWidth() const;

	unsigned getHeight() const;

	// make draw a friend so it can access m_data
	friend void draw(const Buffer& buf);
};
