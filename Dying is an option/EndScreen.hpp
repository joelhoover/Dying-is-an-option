#pragma once

#include <SFML/Graphics.hpp>

class Player;

class EndScreen : public sf::Drawable
{
public:
	EndScreen(sf::Vector2u windowSize, Player& player);
	~EndScreen();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	bool	done();

private:
	sf::RectangleShape	blackout;
	sf::Font			endFont;
	sf::Text			endText;
	sf::Text			highScoreText;
	sf::Text			retryText;
	Player*				m_player;
};

