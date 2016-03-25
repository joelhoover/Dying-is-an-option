#include "Maze.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "EndScreen.hpp"
#include <chrono>

int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 600), "Dying is an option!");
	window.setFramerateLimit(60);

	Maze maze({ 30,30 });
	maze.generate();

	Player player(maze);

	auto won(false);
	EndScreen endScreen(window.getSize(),player);

	std::vector<Enemy>	enemies;
	for (int i = 0; i < 30; i++)
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
				if (!won)
				{
					bool hasKilled(false);
					//first move the player
					if (!player.handleEvent(event))
					{
						won = true;
					}

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
				}
			}
		}

		window.clear();
		if (won)
		{
			window.draw(endScreen);
			if (endScreen.done())
			{
				//done, start again
				player.respawn();
				maze.regenerate(true);
				won = false;
			}
		}
		else
		{
			window.draw(maze);
			window.draw(player);
			for (auto& enemy : enemies)
			{
				window.draw(enemy);
			}
		}
		window.display();
	}

	return 0;
}