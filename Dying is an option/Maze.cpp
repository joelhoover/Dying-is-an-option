#include "Maze.hpp"
#include "MazeNode.hpp"

constexpr int nodeSize = 10;

void Maze::generate(sf::Vector2u size, int seed)
{
	mazeNodes.resize(size.x);
	for (auto& column : mazeNodes)
	{
		column.resize(size.y);
		for (auto& node : column)
		{
			node = std::make_unique<MazeNode>();
		}
	}

	//seed the engine and set up the distribution
	randomEngine.seed(seed);
	randomDistribution = std::uniform_int_distribution<int>(0, 4);	//magic number, so shoot me

	//clear the vertexarray
	maze.clear();
	maze.setPrimitiveType(sf::PrimitiveType::Lines);

	//explore all the nodes recursively, starting at top left
	exploreNode({ 0,0 });
}

void Maze::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	auto transform = getTransform();
	target.draw(maze, transform);
}

void	Maze::exploreNode(sf::Vector2u position)
{
	//set this node as visited first, and add to the vertexArray
	auto& thisNode = mazeNodes[position.x][position.y];
	thisNode->visited = true;

	//get the available neighbours (as a direction bit mask)
	auto availableNeighbours = getUnvisitedNeighbours(position);

	while (availableNeighbours)
	{
		//get a random direction
		auto randomDirection = 1 << randomDistribution(randomEngine);

		while (!(randomDirection & availableNeighbours))
		{
			//selected direction isn't available, get a new one
			randomDirection = 1 << randomDistribution(randomEngine);
		}

		//update our position
		auto nextPosition(position);
		switch (randomDirection)
		{
		case Up:
			--nextPosition.y;
			break;
		case Down:
			++nextPosition.y;
			break;
		case Left:
			--nextPosition.x;
			break;
		case Right:
			++nextPosition.x;
			break;
		}

		//create a line between the two positions
		maze.append(sf::Vector2f(position.x * nodeSize, position.y * nodeSize));
		maze.append(sf::Vector2f(nextPosition.x * nodeSize, nextPosition.y * nodeSize));

		//then explore the node
		exploreNode(nextPosition);

		//check if there are any directions left to explore
		availableNeighbours = getUnvisitedNeighbours(position);
	}
}

int	Maze::getUnvisitedNeighbours(sf::Vector2u position)
{
	auto availableNeighbours(0);

	//Can we move up?
	if (position.y)
	{
		//check if the position above has been visited
		if (!mazeNodes[position.x][position.y - 1]->visited)
		{
			availableNeighbours |= Up;
		}
	}
	
	//Can we move down?
	if (position.y < mazeNodes[position.x].size() - 1)
	{
		if (!mazeNodes[position.x][position.y + 1]->visited)
		{
			availableNeighbours |= Down;
		}
	}

	//Can we move Left?
	if (position.x)
	{
		if (!mazeNodes[position.x-1][position.y]->visited)
		{
			availableNeighbours |= Left;
		}
	}

	//Can we move right?
	if (position.x < mazeNodes.size() - 1)
	{
		if (!mazeNodes[position.x + 1][position.y]->visited)
		{
			availableNeighbours |= Right;
		}
	}

	return availableNeighbours;
}
