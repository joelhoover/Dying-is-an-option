#include "Player.hpp"

Player::Player(Maze& maze) :
	m_maze(&maze)
{
	m_playerSprite.setRadius(5);
	m_playerSprite.setFillColor(sf::Color::Green);

	//start off at the top left of the map
	m_position = { 0,0 };

	//origin to center
	m_playerSprite.setOrigin(5, 5);

	//set up the initial view
	m_view.setCenter(getPosition());
	m_view.zoom(0.2);
}

bool Player::handleEvent(sf::Event event)
{
	switch (event.type)
	{
	case sf::Event::KeyPressed:
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Left:
			return doMove(Left);
		case sf::Keyboard::Right:
			return doMove(Right);
		case sf::Keyboard::Up:
			return doMove(Up);
		case sf::Keyboard::Down:
			return doMove(Down);
		}
	}
	}
	return false;
}

bool	Player::doMove(Direction direction)
{
	auto nodeSize(m_maze->getNodeSize());
	auto availableDirections(m_maze->getAvailableDirections({unsigned int(getPosition().x / nodeSize), unsigned int(getPosition().y / nodeSize)}));
	
	switch (direction & availableDirections)
	{
	case Up:
		move({ 0,-nodeSize });
		break;
	case Down:
		move({ 0,nodeSize });
		break;
	case Left:
		move({ -nodeSize,0 });
		break;
	case Right:
		move({ nodeSize,0 });
		break;

	}
	m_view.setCenter(getPosition());
	return true;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	auto transform = getTransform();
	target.setView(m_view);
	target.draw(m_playerSprite,transform);
}