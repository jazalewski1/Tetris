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

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			for(const auto& rect : m_arr)
				target.draw(rect, states);
		}

	public:
		Grid()
		{
			sf::RectangleShape rect {sf::Vector2f{g_cellSize, g_cellSize}};
			rect.setOutlineColor(sf::Color{255, 255, 255, 30});
			rect.setOutlineThickness(1.0f);
			rect.setFillColor(sf::Color::Transparent);

			for(int y = 0; y < g_gridSize.y; ++y)
			{
				for(int x = 0; x < g_gridSize.x; ++x)
				{
					rect.setPosition(itof(x, y));
					m_arr.push_back(rect);
				}
			}
		}
};

}


#endif