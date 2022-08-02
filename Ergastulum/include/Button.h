#ifndef BUTTON_H
#define BUTTON_H

#include "SFML/Graphics.hpp"
#include "Color.h"

#include <iostream>
#include "InputHandler.h"
#include "boost/bind.hpp"
#include "boost/function.hpp"

#include "Engine.h"

struct MousePosition;

enum buttonState
{
	BTN_IDLE = 0,
	BTN_HOVER = 1,
	BTN_CLICKED = 2
};

class Button
{
public:
	//Constructors & Destructors
	Button();
	Button(sf::Text text, sf::Vector2f size, sf::Vector2f pos,
		sf::Color innerColor, sf::Color outlineColor, sf::Color textColor, float outlineThickness, Engine*engine, int gameState);
	~Button();

	//Functions
	void render(sf::RenderTarget* window);
	void update(InputHandler* handler, sf::Mouse::Button mouseButton);

private:
	int m_buttonState;
	int m_gameState;
	sf::RectangleShape m_shape;
	sf::Color m_idleColor;
	sf::Color m_hoverColor;
	sf::Color m_clickedColor;
	
	sf::Text m_text;
	//Can't be virtual, must be individually implemented
	boost::function<void(int)> onClick;
};

#endif