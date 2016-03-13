#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Maze.hpp"

class Player : public sf::Drawable, public sf::Transformable
{
public:

	Player(Maze& maze);
	~Player() = default;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	//returns true if the player moves
	bool handleEvent(sf::Event event);

private:
	sf::CircleShape m_playerSprite;
	bool			doMove(Direction direction);
	Maze*			m_maze;
	sf::Vector2f	m_position;
	sf::View		m_view;
};
