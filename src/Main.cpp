#include "SFML/Graphics.hpp"
#include "common.hpp"
#include "grid.hpp"
#include <iostream>


const float g_cellSize {32};
const sf::Vector2i g_gridSize {16, 24};
const sf::Vector2u g_windowSize {static_cast<unsigned int>(g_cellSize * g_gridSize.x), static_cast<unsigned int>(g_cellSize * g_gridSize.y)};


sf::Vector2i ftoi(sf::Vector2f pos) { return sf::Vector2i{static_cast<int>(pos.x / g_cellSize), static_cast<int>(pos.y / g_cellSize)}; }
sf::Vector2i ftoi(float posX, float posY) { return ftoi(sf::Vector2f{posX, posY}); }
sf::Vector2f itof(sf::Vector2i index) { return sf::Vector2f{index.x * g_cellSize, index.y * g_cellSize}; }
sf::Vector2f itof(int indexX, int indexY) { return itof(sf::Vector2i{indexX, indexY}); }
sf::Vector2f itoc(sf::Vector2i index) { return sf::Vector2f{(index.x * g_cellSize) + (g_cellSize * 0.5f), (index.y * g_cellSize) + (g_cellSize * 0.5f)}; }
sf::Vector2f itoc(int indexX, int indexY) { return itoc(sf::Vector2i{indexX, indexY}); }


int main()
{
	sf::RenderWindow window {sf::VideoMode{g_windowSize.x, g_windowSize.y}, "Tetris"};
	window.setFramerateLimit(60);

	Game::Grid grid {};

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(grid);
		window.display();
	}

	return 0;
}
