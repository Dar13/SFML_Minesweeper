#ifndef _MENU_H_
#define _MENU_H_

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <SFML\System.hpp>

#include "Misc.h"
#include <map>
#include <functional>

#define DEFAULT_MINECOUNT 50
#define DEFAULT_FIELDWIDTH 16
#define DEFAULT_FIELDHEIGHT 16

struct sharedData
{
	int mineCount;
	int fieldHeight;
	int fieldWidth;
};

struct Button
{
	sf::Text text;
	sf::RectangleShape background;
	std::function<void(sfmlRenderWindow& window,const sf::Event& event)> handleEvent;
};

struct NumberPicker
{
	enum type
	{
		NONE = 0,
		MINE_COUNT = 1,
		FIELD_HEIGHT,
		FIELD_WIDTH,
		TYPE_MAX
	};
	sf::Text description;
	sf::RectangleShape upArrow;
	sf::RectangleShape downArrow;
	sf::RectangleShape textBox;
	sf::Text numberText;
	int number;
};

typedef std::map<NumberPicker::type,NumberPicker> NumberPickerMap;

class Menu
{
public:
	Menu(sfmlRenderWindow& window,sfmlFont& font);

	void Initialize();

	sharedData Run();

	void Destroy(sfmlRenderWindow& window,sfmlFont& font);

	//number pickers
	void setupNumberPicker(NumberPicker::type type,
						   NumberPicker& picker,
						   std::string desc,
						   int startValue,
						   sf::Vector2f position,
						   sf::Font& font);
	static std::pair<bool,NumberPicker::type> doesEventAffectNumberPicker(NumberPickerMap& picker,
																		  const sf::Event& event);
	static void handleEvent(NumberPicker& picker,const sf::Event& event);
	static void drawNumberPickers(sfmlRenderWindow& win,NumberPickerMap& pickers);

	//buttons
	void setupButtons(Button& button,
					  std::string text,
					  sf::Vector2f position,
					  sf::Font& font);
	static void drawButton(sfmlRenderWindow& window, Button& button);
private:
	sfmlRenderWindow _renderWindow;
	sfmlFont _font;

	Button _exitButton;

	NumberPicker _mineCountUI;
	NumberPicker _fieldHeightUI;
	NumberPicker _fieldWidthUI;
	NumberPickerMap _numberPickers;
	
};

#endif