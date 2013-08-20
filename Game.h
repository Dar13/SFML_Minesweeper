#ifndef _GAME_H_
#define _GAME_H_

#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>

#include "Menu.h"

class FieldShape : public sf::RectangleShape
{
public:
	FieldShape() : RectangleShape(),_isMine(false),_clicked(false) {}

	void mouseOver() { this->setFillColor(sf::Color(80,80,150,255)); }
	void revert() { this->setFillColor(sf::Color::Blue); }

	bool isMine() { return _isMine; }
	void setMine(bool isMine) { _isMine = isMine; }

	bool isClicked() { return _clicked; }
	void setClicked(bool isClicked) { _clicked = isClicked;}

private:
	bool _isMine;
	bool _clicked;
};

class Game
{
public:
	Game(sharedData& data, sfmlRenderWindow& window,sfmlFont& font);

	void Initialize();

	void Run();

	sf::Vector2i getRelativeMouseCoords(int mouseX,int mouseY);
	int getGridIndex(int mouseX,int mouseY);
	int getNumberOfAdjacentBombs(int relMouseX,int relMouseY);
	void clickAdjacentEmpties(sf::Vector2i relativeMouse);
	void clickAdjacentEmpties(int mX,int mY);
	bool isAdjacent(int origin,int test);
	void colorGridSpace(int gridIndex,int numberOfBombs);

	int* _generateAdjacentSpaces(int relMouseX,int relMouseY);

	void Destroy(sfmlRenderWindow& window,sfmlFont& font);
private:
	sharedData _gameParameters;

	int _gridFarthestLeft,_gridFarthestRight;
	int _gridTop,_gridBottom;

	sfmlRenderWindow _renderWindow;
	sfmlFont _font;

	std::vector<FieldShape> _grid;
	sf::Text _gameOverText;
};

#endif