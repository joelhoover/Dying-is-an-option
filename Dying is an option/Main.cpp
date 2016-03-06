#include "Maze.hpp"
#include <chrono>

int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 600), "Dying is an option!");

	Maze maze;
	maze.generate({ 50,50 }, std::chrono::high_resolution_clock::now().time_since_epoch().count());
	maze.setPosition(10, 10);

	while (window.isOpen()) 
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type = sf::Event::KeyPressed)
			{
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
		window.display();
	}

	return 0;
}