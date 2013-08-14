#include "Menu.h"

#include <sstream>

Menu::Menu(sfmlRenderWindow& window,sfmlFont& font)
	: _renderWindow(std::move(window)),
	_font(std::move(font))
{
	//will automatically initialize this class.
	Initialize();
}

void Menu::Initialize()
{
	//setup render window
	_renderWindow->setSize(sf::Vector2u(400,400));
	_renderWindow->setView(sf::View(sf::Vector2f(200,200),sf::Vector2f(400,400)));
	_renderWindow->setFramerateLimit(30);

	//setup UI elements.
	setupNumberPicker(NumberPicker::MINE_COUNT,_mineCountUI,"Number of Mines",DEFAULT_MINECOUNT,sf::Vector2f(200,90),*_font);
	setupNumberPicker(NumberPicker::FIELD_HEIGHT,_fieldHeightUI,"Minefield Height",DEFAULT_FIELDHEIGHT,sf::Vector2f(110,260),*_font);
	setupNumberPicker(NumberPicker::FIELD_WIDTH,_fieldWidthUI,"Minefield Width",DEFAULT_FIELDWIDTH,sf::Vector2f(290,260),*_font);

	setupButtons(_exitButton,"Play Game",sf::Vector2f(200,350),*_font);
}

sharedData Menu::Run()
{
	sharedData data;
	data.fieldHeight = 100;
	data.fieldWidth = 100;
	data.mineCount = 10;

	bool menuOpen = true;
	sf::Event evt;
	while(menuOpen)
	{
		while(_renderWindow->pollEvent(evt))
		{
			if(evt.type == sf::Event::Closed)
			{
				menuOpen = false;
			}

			if(evt.type == sf::Event::MouseButtonReleased)
			{
				auto ret = doesEventAffectNumberPicker(_numberPickers,evt);
				if(ret.first)
				{
					handleEvent(_numberPickers[ret.second],evt);
				}

				_exitButton.handleEvent(_renderWindow,evt);
			}
		}
		
		_renderWindow->clear();
		drawNumberPickers(_renderWindow,_numberPickers);
		drawButton(_renderWindow,_exitButton);
		_renderWindow->display();
	}

	data.mineCount = _numberPickers[NumberPicker::MINE_COUNT].number;
	data.fieldHeight = _numberPickers[NumberPicker::FIELD_HEIGHT].number;
	data.fieldWidth = _numberPickers[NumberPicker::FIELD_WIDTH].number;

	return data;
}

void Menu::Destroy(sfmlRenderWindow& window,sfmlFont& font)
{
	window = std::move(_renderWindow);
	font = std::move(_font);
}

void Menu::setupNumberPicker(NumberPicker::type type,
							 NumberPicker& picker, 
							 std::string desc, 
							 int startValue,
							 sf::Vector2f position,
							 sf::Font& font)
{
	picker.description.setString(desc);
	picker.description.setCharacterSize(20);
	picker.description.setFont(font);
	picker.description.setOrigin(picker.description.getLocalBounds().width / 2,
								 picker.description.getLocalBounds().height / 2);
	picker.description.setPosition(position.x,position.y - 80);
	picker.description.setColor(sf::Color::White);

	picker.number = startValue;
	std::stringstream ss;
	ss << picker.number;
	picker.numberText.setString(ss.str());
	picker.numberText.setFont(font);
	picker.numberText.setCharacterSize(15);
	picker.numberText.setOrigin(picker.numberText.getLocalBounds().width / 2,
								picker.numberText.getLocalBounds().height / 2);
	picker.numberText.setPosition(position.x,position.y);
	picker.numberText.setColor(sf::Color::White);

	picker.textBox.setSize(sf::Vector2f(50,25));
	picker.textBox.setFillColor(sf::Color::Transparent);
	picker.textBox.setOutlineColor(sf::Color::White);
	picker.textBox.setOutlineThickness(1);
	picker.textBox.setOrigin(picker.textBox.getLocalBounds().width / 2,
							 picker.textBox.getLocalBounds().height / 2);
	picker.textBox.setPosition(position.x,position.y + 5);

	picker.downArrow.setSize(sf::Vector2f(30,30));
	picker.downArrow.setFillColor(sf::Color::Red);
	picker.downArrow.setOrigin(picker.downArrow.getLocalBounds().width / 2,
							   picker.downArrow.getLocalBounds().height / 2);
	picker.downArrow.setPosition(position.x,position.y + 45);

	picker.upArrow.setSize(sf::Vector2f(30,30));
	picker.upArrow.setFillColor(sf::Color::Red);
	picker.upArrow.setOrigin(picker.upArrow.getLocalBounds().width / 2,
							 picker.upArrow.getLocalBounds().height / 2);
	picker.upArrow.setPosition(position.x, position.y - 35);

	_numberPickers[type] = picker;
}

