#ifndef GAMESTATE_H
#define GAMESTATE_H

#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>

class GameState
{
public:
	//Constructors & Destructors


	//Accessors
	
	
	//Mutators
	

	//Functions
	virtual void update() = 0;
	virtual void render() = 0;

private:
	sf::String m_soundtrack;

};

#endif

