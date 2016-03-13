#pragma once

enum Direction
{
	Up = 1,
	Down = 2,
	Left = 4,
	Right = 8
};

static Direction getOppositeDirection(Direction direction)
{
	switch (direction)
	{
	case Up:
		return Down;
		break;
	case Down:
		return Up;
		break;
	case Left:
		return Right;
		break;
	case Right:
		return Left;
		break;
	}
}

class MazeNode
{
public:
	MazeNode() = default;
	~MazeNode() = default;

	bool	visited = false;
	int		availableDirections = 0;
};

