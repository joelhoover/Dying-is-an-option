#include "Maze.hpp"
#include <chrono>

constexpr float nodeSize = 50;
constexpr float	corridorWidthFactor = 0.4;

Maze::Maze(sf::Vector2u size)
{
	floorTexture.loadFromFile("floor.png");
	floorTexture.setRepeated(true);

	endTexture.loadFromFile("SFML_LOGO.png");
	endSprite.setTexture(endTexture);
	auto endBounds(endSprite.getGlobalBounds());
	endSprite.setOrigin(endBounds.width / 2, endBounds.height / 2);
	endSprite.setScale(0.15f,0.15f);

	m_size = size.x;
	mazeNodes.resize(size.x);
	for (auto& column : mazeNodes)
	{
		column.resize(size.y);
		for (auto& node : column)
		{
			node = std::make_unique<MazeNode>();
		}
	}
	regenerate(true);
}

void Maze::generate(int seed)
{
	mazeNodes.resize(m_size);
	for (auto& column : mazeNodes)
	{
		column.resize(m_size);
		for (auto& node : column)
		{
			node = std::make_unique<MazeNode>();
		}
	}

	//seed the engine and set up the distribution
	randomEngine.seed(seed);
	randomDistribution = std::uniform_int_distribution<unsigned int>(0, 4);	//magic number, so shoot me

	//clear the vertexarray
	maze.clear();
	maze.setPrimitiveType(sf::PrimitiveType::Quads);


	//explore all the nodes recursively, starting at top left
	exploreNode({ 0,0 });

	//set all the tex coords to the vertex positions
	for (auto i(0); i < maze.getVertexCount(); i++)
	{
		maze[i].texCoords = maze[i].position;
	}

	//set the position of the exit Sprite;
	endSprite.setPosition(endPosition.x * nodeSize, endPosition.y * nodeSize);
}

void Maze::regenerate(bool newFinish)
{
	randomDistribution = std::uniform_int_distribution<unsigned int>(0, mazeNodes.size());
	if (newFinish)
	{
		randomDistribution = std::uniform_int_distribution<unsigned int>(0, -1);
		std::mt19937 engine;
		engine.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		randomDistribution = std::uniform_int_distribution<unsigned int>(0, mazeNodes.size());
		endPosition.x = randomDistribution(engine);
		endPosition.y = randomDistribution(engine);
	}
	generate(randomDistribution(randomEngine));
}

void Maze::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	auto transform = getTransform();
	states.transform = transform;
	states.texture = &floorTexture;
	target.draw(maze, states);
	target.draw(endSprite);
}

const float Maze::getNodeSize()
{
	return nodeSize;
}

sf::Vector2u Maze::getMazeSize()
{
	return{ mazeNodes.size(),mazeNodes[mazeNodes.size() - 1].size() };
}

