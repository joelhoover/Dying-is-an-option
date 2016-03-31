#include "EndScreen.hpp"
#include "Player.hpp"

#include "ResourcePath.h"


EndScreen::EndScreen(sf::Vector2u windowSize, Player& player) :
	m_player(&player)

{
	blackout = sf::RectangleShape(static_cast<sf::Vector2f>(windowSize));
	blackout.setFillColor(sf::Color::Black);

	endFont.loadFromFile(resourcePath() + "ALGER.TTF");
	endText.setFont(endFont);
	highScoreText.setFont(endFont);
	retryText.setFont(endFont);
	sf::Vector2i screenCenter(windowSize.x / 2, windowSize.y / 2);
	endText.setPosition(static_cast<sf::Vector2f>(screenCenter));
	auto bounds(endText.getGlobalBounds());
	highScoreText.setPosition(screenCenter.x,bounds.top + bounds.height);
	bounds = highScoreText.getGlobalBounds();
	retryText.setPosition(screenCenter.x,bounds.top + bounds.height);
}

EndScreen::~EndScreen()
{
}

void EndScreen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.setView(target.getDefaultView());
	target.draw(blackout);
	target.draw(endText);
	target.draw(highScoreText);
	target.draw(retryText);
}

bool	EndScreen::done()
{
	endText.setString("Score: " + std::to_string(m_player->score));
	highScoreText.setString("Best Score: " + std::to_string(m_player->highScore));
	retryText.setString("Press space to retry");
	auto endTextBounds(endText.getGlobalBounds());
	endText.setOrigin(endTextBounds.width / 2,0);
	auto highScoreBounds(highScoreText.getGlobalBounds());
	highScoreText.setPosition(endText.getPosition().x,endTextBounds.top + endTextBounds.height);
	highScoreText.setOrigin(highScoreBounds.width / 2, 0);
	auto retryTextBounds(retryText.getGlobalBounds());
	retryText.setOrigin(retryTextBounds.width / 2, 0);
	retryText.setPosition(highScoreText.getPosition().x,highScoreBounds.top + highScoreBounds.height);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		return true;
}
