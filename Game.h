#ifndef _GAME_H_
#define _GAME_H_

#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>

#include "Menu.h"

class FieldShape : public sf::RectangleShape
{
public:
	FieldShape() : RectangleShape() {}

	void mouseOver() { this->setFillColor(sf::Color(80,80,150,255)); }
	void revert() { this->setFillColor(sf::Color::Blue); }

	bool isMine() { return _isMine; }

private:
	bool _isMine;
};

class Game
{
public:
	Game(sharedData& data, sfmlRenderWindow& window,sfmlFont& font);

	void Initialize();

	void Run();

	void Destroy(sfmlRenderWindow& window,sfmlFont& font);
private:
	sharedData _gameParameters;

	int _gridFarthestLeft,_gridFarthestRight;
	int _gridTop,_gridBottom;

	sfmlRenderWindow _renderWindow;
	sfmlFont _font;

	std::vector<FieldShape> _grid;
};

#endif