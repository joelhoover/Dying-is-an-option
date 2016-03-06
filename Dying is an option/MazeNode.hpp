#pragma once

enum Direction
{
	Up = 1,
	Down = 2,
	Left = 4,
	Right = 8
};

class MazeNode
{
public:
	MazeNode() = default;
	~MazeNode() = default;

	bool	visited = false;
};

