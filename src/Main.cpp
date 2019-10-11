#include "SFML/Graphics.hpp"
#include "common.hpp"
#include "block.hpp"
#include "endscreen.hpp"
#include "grid.hpp"
#include "random.hpp"
#include <iostream>
#include <memory>
#include <unordered_map>


const float g_cellSize {36};
const sf::Vector2i g_windowCellCount {21, 24};
const sf::Vector2u g_windowSize {static_cast<unsigned int>(g_cellSize * g_windowCellCount.x), static_cast<unsigned int>(g_cellSize * g_windowCellCount.y)};
sf::Font g_font;

using random = effolkronium::random_static;


sf::Vector2i ftoi(sf::Vector2f pos) { return sf::Vector2i{static_cast<int>(pos.x / g_cellSize), static_cast<int>(pos.y / g_cellSize)}; }
sf::Vector2i ftoi(float posX, float posY) { return ftoi(sf::Vector2f{posX, posY}); }
sf::Vector2f itof(sf::Vector2i index) { return sf::Vector2f{index.x * g_cellSize, index.y * g_cellSize}; }
sf::Vector2f itof(int indexX, int indexY) { return itof(sf::Vector2i{indexX, indexY}); }
sf::Vector2f itoc(sf::Vector2i index) { return sf::Vector2f{(index.x * g_cellSize) + (g_cellSize * 0.5f), (index.y * g_cellSize) + (g_cellSize * 0.5f)}; }
sf::Vector2f itoc(int indexX, int indexY) { return itoc(sf::Vector2i{indexX, indexY}); }

namespace Game
{

class Simulation : public sf::Drawable
{
	private:
		enum class Mode {PLAY, GAMEOVER, NONE};

	private:
		Grid m_placeBoard;
		Grid m_nextBoard;
		std::unique_ptr<Block> m_activeBlock;
		std::unique_ptr<GhostBlock> m_ghostBlock;
		std::unique_ptr<Block> m_nextBlock;

		std::vector<char> m_shapes;

		std::unordered_map<sf::Vector2i, Cell> m_cells;

		int m_lifetime;
		int m_updateFreq;
		int m_maxLevel;

		bool m_upPressed;
		bool m_downPressed;
		bool m_leftPressed;
		bool m_rightPressed;

		sf::RectangleShape m_topCover;

		Mode m_mode;

		EndScreen m_endscreen;
		bool m_restart;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(m_placeBoard, states);
			target.draw(m_nextBoard, states);
			target.draw(*m_activeBlock, states);
			target.draw(*m_ghostBlock, states);
			target.draw(*m_nextBlock, states);

			for(const auto& cell : m_cells)
				target.draw(cell.second, states);

			target.draw(m_topCover, states);

			if(m_mode == Mode::GAMEOVER)
			{
				target.draw(m_endscreen, states);
			}
		}

		void removeLevels()
		{
			for(int baseLevel = 0; baseLevel < m_maxLevel; ++baseLevel)
			{
				sf::Vector2i boardOffset {m_placeBoard.getBorderLeft().x, m_placeBoard.getBorderRight().y - 1};
				int counter {0};

				for(int x = 0; x < m_placeBoard.getSize().x; ++x)
				{
					sf::Vector2i index {boardOffset.x + x, boardOffset.y - baseLevel};
					if(m_cells.find(index) != m_cells.end())
						++counter;
					else
						break;
				}

				if(counter == m_placeBoard.getSize().x)
				{
					for(int x = 0; x < m_placeBoard.getSize().x; ++x)
					{
						sf::Vector2i index {boardOffset.x + x, boardOffset.y - baseLevel};
						m_cells.erase(index);
					}

					for(int y = baseLevel + 1; y < m_maxLevel; ++y)
					{
						for(int x = 0; x < m_placeBoard.getSize().x; ++x)
						{
							sf::Vector2i index {boardOffset.x + x, boardOffset.y - y};
							auto search {m_cells.find(index)};
							if(search != m_cells.end())
							{
								sf::Vector2i newIndex {index.x, index.y + 1};
								search->second.setIndex(newIndex);
								auto nh {m_cells.extract(index)};
								nh.key() = newIndex;
								m_cells.insert(std::move(nh));
							}
						}
					}

					--baseLevel;
					--m_maxLevel;
				}
			}
		}

