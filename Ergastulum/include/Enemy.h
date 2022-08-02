#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include <map>


#include "Character.h"

struct Loot;

class Enemy : public Character
{
public:
	Enemy(const json& file, sf::String mob, sf::Vector2i pos);
	Enemy(sf::String name, sf::Vector2i pos, Spritesheet* spritesheet);
	void die();
private:
	std::vector<std::pair<Loot, int>>m_loot;
	int m_dropChance;
};



#endif