#include "Game.h"

#include <thread>
#include <chrono>
#include <random>

Game::Game(sharedData& data, sfmlRenderWindow& window,sfmlFont& font)
	: _renderWindow(std::move(window)),
	_font(std::move(font)),
	_gameParameters(data)
{
	Initialize();
}

void Game::Initialize()
{
	//create playing field based on data.
	sf::Vector2u size((_gameParameters.fieldWidth * 13 + 200),(_gameParameters.fieldHeight * 13 + 200));
	_renderWindow->setSize(size);
	sf::Vector2u v = _renderWindow->getSize();
	_renderWindow->setView(sf::View(sf::Vector2f(size.x / 2,size.y / 2),
									sf::Vector2f(size.x,size.y)));

	int offsetX = (size.x - _gameParameters.fieldWidth * 17) / 2;
	int offsetY = 75;

	std::default_random_engine engine;
	std::uniform_int_distribution<int> dist(0,(_gameParameters.fieldHeight * _gameParameters.fieldWidth));
	std::vector<int> mineIndices;
	for(int i = 0; i < _gameParameters.mineCount; ++i)
	{
		mineIndices.push_back(dist(engine));
	}

	FieldShape shape;
	for(int i = 0; i < _gameParameters.fieldWidth; i++)
	{
		for(int j = 0; j < _gameParameters.fieldHeight; j++)
		{
			shape.setFillColor(sf::Color::Blue);
			shape.setOutlineColor(sf::Color::White);
			shape.setOutlineThickness(1.0f);
			shape.setSize(sf::Vector2f(15,15));
			shape.setOrigin(-1,-1);
			shape.setPosition(offsetX + i * 17,offsetY + j * 17);
			if(std::find(mineIndices.begin(),mineIndices.end(),(j*i)) != mineIndices.end())
			{
				shape.setMine(true);
			}
			else
			{
				shape.setMine(false);
			}
			_grid.push_back(shape);
		}
	}

	_gameOverText.setFont(*_font);
	_gameOverText.setCharacterSize(20);
	_gameOverText.setColor(sf::Color::White);
	_gameOverText.setString("Game Over");
	_gameOverText.setPosition((_renderWindow->getSize().x / 2) - (_gameOverText.getLocalBounds().width / 2), 10);
	
	_gridFarthestLeft = _grid.begin()->getPosition().x;
	_gridFarthestRight = std::prev(_grid.end())->getGlobalBounds().left + std::prev(_grid.end())->getGlobalBounds().width;
	_gridTop = _grid.begin()->getPosition().y;
	_gridBottom = std::prev(_grid.end())->getGlobalBounds().top + std::prev(_grid.end())->getGlobalBounds().height;
}

sf::Vector2i Game::getRelativeMouseCoords(int mouseX,int mouseY)
{
	if(mouseX < _gridFarthestLeft || mouseX > _gridFarthestRight ||
		mouseY < _gridTop || mouseY > _gridBottom)
	{
		return sf::Vector2i(-1,-1);
	}
	else
	{
		return sf::Vector2i(ceil((mouseX - _gridFarthestLeft) / 17),ceil(mouseY - _gridTop) / 17);
	}
}

int Game::getGridIndex(int mouseX,int mouseY)
{
	int o_mX,o_mY;
	o_mX = mouseX;
	o_mY = mouseY;

	int finalGridCoord = -1;

	auto vec = getRelativeMouseCoords(mouseX,mouseY);
	if(vec.x != -1 && vec.y != -1)
	{
		finalGridCoord = (vec.x * _gameParameters.fieldHeight) + vec.y;
	}

	return finalGridCoord;
}

