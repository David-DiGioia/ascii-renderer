#pragma once

// width and height (in characters) of buffer
constexpr unsigned WIDTH{ 120 };
constexpr unsigned HEIGHT{ 28 };
constexpr int FPS{ 25 };
constexpr int PORT{ 54000 };
constexpr char WALL{ '#' };
constexpr char BULLET{ '*' };
constexpr char BLOCK{ '3' };
constexpr char STAY{ 0 };
constexpr char UP{ 1 << 0 };
constexpr char DOWN{ 1 << 1 };
constexpr char RIGHT{ 1 << 2 };
constexpr char LEFT{ 1 << 3 };
constexpr int SMALL_BUFFER{8};
