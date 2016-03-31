#include "Player.hpp"
#include <fstream>

#include "ResourcePath.h"

Player::Player(Maze& maze) :
	m_maze(&maze)
{
	m_playerTexture.loadFromFile(resourcePath() + "Player.png");
	m_playerSprite.setTexture(m_playerTexture);

	//start off at the top left of the map
	m_position = { 0,0 };

	//origin to center
	auto playerRect = m_playerSprite.getGlobalBounds();
	m_playerSprite.setOrigin(playerRect.width / 2, playerRect.height / 2);

	//set up the initial view
	m_view.setCenter(getPosition());
	m_view.zoom(0.4);

	//set up the score
	std::ifstream saveFile(savePath() + "failure.is.an.option");
	scoreFont.loadFromFile(resourcePath() + "ALGER.TTF");
	scoreText.setFont(scoreFont);
	highScoreText.setFont(scoreFont);
	score = 0;
	scoreText.setString("Moves: " + std::to_string(score));
	if (saveFile.good())
	{
		//read the saved score
		saveFile >> highScore;
		highScoreText.setString("Best: " + std::to_string(highScore));

		//put the current score below the high score
		auto highScoreBounds(highScoreText.getGlobalBounds());
		sf::Vector2f position(highScoreBounds.left, highScoreBounds.top + highScoreBounds.height);
		scoreText.setPosition(position);
	}
	else
	{
		highScore = static_cast<unsigned int>(-1);
	}
}

void	Player::respawn()
{
	m_position = { 0,0 };
	score = 0;
	auto nodeSize(m_maze->getNodeSize());
	setPosition(m_position.x * nodeSize, m_position.y* nodeSize);
	m_view.setCenter(getPosition());
	scoreText.setString("Moves: " + std::to_string(score));
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
	return true;
}

bool	Player::doMove(Direction direction)
{
	auto nodeSize(m_maze->getNodeSize());
	auto availableDirections(m_maze->getAvailableDirections({static_cast<unsigned int>(getPosition().x / nodeSize), static_cast<unsigned int>(getPosition().y / nodeSize)}));

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
	score++;
	scoreText.setString("Moves: " + std::to_string(score));
	if (m_maze->getEndPosition() == m_position)
	{
		//reached the end
		if (highScore > score)
		{
			highScore = score;
			std::ofstream saveFile(savePath() + "failure.is.an.option");
			saveFile << highScore;
			saveFile.close();
			highScoreText.setString("Best: " + std::to_string(highScore));
		}
		return false;
	}

	return true;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	auto transform = getTransform();
	target.setView(m_view);
	target.draw(m_playerSprite,transform);
	target.setView(target.getDefaultView());
	target.draw(scoreText);
	target.draw(highScoreText);
	target.setView(m_view);

}

sf::Vector2u	Player::getMazePosition()
{
	return m_position;
}
