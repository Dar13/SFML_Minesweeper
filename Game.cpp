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
	sf::RectangleShape shape;
	for(int i = 0; i < _gameParameters.fieldWidth; i++)
	{
		for(int j = 0; j < _gameParameters.fieldHeight; j++)
		{
			//sf::RectangleShape shape;
			shape.setFillColor(sf::Color::Blue);
			shape.setOutlineColor(sf::Color::White);
			shape.setOutlineThickness(1.0f);
			shape.setSize(sf::Vector2f(15,15));
			shape.setOrigin(-1,-1);
			shape.setPosition(offsetX + i * 17,offsetY + j * 17);
			_grid.push_back(shape);
		}
	}
	
}

void Game::Run()
{
	bool gameRun = true;
	sf::Event evt;
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
				mX = mX - _grid[0].getPosition().x;
				mY = mY - _grid[0].getPosition().y;
			}
		}

		_renderWindow->clear();
		std::for_each(_grid.begin(),_grid.end(),[&](sf::RectangleShape& shape)
		{
			_renderWindow->draw(shape);
		});
		_renderWindow->display();
	}
}

void Game::Destroy(sfmlRenderWindow& window,sfmlFont& font)
{
	window = std::move(_renderWindow);
	font = std::move(_font);
}