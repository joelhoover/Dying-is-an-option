#pragma once

#include <SFML/Graphics.hpp>
#include "Maze.hpp"
#include "Player.hpp"

class Enemy : public sf::Drawable, sf::Transformable 
{
public:
	Enemy(Maze& maze, Player& player);
	~Enemy() = default;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void	doMove();
	void	respawn();
	bool	hasKilled();

private:
	sf::Sprite							m_enemySprite;
	Maze*								m_maze;
	Player*								m_player;
	sf::Vector2u						m_position;
	Direction							m_currentDirection;
	static std::unique_ptr<sf::Texture>	m_enemyTexture;
};

