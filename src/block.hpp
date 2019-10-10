#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <SFML/Graphics.hpp>
#include "common.hpp"
#include "grid.hpp"
#include "sfutils.hpp"
#include <vector>

namespace Game
{

class Cell : public sf::Drawable
{
	protected:
		sf::RectangleShape m_shape;
		sf::Vector2i m_index;
		sf::Vector2f m_pos;

	protected:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(m_shape, states);
		}

	public:
		Cell(sf::Vector2i index, sf::Color color) :
			m_shape{sf::Vector2f{g_cellSize, g_cellSize}}, m_index{index}, m_pos{itof(index)}
		{
			m_shape.setPosition(m_pos);
			m_shape.setFillColor(color);
		}
		Cell(int indexX, int indexY, sf::Color color) :
			Cell{sf::Vector2i{indexX, indexY}, color}
		{
		}

		void setIndex(sf::Vector2i index)
		{
			m_index = index;
			m_pos = itof(index);
			m_shape.setPosition(m_pos);
		}
		void setIndex(int indexX, int indexY) { setIndex(sf::Vector2i{indexX, indexY}); }


		sf::Vector2i getIndex() const { return m_index; }
};


class Block : public sf::Drawable
{
	private:
		std::vector<Cell> m_cells;
		int m_rotation;
		sf::Vector2i m_center;
		char m_shape;

		const Grid* m_board;
		sf::Vector2i m_boardOffset;

		bool m_canMoveUp;
		bool m_canMoveDown;
		bool m_canMoveLeft;
		bool m_canMoveRight;
		bool m_canRotateLeft;
		bool m_canRotateRight;

