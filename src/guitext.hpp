#ifndef GUITEXT_HPP
#define GUITEXT_HPP


#include "SFML/Graphics.hpp"
#include <sstream>
#include <string>


namespace Gui
{
// Text class best working as one-line string
class Text : public sf::Drawable
{
	protected:
		sf::Text m_text;
		std::stringstream m_stream;
		sf::Vector2f m_pos;
		sf::Vector2f m_size;
		sf::Vector2f m_originOffset;
		sf::Vector2f m_alignOffset;
		sf::Vector2f m_alignFactor;
		sf::Vector2f m_sumOffset;

	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(m_text, states);
		}

		virtual void adjust()
		{
			m_size = sf::Vector2f{m_text.getLocalBounds().width, m_text.getLocalBounds().height};
			m_alignOffset = sf::Vector2f{m_size.x * m_alignFactor.x, m_size.y * m_alignFactor.y};
			m_originOffset = sf::Vector2f{m_text.getLocalBounds().left, m_text.getLocalBounds().top};
			m_text.setOrigin(m_sumOffset + m_alignOffset + m_originOffset);
		}

	public:
		Text(sf::Vector2f pos, std::string str = "") :
			m_pos{pos}, m_size{0.0f, 0.0f}, m_originOffset{0.0f, 0.0f},
			m_alignOffset{0.0f, 0.0f}, m_alignFactor{0.0f, 0.0f},
			m_sumOffset{0.0f, 0.0f}
		{
			m_text.setCharacterSize(24);
			m_text.setFillColor(sf::Color::Red);
			m_text.setString(str);

			adjust();
			m_text.setPosition(m_pos);
		}
		Text(float posX, float posY, std::string str = "") :
			Text{sf::Vector2f{posX, posY}, str}
		{
		}
		Text() :
			Text{0.0f, 0.0f, ""}
		{
		}

		template <typename T>
		void append(T arg)
		{
			m_stream << arg;
			m_text.setString(m_text.getString().toAnsiString() + m_stream.str());
			adjust();

			m_stream.str(std::string{});
			m_stream.clear();
		}
		template <typename T, typename... Ts>
		void append(T arg, Ts... args)
		{
			m_stream << arg;
			append(args...);
		}

		template <typename T>
		void setString(T arg)
		{
			m_stream << arg;
			if(m_stream.str() != m_text.getString().toAnsiString())
			{
				m_text.setString(m_stream.str());
				adjust();
			}

			m_stream.str(std::string{});
			m_stream.clear();
		}
		template <typename T, typename... Ts>
		void setString(T arg, Ts... args)
		{
			m_stream << arg;
			setString(args...);
		}

		virtual void setPosition(sf::Vector2f pos)
		{
			m_pos = pos;
			m_text.setPosition(m_pos);
		}
		virtual void setPosition(float posX, float posY) { setPosition(sf::Vector2f{posX, posY}); }

		virtual void setFillColor(sf::Color color) { m_text.setFillColor(color); }
		virtual void setOutlineColor(sf::Color color) { m_text.setOutlineColor(color); }
		virtual void setOutlineThickness(float value) { m_text.setOutlineThickness(value); }
		virtual void setFont(const sf::Font& font) { m_text.setFont(font); }

		virtual void setCharacterSize(unsigned int size) { m_text.setCharacterSize(size); adjust(); }
		virtual void setLineSpacing(float value) { m_text.setLineSpacing(value); adjust(); }
		virtual void setLetterSpacing(float value) { m_text.setLetterSpacing(value); adjust(); }


		virtual void alignToCenterX() { m_sumOffset = sf::Vector2f{0.0f, 0.0f}; m_alignFactor.x = 0.5f; adjust(); }
		virtual void alignToCenterY() { m_sumOffset = sf::Vector2f{0.0f, 0.0f}; m_alignFactor.y = 0.5f; adjust(); }
		virtual void alignToCenter() { m_sumOffset = sf::Vector2f{0.0f, 0.0f}; m_alignFactor = sf::Vector2f{0.5f, 0.5f}; adjust(); }

		virtual void alignToRight() { m_sumOffset = sf::Vector2f{0.0f, 0.0f}; m_alignFactor.x = 1.0f; adjust(); }
		virtual void alignToLeft() { m_sumOffset = sf::Vector2f{0.0f, 0.0f}; m_alignFactor.x = 0.0f; adjust(); }

		virtual void alignToTop() { m_sumOffset = sf::Vector2f{0.0f, 0.0f}; m_alignFactor.y = 0.0f; adjust(); }
		virtual void alignToBottom() { m_sumOffset = sf::Vector2f{0.0f, 0.0f}; m_alignFactor.y = 1.0f; adjust(); }
		virtual void setAlignement(sf::Vector2f offset)
		{
			m_alignFactor = sf::Vector2f{0.0f, 0.0f};
			m_sumOffset = offset;
			adjust();
		}
		virtual void setAlignement(float offsetX, float offsetY) { setAlignement(sf::Vector2f{offsetX, offsetY}); }


		virtual sf::Vector2f getAlignement() const { return m_alignOffset; }
		virtual sf::Vector2f getPosition() const { return m_pos; }
		virtual sf::Vector2f getSize() const { return m_size; }
};
}


#endif