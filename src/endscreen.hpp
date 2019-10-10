#ifndef ENDSCREEN_HPP
#define ENDSCREEN_HPP

#include "SFML/Graphics.hpp"
#include "button.hpp"
#include "common.hpp"
#include "guitext.hpp"

namespace Game
{

class EndScreen : public sf::Drawable
{
	private:
		sf::RectangleShape m_background;
		Gui::Text m_text;
		Game::Button m_button;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(m_background, states);
			target.draw(m_text, states);
			target.draw(m_button, states);
		}

	public:
		EndScreen() :
			m_background{sf::Vector2f{itof(sf::Vector2i{g_windowCellCount.x, 10})}},
			m_text{},
			m_button{6, 13, 8, 3, true}
		{
			m_background.setFillColor(sf::Color{240, 235, 189, 220});
			m_background.setPosition(itof(sf::Vector2i{0, 8}));

			m_text.setFont(g_font);
			m_text.setPosition(g_windowSize.x * 0.5f, 360.0f);
			m_text.setFillColor(sf::Color::Black);
			m_text.setCharacterSize(72);
			m_text.alignToCenterX();
			m_text.setString("GAME OVER");

			m_button.setText("RESTART");
		}

		bool buttonPressed(sf::Vector2f mouse) { return m_button.pressed(mouse); }
};

}

#endif