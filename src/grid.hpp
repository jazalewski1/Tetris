#ifndef GRID_HPP
#define GRID_HPP


#include "SFML/Graphics.hpp"
#include "common.hpp"
#include <vector>


namespace Game
{

class Grid : public sf::Drawable
{
	private:
		std::vector<sf::RectangleShape> m_arr;
		sf::Vector2i m_borderLeft;
		sf::Vector2i m_borderRight;

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			for(const auto& rect : m_arr)
				target.draw(rect, states);
		}

	public:
		Grid(sf::Vector2i offset, sf::Vector2i size) :
			m_borderLeft{offset}, m_borderRight{offset + size}
		{
			sf::RectangleShape rect {sf::Vector2f{g_cellSize, g_cellSize}};
			rect.setOutlineColor(sf::Color{255, 255, 255, 30});
			rect.setOutlineThickness(1.0f);
			rect.setFillColor(sf::Color::Transparent);

			for(int y = 0; y < size.y; ++y)
			{
				for(int x = 0; x < size.x; ++x)
				{
					rect.setPosition(itof(offset + sf::Vector2{x, y}));
					m_arr.push_back(rect);
				}
			}
		}
		Grid(int offsetX, int offsetY, int sizeX, int sizeY) :
			Grid{sf::Vector2i{offsetX, offsetY}, sf::Vector2i{sizeX, sizeY}}
		{
		}

		sf::Vector2i getBorderRight() const { return m_borderRight; }
		sf::Vector2i getBorderLeft() const { return m_borderLeft; }
};

}


#endif