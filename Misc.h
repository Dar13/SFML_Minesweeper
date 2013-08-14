#include <memory>

#include <SFML/Graphics.hpp>

#include <Windows.h>

#ifndef _MISC_H_
#define _MISC_H_

typedef std::unique_ptr<sf::RenderWindow> sfmlRenderWindow;
typedef std::unique_ptr<sf::Font> sfmlFont;

#endif