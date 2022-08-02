#include "Bar.h"

//Constructors & Destructors
Bar::Bar(float x, float y, float width, float height, float outlineThickness,
	sf::Color innerColor, sf::Color outlineColor)
{
	m_bar.setSize(sf::Vector2f(width, height));
	m_bar.setPosition(sf::Vector2f(x, y));
	m_bar.setOutlineColor(outlineColor);
	m_bar.setOutlineThickness(outlineThickness);
	m_bar.setFillColor(innerColor);
}

//Mutators
void Bar::width(float newWidth)
{
	m_bar.setSize(sf::Vector2f(newWidth, m_bar.getSize().y));
}