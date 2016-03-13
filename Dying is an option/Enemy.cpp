#include "Enemy.hpp"
#include <chrono>

Enemy::Enemy(Maze& maze,Player& player):
	m_maze(&maze),
	m_player(&player)
{
	//use a red circle for now
	m_enemySprite.setRadius(10);
	m_enemySprite.setFillColor(sf::Color::Red);
	m_enemySprite.setOrigin(m_enemySprite.getRadius(), m_enemySprite.getRadius());
	
	//start in a random Position
	std::mt19937 engine;
	engine.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<unsigned int> posDistribution(0,m_maze->getMazeSize().x-1);
	m_position = { posDistribution(engine),posDistribution(engine) };
	auto nodeSize(m_maze->getNodeSize());
	setPosition(m_position.x * nodeSize, m_position.y * nodeSize);

	m_currentDirection = Up;
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	auto transform = getTransform();
	target.draw(m_enemySprite,transform);
}

bool	Enemy::hasKilled()
{
	auto playerPosition(m_player->getMazePosition());
	if (playerPosition == m_position)
	{
		m_maze->regenerate();
		respawn();
		return true;
	}
	return false;
}
void	Enemy::doMove()
{
	auto availableDirections = m_maze->getAvailableDirections(m_position);
	
	//if we can't keep going in the current direction, find a new one
	if (!(availableDirections & m_currentDirection))
	{
		m_currentDirection = m_maze->getRandomDirection();
		while (!(m_currentDirection & availableDirections))
		{
			m_currentDirection = m_maze->getRandomDirection();
		}
	}

	switch (m_currentDirection)
	{
	case Up:
		--m_position.y;
		break;
	case Down:
		++m_position.y;
		break;
	case Left:
		--m_position.x;
		break;
	case Right:
		++m_position.x;
		break;
	default:
		__debugbreak();
		break;
	}

	auto nodeSize(m_maze->getNodeSize());
	setPosition(m_position.x * nodeSize, m_position.y * nodeSize);
}

void	Enemy::respawn()
{
	//just go to a new random position
	std::mt19937 engine;
	engine.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<unsigned int> posDistribution(0, m_maze->getMazeSize().x-1);
	m_position = { posDistribution(engine),posDistribution(engine) };
	auto nodeSize(m_maze->getNodeSize());
	setPosition(m_position.x * nodeSize, m_position.y * nodeSize);
}