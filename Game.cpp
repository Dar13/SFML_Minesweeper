#include "Game.h"

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
			_grid.push_back(shape);
		}
	}
	
	_gridFarthestLeft = _grid.begin()->getPosition().x;
	_gridFarthestRight = std::prev(_grid.end())->getGlobalBounds().left + std::prev(_grid.end())->getGlobalBounds().width;
	_gridTop = _grid.begin()->getPosition().y;
	_gridBottom = std::prev(_grid.end())->getGlobalBounds().top + std::prev(_grid.end())->getGlobalBounds().height;
}

int Game::getGridIndex(int mouseX,int mouseY)
{
	int o_mX,o_mY;
	o_mX = mouseX;
	o_mY = mouseY;

	int finalGridCoord = -1;

	if(o_mX < _gridFarthestLeft || o_mX > _gridFarthestRight ||
		o_mY < _gridTop || o_mY > _gridBottom)
	{
		return finalGridCoord;
	}
	else
	{
		mouseX = ceil((mouseX - _gridFarthestLeft) / 17);
		mouseY = ceil((mouseY - _gridTop) / 17);

		finalGridCoord = (mouseX * _gameParameters.fieldHeight) + mouseY;
	}

	return finalGridCoord;
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
				if(gridIndex != -1)
				{
					_grid[gridIndex].mouseOver();
				}
			}
		}

		_renderWindow->clear();
		index = 0;
		std::for_each(_grid.begin(),_grid.end(),[&](FieldShape& shape)
		{
			_renderWindow->draw(shape);
			if(index != gridIndex)
			{
				shape.revert();
			}
			index++;
		});
		_renderWindow->display();
	}
}

void Game::Destroy(sfmlRenderWindow& window,sfmlFont& font)
{
	window = std::move(_renderWindow);
	font = std::move(_font);
}