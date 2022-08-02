#ifndef CHARACTERMENU_H
#define CHARACTERMENU_H

#include "Menu.h"

#include <vector>
#include <SFML/Graphics.hpp>

class CharacterMenu : public Menu
{
public:
	//Constructors & Destructors
	CharacterMenu(sf::RectangleShape panel, sf::Color innerColor, sf::Color outlineColor,
		float outlineThickness, unsigned int stateToDraw);
	~CharacterMenu();

	//Accessors

	//Mutators

	//Functions
	void update();
	void render(sf::RenderWindow *window);

private:

protected:
};

#endif