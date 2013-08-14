#ifndef _GAME_H_
#define _GAME_H_

#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>

#include "Menu.h"

class Game
{
public:
	Game(sharedData& data, sfmlRenderWindow& window,sfmlFont& font);

	void Initialize();

	void Run();

	void Destroy(sfmlRenderWindow& window,sfmlFont& font);
private:
	sharedData _gameParameters;

	sfmlRenderWindow _renderWindow;
	sfmlFont _font;

	std::vector<sf::RectangleShape> _grid;
};

#endif