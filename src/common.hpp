#include "SFML/Graphics.hpp"

extern const float g_cellSize;
extern const sf::Vector2i g_windowCellCount;
extern const sf::Vector2u g_windowSize;
extern const float g_PI;

extern sf::Vector2i ftoi(sf::Vector2f pos);
extern sf::Vector2i ftoi(float posX, float posY);
extern sf::Vector2f itof(sf::Vector2i index);
extern sf::Vector2f itof(int indexX, int indexY);
extern sf::Vector2f itoc(sf::Vector2i index);
extern sf::Vector2f itoc(int indexX, int indexY);