	public:
		Simulation() :
			m_placeBoard{2, 2, 10, 20}, m_nextBoard{13, 4, 6, 4},
			m_activeBlock{nullptr}, m_ghostBlock{nullptr}, m_nextBlock{nullptr},
			m_shapes{std::initializer_list{'I', 'J', 'L', 'O', 'S', 'T', 'Z'}},
			m_lifetime{0}, m_updateFreq{60}, m_maxLevel{0},
			m_upPressed{false}, m_downPressed{false}, m_leftPressed{false}, m_rightPressed{false},
			m_topCover{itof(g_windowCellCount.x, 2)},
			m_mode{Mode::PLAY}, m_restart{false}
		{
			m_nextBlock = std::make_unique<Block>(*random::get(m_shapes.begin(), m_shapes.end()), &m_nextBoard);
			m_nextBlock->move(1 + m_nextBoard.getBorderLeft().x, 2 + m_nextBoard.getBorderLeft().y);

			m_activeBlock = std::make_unique<Block>(*random::get(m_shapes), &m_placeBoard);
			m_activeBlock->move(3 + m_placeBoard.getBorderLeft().x, -1 + m_placeBoard.getBorderLeft().y);

			m_ghostBlock = std::make_unique<GhostBlock>(m_activeBlock.get());

			m_topCover.setFillColor(sf::Color::Black);
		}

		void update()
		{
			if(m_mode == Mode::PLAY)
			{
				m_activeBlock->resetChecks();
				m_activeBlock->checkWallsMovement();
				m_activeBlock->checkCellsMovement(m_cells);

				if(m_lifetime % m_updateFreq != 0)
				{
					if(m_upPressed)
					{
						m_activeBlock->rotateRight();
						m_activeBlock->checkWallsRotation();
						m_activeBlock->checkCellsRotation(m_cells);
						m_upPressed = false;
					}
					if(m_downPressed)
					{
						m_activeBlock->steerDown();
						m_downPressed = false;
					}
					if(m_leftPressed)
					{
						m_activeBlock->steerLeft();
						m_leftPressed = false;
					}
					if(m_rightPressed)
					{
						m_activeBlock->steerRight();
						m_rightPressed = false;
					}

				}
				else
				{
					m_activeBlock->steerDown();
				}

				m_ghostBlock = std::make_unique<GhostBlock>(m_activeBlock.get());

				for(int i = 0; i < m_placeBoard.getSize().y; ++i)
				{
					m_ghostBlock->resetChecks();
					m_ghostBlock->checkWallsMovement();
					m_ghostBlock->checkCellsMovement(m_cells);

					m_ghostBlock->steerDown();
				}


				if(m_activeBlock->hasStopped())
				{
					for(const auto& cell : m_activeBlock->getCells())
						m_cells.emplace(cell.getIndex(), cell);

					for(const auto& cell : m_activeBlock->getCells())
					{
						int y {m_placeBoard.getBorderRight().y - cell.getIndex().y};
						if(y > m_maxLevel)
							m_maxLevel = y;
					}

					m_activeBlock = std::make_unique<Block>(m_nextBlock->getShape(), &m_placeBoard);
					m_activeBlock->move(3 + m_placeBoard.getBorderLeft().x, -1 + m_placeBoard.getBorderLeft().y);

					m_ghostBlock = std::make_unique<GhostBlock>(m_activeBlock.get());

					m_nextBlock = std::make_unique<Block>(*random::get(m_shapes), &m_nextBoard);
					m_nextBlock->move(1 + m_nextBoard.getBorderLeft().x, 2 + m_nextBoard.getBorderLeft().y);

					removeLevels();

					if(m_maxLevel >= m_placeBoard.getSize().y)
						m_mode = Mode::GAMEOVER;
				}

				++m_lifetime;
			}
		}

		void keyPressed(sf::Keyboard::Key key)
		{
			if(key == sf::Keyboard::Up)
				m_upPressed = true;
			if(key == sf::Keyboard::Down)
				m_downPressed = true;
			if(key == sf::Keyboard::Left)
				m_leftPressed = true;
			if(key == sf::Keyboard::Right)
				m_rightPressed = true;
		}

		void mouseButtonPressed(sf::Vector2f pos)
		{
			if(m_mode == Mode::GAMEOVER)
			{
				if(m_endscreen.buttonPressed(pos))
					m_restart = true;
			}
		}

		bool restart() const { return m_restart; }
};

}

int main()
{
	sf::RenderWindow window {sf::VideoMode{g_windowSize.x, g_windowSize.y}, "Tetris"};
	window.setFramerateLimit(60);

	g_font.loadFromFile("content/consola.ttf");

	std::unique_ptr<Game::Simulation> sim {std::make_unique<Game::Simulation>()};
	sf::Vector2i mousei;
	sf::Vector2f mousef;

	while(window.isOpen())
	{
		mousei = sf::Mouse::getPosition(window);
		mousef = sf::Vector2f{mousei.x * 1.0f, mousei.y * 1.0f};

		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();

			if(event.type == sf::Event::KeyPressed)
			{
				if(event.key.code == sf::Keyboard::Escape)
					window.close();

				sim->keyPressed(event.key.code);
			}

			if(event.type == sf::Event::MouseButtonPressed)
			{
				if(event.mouseButton.button == sf::Mouse::Left)
				{
					sim->mouseButtonPressed(mousef);
					if(sim->restart())
						sim = std::make_unique<Game::Simulation>();
				}
			}
		}

		sim->update();

		window.clear();
		window.draw(*sim);
		window.display();
	}

	return 0;
}
