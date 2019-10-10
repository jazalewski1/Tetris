#ifndef BUTTON_HPP
#define BUTTON_HPP


#include "SFML/Graphics.hpp"
#include "common.hpp"
#include "guitext.hpp"

namespace Game
{

class Button : public sf::Drawable
{
	private:
		sf::RectangleShape m_shape;
		sf::Vector2i m_offset;
		sf::Vector2i m_size;
		sf::Vector2f m_center;

		bool m_isActive;
		sf::Color m_activeColor;
		sf::Color m_activeColorDetail;
		sf::Color m_unactiveColor;
		sf::Color m_unactiveColorDetail;

		Gui::Text m_text;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(m_shape, states);
			target.draw(m_text, states);
		}

		void changeColor()
		{
			if(m_isActive)
			{
				m_shape.setFillColor(m_activeColor);
				m_shape.setOutlineColor(m_activeColorDetail);
				m_text.setFillColor(m_activeColorDetail);
			}
			else
			{
				m_shape.setFillColor(m_unactiveColor);
				m_shape.setOutlineColor(m_unactiveColorDetail);
				m_text.setFillColor(m_unactiveColorDetail);
			}
		}

	public:
		Button(sf::Vector2i offset, sf::Vector2i size, bool active = false) :
			m_shape{itof(size)}, m_offset{offset}, m_size{size},
			m_isActive{active},
			m_activeColor{83, 158, 24}, m_activeColorDetail{37, 71, 9},
			m_unactiveColor{196, 196, 196}, m_unactiveColorDetail{94, 94, 94},
			m_text{itoc(offset), "CONFIRM"}
		{
			m_shape.setPosition(itof(m_offset));
			m_shape.setOutlineThickness(3.0f);
			changeColor();

			m_center = itof(m_offset) + (m_shape.getSize() * 0.5f);

			m_text.setFont(g_font);
			m_text.setCharacterSize(32);
			m_text.alignToCenter();
			m_text.setPosition(m_center);
		}
		Button(int offsetX, int offsetY, int sizeX, int sizeY, bool active = false) :
			Button{sf::Vector2i{offsetX, offsetY}, sf::Vector2i{sizeX, sizeY}, active}
		{
		}


		bool pressed(sf::Vector2f mouse) { return m_isActive && m_shape.getGlobalBounds().contains(mouse); }

		void setActive(bool active)
		{
			m_isActive = active;
			changeColor();
		}
		void setText(const char* text) { m_text.setString(text); }

		bool isActive() const { return m_isActive; }
};

}


#endif