std::pair<bool,NumberPicker::type> Menu::doesEventAffectNumberPicker(NumberPickerMap& picker,const sf::Event& event)
{
	std::pair<bool,NumberPicker::type> ret(false,NumberPicker::NONE);
	
	if(event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased)
	{
		std::for_each(picker.begin(),picker.end(),[&](std::pair<NumberPicker::type,NumberPicker> entry)
		{
			if(entry.second.upArrow.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x),
															   static_cast<float>(event.mouseButton.y))
				|| entry.second.downArrow.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x),
																	 static_cast<float>(event.mouseButton.y)))
			{
				ret.first = true;
				ret.second = entry.first;
			}
		});
	}

	return ret;
}

void Menu::handleEvent(NumberPicker& picker,const sf::Event& event)
{
	if(picker.upArrow.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x),
												 static_cast<float>(event.mouseButton.y)))
	{
		picker.number++;
		std::stringstream ss;
		ss << picker.number;
		picker.numberText.setString(ss.str());
	}

	if(picker.downArrow.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x),
												   static_cast<float>(event.mouseButton.y)))
	{
		picker.number--;
		std::stringstream ss;
		ss << picker.number;
		picker.numberText.setString(ss.str());
	}
}

void Menu::drawNumberPickers(sfmlRenderWindow& win,NumberPickerMap& pickers)
{
	std::for_each(pickers.begin(),pickers.end(),[&](std::pair<NumberPicker::type,NumberPicker> entry)
	{
		win->draw(entry.second.description);
		win->draw(entry.second.upArrow);
		win->draw(entry.second.textBox);
		win->draw(entry.second.numberText);
		win->draw(entry.second.downArrow);
	});
}

void Menu::setupButtons(Button& button,
						std::string text,
						sf::Vector2f position,
						sf::Font& font)
{
	button.text.setString(text.c_str());
	button.text.setFont(font);
	button.text.setCharacterSize(20);
	button.text.setColor(sf::Color::White);
	button.text.setOrigin(button.text.getLocalBounds().width / 2,
						  button.text.getLocalBounds().height / 2);
	button.text.setPosition(position);

	button.background.setSize(sf::Vector2f(button.text.getLocalBounds().width + 10,
										   button.text.getLocalBounds().height + 10));
	button.background.setOrigin(button.background.getLocalBounds().width / 2,
								button.background.getLocalBounds().height / 2);
	button.background.setFillColor(sf::Color::Red);
	button.background.setPosition(position.x,position.y + 5);

	button.handleEvent = [&] (sfmlRenderWindow& window,const sf::Event& event)
	{
		if(event.type == sf::Event::MouseButtonReleased
			&& button.background.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x),
															static_cast<float>(event.mouseButton.y)))
		{
			SendMessage(window->getSystemHandle(),WM_CLOSE,0,0);
		}
	};
}

void Menu::drawButton(sfmlRenderWindow& window, Button& button)
{
	window->draw(button.background);
	window->draw(button.text);
}