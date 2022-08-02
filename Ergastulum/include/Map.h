#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <fstream>
#include "Tile.h"
#include "BSP.h"
#include "Engine.h"
#include "AI.h"

class Character;
class FOVMap;
class Tile;
class BSP;
class Engine;
class Item;
struct Spritesheet;

class Map
{
public:
	//Constructors & Destructors
	Map(unsigned int width, unsigned int height,unsigned int spriteSize, Engine* engine);
	Map();
	~Map();

	//Accessors
	Tile *tile(sf::Vector2i pos);
	int height();
	int width();
	std::vector<std::unique_ptr<Character>> *characters();

	//Mutators
	void engine(Engine* engine);
	static void spriteSize(int spriteSize);

	//Functions
	void render(sf::RenderWindow* window, float elapsedTime);
	void update();
	void init(unsigned int width, unsigned int height, unsigned int spriteSize, Engine* engine);
	bool isWalkable(Tile* tile);
	bool occupant(sf::Vector2i pos);

	//MapGen Functions
	void createRoom(bool first, int x, int y, int width, int height);
	void wall(Tile* tile);
	void dig(Tile* tile);
	void dig(int x, int y, int width, int height);
		


	//Auxiliary Functions
	Character* player();
	void addPlayer(sf::String name, sf::Vector2i pos, Spritesheet *spritesheet);
	void addCharacter(sf::String name, sf::Vector2i pos, Spritesheet* spritesheet);
	void addItem(sf::String name, sf::Vector2i pos, sf::Sprite sprite);
	void addCharacter(const json& file, sf::String mob, sf::Vector2i pos);
	void addItem(const json& file, sf::String name, sf::Vector2i pos);

private:
	int m_width, m_height;
	static const int minRoomSize = 2;
	static const int maxRoomSize = 4;
	std::vector<std::unique_ptr<Tile>> m_tiles;
	Engine* m_engine;
	std::unique_ptr<BSP> m_BSP;
	static int m_spriteSize;

	//Misc
	std::vector<std::unique_ptr<Character>> m_characters;
	std::vector<std::unique_ptr<Item>> m_items;
	//std::unique_ptr<FOVMap> m_fovMap;
	Character* m_player;


};



#endif