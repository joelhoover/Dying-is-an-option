#include "Maze.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include <chrono>

int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 600), "Dying is an option!");

	Maze maze;
	maze.generate({ 30,30 }, std::chrono::high_resolution_clock::now().time_since_epoch().count());

	Player player(maze);

	std::vector<Enemy>	enemies;
	for (int i = 0; i < 10; i++)
	{
		enemies.emplace_back(Enemy(maze, player));
	}

	while (window.isOpen()) 
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				bool hasKilled(false);
				//first move the player
				player.handleEvent(event);

				//check they haven't moved into an enemy
				for (auto& enemy : enemies)
				{
					if (hasKilled = enemy.hasKilled())
						break;
				}
				
				if (!hasKilled)
				{
					//if the player hasn't died then move all the enemies and check the kill again
					for (auto& enemy : enemies)
					{
						enemy.doMove();
						enemy.hasKilled();
					}
				}
				
				switch (event.key.code)
				{
				case sf::Keyboard::Space:
					maze.generate({ 30,30 }, std::chrono::high_resolution_clock::now().time_since_epoch().count());
					break;
				}
			}
		}

		window.clear();
		window.draw(maze);
		window.draw(player);
		for (auto& enemy : enemies)
		{
			window.draw(enemy);
		}
		window.display();
	}

	return 0;
}