void	Maze::exploreNode(sf::Vector2u position)
{
	//set this node as visited first, and add to the vertexArray
	auto& thisNode = mazeNodes[position.x][position.y];
	thisNode->visited = true;

	//create a quad for the current node
	maze.append(sf::Vector2f(position.x * nodeSize - nodeSize * corridorWidthFactor, position.y * nodeSize - nodeSize * corridorWidthFactor));
	maze.append(sf::Vector2f(position.x * nodeSize + nodeSize * corridorWidthFactor, position.y * nodeSize - nodeSize * corridorWidthFactor));
	maze.append(sf::Vector2f(position.x * nodeSize + nodeSize * corridorWidthFactor, position.y * nodeSize + nodeSize * corridorWidthFactor));
	maze.append(sf::Vector2f(position.x * nodeSize - nodeSize * corridorWidthFactor, position.y * nodeSize + nodeSize * corridorWidthFactor));

	//if it's the exit node then return early
	if (position == endPosition)
	{
		return;
	}
	//get the available neighbours (as a direction bit mask)
	auto availableNeighbours = getUnvisitedNeighbours(position);

	while (availableNeighbours)
	{
		//get a random direction
		auto randomDirection = getRandomDirection();

		while (!(randomDirection & availableNeighbours))
		{
			//selected direction isn't available, get a new one
			randomDirection = getRandomDirection();
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

		//and a quad for the connection to the next node
		//I know there'll be a smarter way of doing this. Leave me alone
		switch (randomDirection)
		{
		case Up:
			maze.append(sf::Vector2f(position.x * nodeSize - nodeSize *corridorWidthFactor, nextPosition.y * nodeSize + nodeSize * corridorWidthFactor));
			maze.append(sf::Vector2f(position.x * nodeSize + nodeSize *corridorWidthFactor, nextPosition.y * nodeSize + nodeSize * corridorWidthFactor));
			maze.append(sf::Vector2f(position.x * nodeSize + nodeSize *corridorWidthFactor, position.y * nodeSize - nodeSize * corridorWidthFactor));
			maze.append(sf::Vector2f(position.x * nodeSize - nodeSize *corridorWidthFactor, position.y * nodeSize - nodeSize * corridorWidthFactor));
			break;
		case Down:
			maze.append(sf::Vector2f(position.x * nodeSize - nodeSize * corridorWidthFactor, position.y * nodeSize + nodeSize * corridorWidthFactor));
			maze.append(sf::Vector2f(position.x * nodeSize + nodeSize * corridorWidthFactor, position.y * nodeSize + nodeSize * corridorWidthFactor));
			maze.append(sf::Vector2f(position.x * nodeSize + nodeSize * corridorWidthFactor, nextPosition.y * nodeSize - nodeSize * corridorWidthFactor));
			maze.append(sf::Vector2f(position.x * nodeSize - nodeSize * corridorWidthFactor, nextPosition.y * nodeSize - nodeSize * corridorWidthFactor));
			break;
		case Left:
			maze.append(sf::Vector2f(nextPosition.x * nodeSize + nodeSize * corridorWidthFactor, position.y * nodeSize + nodeSize * corridorWidthFactor));
			maze.append(sf::Vector2f(position.x * nodeSize - nodeSize * corridorWidthFactor, position.y * nodeSize + nodeSize * corridorWidthFactor));
			maze.append(sf::Vector2f(position.x * nodeSize - nodeSize * corridorWidthFactor, position.y * nodeSize - nodeSize * corridorWidthFactor));
			maze.append(sf::Vector2f(nextPosition.x * nodeSize + nodeSize * corridorWidthFactor, position.y * nodeSize - nodeSize * corridorWidthFactor));
			break;
		case Right:
			maze.append(sf::Vector2f(position.x * nodeSize + nodeSize * corridorWidthFactor, position.y * nodeSize + nodeSize * corridorWidthFactor));
			maze.append(sf::Vector2f(nextPosition.x * nodeSize - nodeSize * corridorWidthFactor, position.y * nodeSize + nodeSize * corridorWidthFactor));
			maze.append(sf::Vector2f(nextPosition.x * nodeSize - nodeSize * corridorWidthFactor, position.y * nodeSize - nodeSize * corridorWidthFactor));
			maze.append(sf::Vector2f(position.x * nodeSize + nodeSize * corridorWidthFactor, position.y * nodeSize - nodeSize * corridorWidthFactor));
			break;
		}

		//update the available directions
		thisNode->availableDirections |= randomDirection;
		auto& nextNode(mazeNodes[nextPosition.x][nextPosition.y]);
		auto oppositeDirection(getOppositeDirection(static_cast<Direction>(randomDirection)));
		nextNode->availableDirections |= oppositeDirection;

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

int	Maze::getAvailableDirections(sf::Vector2u pos)
{
	return mazeNodes[pos.x][pos.y]->availableDirections;
}

Direction Maze::getRandomDirection()
{
	return static_cast<Direction>(1 << randomDistribution(randomEngine));
}

sf::Vector2u Maze::getEndPosition()
{
	return endPosition;
}