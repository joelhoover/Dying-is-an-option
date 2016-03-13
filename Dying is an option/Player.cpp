#include "Player.hpp"

Player::Player(Maze& maze) :
	m_maze(&maze)
{
	m_playerTexture.loadFromFile("player.png");
	m_playerSprite.setTexture(m_playerTexture);

	//start off at the top left of the map
	m_position = { 0,0 };

	//origin to center
	auto playerRect = m_playerSprite.getGlobalBounds();
	m_playerSprite.setOrigin(playerRect.width / 2, playerRect.height / 2);

	//set up the initial view
	m_view.setCenter(getPosition());
	m_view.zoom(0.4);
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

	}
	setPosition(m_position.x * nodeSize, m_position.y* nodeSize);
	m_view.setCenter(getPosition());
	return true;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	auto transform = getTransform();
	target.setView(m_view);
	target.draw(m_playerSprite,transform);
}

sf::Vector2u	Player::getMazePosition()
{
	return m_position;
}