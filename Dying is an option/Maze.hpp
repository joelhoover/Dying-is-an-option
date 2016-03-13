#pragma once

#include <vector>
#include <random>
#include <memory>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "MazeNode.hpp"

class Maze : public sf::Drawable, public sf::Transformable
{
public:
	Maze();

	void	generate(sf::Vector2u size, int seed);
	void	regenerate();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	const float		getNodeSize();
	sf::Vector2u	getMazeSize();

	int			getAvailableDirections(sf::Vector2u pos);
	Direction	getRandomDirection();

private:

	void												exploreNode(sf::Vector2u position);
	int													getUnvisitedNeighbours(sf::Vector2u position);

	std::vector<std::vector<std::unique_ptr<MazeNode>>>	mazeNodes;
	std::mt19937										randomEngine;
	std::uniform_int_distribution<unsigned int>			randomDistribution;
	sf::VertexArray										maze;
	sf::Texture											floorTexture;
};

