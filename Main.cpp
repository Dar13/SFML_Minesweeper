#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\Audio.hpp>

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include <iostream>
#include <fstream>

#include <string>
#include <sstream>
#include <memory>

#include "Misc.h"
#include "Menu.h"
#include "Game.h"

#include <cstdlib>

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
	sfmlRenderWindow renderWindow;
	renderWindow.reset(new sf::RenderWindow(sf::VideoMode(800,600),
											"Minesweeper Clone",
											sf::Style::Titlebar | sf::Style::Close,
											settings));

	sfmlFont font(new sf::Font());
	if(!font->loadFromFile("sego_ui.ttf"))
	{
		exit(1); //error, but no abort.
	}

	Menu menu(renderWindow,font);
	sharedData data = menu.Run();
	menu.Destroy(renderWindow,font);

	Game game(data,renderWindow,font);
	game.Run();
	game.Destroy(renderWindow,font);

	renderWindow->close();

	return 0;
};