int Game::getNumberOfAdjacentBombs(int relMouseX,int relMouseY)
{
	const int TOP = 0;
	const int BTM = 1;
	const int LFT = 2;
	const int RGT = 3;
	const int TOP_LFT = 4;
	const int TOP_RGT = 5;
	const int BTM_LFT = 6;
	const int BTM_RGT = 7;

	bool adjacentBombs[7];

	int adjacentSpaces[8];
	adjacentSpaces[TOP] = ((relMouseX * _gameParameters.fieldHeight) + (relMouseY - 1));
	adjacentSpaces[BTM] = ((relMouseX *  _gameParameters.fieldHeight) +  (relMouseY + 1));
	adjacentSpaces[RGT] = (((relMouseX + 1) *  _gameParameters.fieldHeight) + relMouseY);
	adjacentSpaces[LFT] = (((relMouseX - 1) *  _gameParameters.fieldHeight) + relMouseY);
	adjacentSpaces[TOP_LFT] = (((relMouseX - 1) *  _gameParameters.fieldHeight) + (relMouseY - 1));
	adjacentSpaces[TOP_RGT] = (((relMouseX + 1) *  _gameParameters.fieldHeight) + (relMouseY - 1));
	adjacentSpaces[BTM_LFT] = (((relMouseX - 1) *  _gameParameters.fieldHeight) + (relMouseY + 1));
	adjacentSpaces[BTM_RGT] = (((relMouseX + 1) *  _gameParameters.fieldHeight) + (relMouseY + 1));

	//check edge cases
	for(int i = TOP; i < 8; i++)
	{
		if( adjacentSpaces[i] < 0 || adjacentSpaces[i] >= (_gameParameters.fieldHeight * _gameParameters.fieldWidth))
		{
			adjacentSpaces[i] = -1;
		}
	}

	//check mines
	int sum = 0;
	for(int i = 0; i < 8; i++)
	{
		if(adjacentSpaces[i] == -1) { continue; }

		//adjacentBombs[i] = _grid[adjacentSpaces[i]].isMine();
		if(_grid[adjacentSpaces[i]].isMine())
		{
			sum++;
		}
	}

	return sum;
}

void Game::Run()
{
	bool gameRun = true;
	sf::Event evt;

	int index = 0;
	int gridIndex = 0;

	while(gameRun)
	{
		while(_renderWindow->pollEvent(evt))
		{
			if(evt.type == sf::Event::Closed)
			{
				gameRun = false;
			}

			if(evt.type == sf::Event::MouseMoved)
			{
				//transform mouse coordinates to coordinates relative to the grid.
				gridIndex = getGridIndex(evt.mouseMove.x,evt.mouseMove.y);
				if(gridIndex != -1 && !_grid[gridIndex].isClicked())
				{
					_grid[gridIndex].mouseOver();
				}
			}

			if(evt.type == sf::Event::MouseButtonReleased)
			{
				gridIndex = getGridIndex(evt.mouseButton.x,evt.mouseButton.y);
				if(gridIndex >= 0)
				{
					if(evt.mouseButton.button == sf::Mouse::Button::Left)
					{
						if(_grid[gridIndex].isMine())
						{
							//game over.
							_grid[gridIndex].setFillColor(sf::Color::Red);
							//gameRun = false;
						}
						else
						{
							//check adjacent tiles for bombs.
							auto v = getRelativeMouseCoords(evt.mouseButton.x,evt.mouseButton.y);
							//no need to check for valid input. If it gets this far, the data is good.
							int numOfAdjacentBombs = getNumberOfAdjacentBombs(v.x,v.y);

							switch(numOfAdjacentBombs)
							{
							case 1:
								_grid[gridIndex].setFillColor(sf::Color::Green);
								break;
							case 2:
								_grid[gridIndex].setFillColor(sf::Color::Cyan);
								break;
							case 3:
								_grid[gridIndex].setFillColor(sf::Color::Magenta);
								break;
							case 4:
								_grid[gridIndex].setFillColor(sf::Color::Yellow);
								break;
							case 5:
								_grid[gridIndex].setFillColor(sf::Color::Black);
								break;
							}

							_grid[gridIndex].setClicked(true);
						}
					}
				}
			}
		}

		_renderWindow->clear();
		index = 0;
		std::for_each(_grid.begin(),_grid.end(),[&](FieldShape& shape)
		{
			_renderWindow->draw(shape);
			if(index != gridIndex && !shape.isClicked())
			{
				shape.revert();
			}
			index++;
		});
		if(!gameRun) { _renderWindow->draw(_gameOverText); }
		_renderWindow->display();

		if(!gameRun)
		{
			std::this_thread::sleep_for(std::chrono::seconds(3));
		}
	}
}

void Game::Destroy(sfmlRenderWindow& window,sfmlFont& font)
{
	//Give back all resources 'taken' by the Game class.
	window = std::move(_renderWindow);
	font = std::move(_font);
}