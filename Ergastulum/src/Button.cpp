#include "Button.h"
#include "InputHandler.h"
#include "boost/bind.hpp"
#include "Engine.h"


Button::Button()
{

}

Button::Button(sf::Text text, sf::Vector2f size, sf::Vector2f pos, sf::Color innerColor,
	sf::Color outlineColor, sf::Color textColor, float outlineThickness, Engine* engine, int gameState)
{
	this->m_shape.setSize(size);
	this->m_shape.setOrigin(size.x / 2, size.y / 2);
	this->m_shape.setPosition(pos);
	this->m_shape.setFillColor(innerColor);
	this->m_idleColor = innerColor;
	this->m_hoverColor = sf::Color(innerColor.r*0.75f, innerColor.g * 0.75f, innerColor.b * 0.75f);
	this->m_clickedColor = sf::Color(innerColor.r*0.35f, innerColor.g * 0.35f, innerColor.b * 0.35f); //Makes the clicked Color a darker hue than the idleColor
	if (outlineThickness > 0.f)
	{
		this->m_shape.setOutlineColor(outlineColor);
		this->m_shape.setOutlineThickness(outlineThickness);
	}
	if (text.getString() != "")
	{
		this->m_text = text;
		this->m_text.setFillColor(textColor);
		sf::FloatRect bounds = m_text.getLocalBounds();
		sf::Vector2f origin;
		origin.x = bounds.width/2;
		origin.y = bounds.height/2;
		this->m_text.setOrigin(origin);
		this->m_text.setPosition(m_shape.getPosition().x, m_shape.getPosition().y);

	}
	this->onClick = boost::bind(&Engine::setGameState,engine,_1);
	this->m_gameState = gameState;
	std::cout << "Button center X:" << m_shape.getPosition().x << "/Y:" << m_shape.getPosition().y << std::endl;
	std::cout << "Text center X:" << m_text.getPosition().x << "/Y:" << m_text.getPosition().y << std::endl;
}

Button::~Button()
{

}

void Button::render(sf::RenderTarget* window)
{
	window->draw(m_shape);
	window->draw(m_text);
}

void Button::update(InputHandler* handler, sf::Mouse::Button mouseButton)
{
	this->m_buttonState = BTN_IDLE;
	
	if (this->m_shape.getGlobalBounds().contains(sf::Vector2f(handler->getMouseCurrentPosition().x, handler->getMouseCurrentPosition().y)))
	{
		this->m_buttonState = BTN_HOVER;
		if (handler->isMouseButtonPressed(mouseButton))
		{
			this->m_buttonState = BTN_CLICKED;
			onClick(m_gameState);
		}
	}
	switch (m_buttonState)
	{
	case BTN_IDLE:
		m_shape.setFillColor(m_idleColor);
		break;
	case BTN_HOVER:
		m_shape.setFillColor(m_hoverColor);
		break;
	case BTN_CLICKED:
		m_shape.setFillColor(m_clickedColor);
		break;
	}
}


