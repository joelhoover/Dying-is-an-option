#include "Maze.hpp"
#include "Player.hpp"
#include <chrono>

int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 600), "Dying is an option!");

	Maze maze;
	maze.generate({ 50,50 }, std::chrono::high_resolution_clock::now().time_since_epoch().count());

	Player player(maze);

	while (window.isOpen()) 
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				player.handleEvent(event);
				switch (event.key.code)
				{
				case sf::Keyboard::Space:
					maze.generate({ 50,50 }, std::chrono::high_resolution_clock::now().time_since_epoch().count());
					break;
				}
			}
		}

		window.clear();
		window.draw(maze);
		window.draw(player);
		window.display();
	}

	return 0;
}