		bool m_hasStopped;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			for(const auto& cell : m_cells)
				target.draw(cell, states);
		}

		void makeI()
		{
			m_center = sf::Vector2i{5, -1};
			for(int x = 3; x < 7; ++x)
				m_cells.emplace_back(x, -1, sf::Color::Cyan);
		}
		void makeJ()
		{
			m_center = sf::Vector2i{4, -1};
			m_cells.emplace_back(3, -1, sf::Color::Blue);
			m_cells.emplace_back(4, -1, sf::Color::Blue);
			m_cells.emplace_back(5, -1, sf::Color::Blue);
			m_cells.emplace_back(3, -2, sf::Color::Blue);
		}
		void makeL()
		{
			m_center = sf::Vector2i{5, -1};
			m_cells.emplace_back(4, -1, sf::Color{255, 153, 0});
			m_cells.emplace_back(5, -1, sf::Color{255, 153, 0});
			m_cells.emplace_back(6, -1, sf::Color{255, 153, 0});
			m_cells.emplace_back(6, -2, sf::Color{255, 153, 0});
		}
		void makeO()
		{
			m_center = sf::Vector2i{5, -1};
			m_cells.emplace_back(4, -1, sf::Color::Yellow);
			m_cells.emplace_back(5, -1, sf::Color::Yellow);
			m_cells.emplace_back(4, -2, sf::Color::Yellow);
			m_cells.emplace_back(5, -2, sf::Color::Yellow);
		}
		void makeS()
		{
			m_center = sf::Vector2i{5, -1};
			m_cells.emplace_back(4, -1, sf::Color::Green);
			m_cells.emplace_back(5, -1, sf::Color::Green);
			m_cells.emplace_back(5, -2, sf::Color::Green);
			m_cells.emplace_back(6, -2, sf::Color::Green);
		}
		void makeT()
		{
			m_center = sf::Vector2i{5, -1};
			m_cells.emplace_back(4, -1, sf::Color{136, 0, 255});
			m_cells.emplace_back(5, -1, sf::Color{136, 0, 255});
			m_cells.emplace_back(6, -1, sf::Color{136, 0, 255});
			m_cells.emplace_back(5, -2, sf::Color{136, 0, 255});
		}
		void makeZ()
		{
			m_center = sf::Vector2i{4, -1};
			m_cells.emplace_back(3, -2, sf::Color::Red);
			m_cells.emplace_back(4, -2, sf::Color::Red);
			m_cells.emplace_back(4, -1, sf::Color::Red);
			m_cells.emplace_back(5, -1, sf::Color::Red);
		}

		void moveUp()
		{
			if(m_canMoveUp)
			{
				--m_center.y;
				for(auto& cell : m_cells)
					cell.setIndex(cell.getIndex().x, cell.getIndex().y - 1);
			}
		}
		void moveDown()
		{
			if(m_canMoveDown)
			{
				++m_center.y;
				for(auto& cell : m_cells)
					cell.setIndex(cell.getIndex().x, cell.getIndex().y + 1);
			}
		}
		void moveLeft()
		{
			if(m_canMoveLeft)
			{
				--m_center.x;
				for(auto& cell : m_cells)
					cell.setIndex(cell.getIndex().x - 1, cell.getIndex().y);
			}
		}
		void moveRight()
		{
			if(m_canMoveRight)
			{
				++m_center.x;
				for(auto& cell : m_cells)
					cell.setIndex(cell.getIndex().x + 1, cell.getIndex().y);
			}
		}

	public:
		Block(char shape, const Grid* board) :
			m_rotation{0}, m_shape{shape},
			m_board{board}, m_boardOffset{board->getBorderLeft()},
			m_canMoveUp{true}, m_canMoveDown{true}, m_canMoveLeft{true}, m_canMoveRight{true},
			m_canRotateLeft{true}, m_canRotateRight{true},
			m_hasStopped{false}
		{
			switch(m_shape)
			{
				case 'I': makeI(); break;
				case 'J': makeJ(); break;
				case 'L': makeL(); break;
				case 'O': makeO(); break;
				case 'S': makeS(); break;
				case 'T': makeT(); break;
				case 'Z': makeZ(); break;
				default: break;
			}

			m_center += m_boardOffset;
			for(auto& cell : m_cells)
				cell.setIndex(cell.getIndex() + m_boardOffset);
		}

		void rotateRight()
		{
			m_rotation += 90;
			for(auto& cell : m_cells)
			{
				sf::Vector2i offset {cell.getIndex() - m_center};
				sf::Vector2f newOffset;
				newOffset.x = ((offset.x * cos(90.0f * g_PI/180.0f)) - (offset.y * sin(90.0f * g_PI/180.0f)));
				newOffset.y = ((offset.x * sin(90.0f * g_PI/180.0f)) + (offset.y * cos(90.0f * g_PI/180.0f)));
				sf::Vector2i newIndex {m_center + sf::Vector2i{static_cast<int>(roundf(newOffset.x)), static_cast<int>(roundf(newOffset.y))}};
				cell.setIndex(newIndex);
			}
		}
		void rotateLeft()
		{
			m_rotation -= 90;
			for(auto& cell : m_cells)
			{
				sf::Vector2i offset {cell.getIndex() - m_center};
				sf::Vector2f newOffset;
				newOffset.x = ((offset.x * cos(-90.0f * g_PI/180.0f)) - (offset.y * sin(-90.0f * g_PI/180.0f)));
				newOffset.y = ((offset.x * sin(-90.0f * g_PI/180.0f)) + (offset.y * cos(-90.0f * g_PI/180.0f)));
				sf::Vector2i newIndex {m_center + sf::Vector2i{static_cast<int>(roundf(newOffset.x)), static_cast<int>(roundf(newOffset.y))}};
				cell.setIndex(newIndex);
			}
		}
		void steerLeft() { moveLeft(); }
		void steerRight() { moveRight(); }
		void steerDown() { moveDown(); }

		bool hasStopped() const { return m_hasStopped; }

		void resetChecks()
		{
			m_canMoveUp = true;
			m_canMoveDown = true;
			m_canMoveLeft = true;
			m_canMoveRight = true;
		}

		void checkWallsMovement()
		{
			for(const auto& cell : m_cells)
			{
				while(cell.getIndex().y + 1 >= m_board->getBorderRight().y)
				{
					m_canMoveDown = false;
					break;
				}
			}
			if(!m_canMoveDown)
				m_hasStopped = true;
			for(const auto& cell : m_cells)
			{
				while(cell.getIndex().x - 1 < m_board->getBorderLeft().x)
				{
					m_canMoveLeft = false;
					break;
				}
			}
			for(const auto& cell : m_cells)
			{
				while(cell.getIndex().x + 1 >= m_board->getBorderRight().x)
				{
					m_canMoveRight = false;
					break;
				}
			}
		}
		void checkCellsMovement(const std::vector<Cell>& otherCells)
		{
			for(const auto& cell : m_cells)
			{
				for(const auto& otherCell : otherCells)
				{
					sf::Vector2i diff {cell.getIndex() - otherCell.getIndex()};
					if(diff.y == 1 && diff.x == 0)
						m_canMoveUp = false;
					if(diff.y == -1 && diff.x == 0)
						m_canMoveDown = false;
					if(diff.x == 1 && diff.y == 0)
						m_canMoveLeft = false;
					if(diff.x == -1 && diff.y == 0)
						m_canMoveRight = false;
				}
			}
			if(!m_canMoveDown)
				m_hasStopped = true;
		}

		void checkWallsRotation()
		{
			for(const auto& cell : m_cells)
			{
				while(cell.getIndex().y >= m_board->getBorderRight().y)
					moveUp();
				while(cell.getIndex().x >= m_board->getBorderRight().x)
					moveLeft();
				while(cell.getIndex().x < m_board->getBorderLeft().x)
					moveRight();
			}
		}
		void checkCellsRotation(const std::vector<Cell>& otherCells)
		{
			for(const auto& cell : m_cells)
			{
				for(const auto& otherCell : otherCells)
				{
					while(cell.getIndex() == otherCell.getIndex())
					{
						if(m_canMoveLeft)
							moveLeft();
						else if(m_canMoveRight)
							moveRight();
						else if(m_canMoveUp)
							moveUp();
						else
							moveDown();
					}
				}
			}

		}

		const std::vector<Cell>& getCells() const { return m_cells; }
		char getShape() const { return m_shape; }
};


}


#endif