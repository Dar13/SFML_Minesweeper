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

void Game::Run()
{
	bool gameRun = true;
	sf::Event evt;

	int finalGridCoord = 0;
	int index = 0;

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
				int mX = evt.mouseMove.x,mY = evt.mouseMove.y;
				int o_mX = mX,o_mY = mY;
				mX = mX - _gridFarthestLeft;
				mY = mY - _gridTop;

				mX /= 17;
				mY /= 17;

				mX = ceil(mX);
				mY = ceil(mY);
				
				if(o_mX > _gridFarthestLeft && o_mX < _gridFarthestRight
					&& o_mY > _gridTop && o_mY < _gridBottom)
				{
					finalGridCoord = (mX * _gameParameters.fieldHeight) + mY;
					_grid[finalGridCoord].mouseOver();
				}
				else
				{
					finalGridCoord = -1;
				}
			}
		}

		_renderWindow->clear();
		index = 0;
		std::for_each(_grid.begin(),_grid.end(),[&](FieldShape& shape)
		{
			_renderWindow->draw(shape);
			if(index != finalGridCoord)
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