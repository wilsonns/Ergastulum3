#include "Map.h"

#include "Character.h"
#include "Enemy.h"
#include "Item.h"
#include "Engine.h"

#include "Color.h"

int Map::m_spriteSize;

//Constructors & Destructors
Map::Map(unsigned int width, unsigned int height, unsigned int spriteSize, Engine* engine)
{
	this->m_width = width;
	this->m_height = height;
	this->m_spriteSize = spriteSize;
	Tile::spriteSize(spriteSize);
	m_engine = engine;
	for (unsigned int x = 0; x < width; x++)
	{
		for (unsigned int y = 0; y < height;y++)//Fill the m_tiles vector with wall tiles
		{
			bool passable = false;
			int sp = 1;
			m_tiles.push_back(std::make_unique<Tile>(sf::Vector2i(x, y), m_engine->sprite(0, "wall"), passable));
		}
	}
	//The snippet below slipts the map area in many interwined smaller areas as to generate the game map
	m_BSP = std::make_unique<BSP>(0,0,m_width,m_height, this);
	m_BSP.get()->recursiveSplit(2, maxRoomSize,maxRoomSize, 1.5f, 1.5f);
	BSPListener listener((this));
	m_BSP.get()->traverseInOrder(&listener);
	return;
}

Map::Map()
{

}

Map::~Map()
{

}

//Accessors
/**Returns a Tile pointer in the pos position.*/
Tile *Map::tile(sf::Vector2i pos)
{
	return m_tiles[pos.x + pos.y * m_width].get();
}
int Map::height()
{
	return m_height;
}
int Map::width()
{
	return m_width;
}

std::vector<std::unique_ptr<Character>> *Map::characters()
{
	return &m_characters;
}

//Mutators
void Map::engine(Engine* engine)
{
	m_engine = engine;
}

void Map::spriteSize(int spriteSize)
{
	m_spriteSize = spriteSize;
}

//Functions
/**Renders each tile and then each entity in the Map.*/
void Map::render(sf::RenderWindow* window, float elapsedTime)
{
	for (auto& tile : m_tiles)
	{
		if (tile->visible() == true)
		{
			//draws a tile if visible
			tile->sprite()->setColor(Color::White);
			window->draw(*tile->sprite());
		}
		else if (tile->visible() == false && tile->explored() == true)
		{
			//draws a tile if not visible but was already explored. The sprite is set to a sepia color
			tile->sprite()->setColor(Color::Sepia);
			window->draw(*tile->sprite());
		}
	}
	for (auto& item : m_items)
	{
		if (tile(item.get()->pos())->visible() && item.get()->owner() == nullptr)
		{
			item.get()->render(m_engine->window());
		}
	}
	for (auto& character : m_characters)
	{
		if (tile(character.get()->pos())->visible())
		{
			character.get()->render(m_engine->window(), elapsedTime);
		}
	}
}

void Map::update()
{
	for (auto& tile : m_tiles)
	{
		tile->visible(false);
	}
}

void Map::init(unsigned int width, unsigned int height, unsigned int spriteSize, Engine* engine)
{
	this->m_width = width;
	this->m_height = height;
	this->m_spriteSize = spriteSize;
	m_engine = engine;
	Tile::spriteSize(spriteSize);
	for (unsigned int y = 0; y < height;y++)
	{
		for (unsigned int x = 0; x < width; x++)//Fill the m_tiles vector with wall tiles
		{
			bool passable = false;
			int sp = 1;
			m_tiles.push_back(std::make_unique<Tile>(sf::Vector2i(x, y), m_engine->sprite(0, "wall"), passable, true));
		}
	}
	//The snippet below slipts the map area in many interwined smaller areas as to generate the game map
	m_BSP = std::make_unique<BSP>(0,0,m_width,m_height, this);
	m_BSP.get()->recursiveSplit(4, maxRoomSize, maxRoomSize, 1.5f, 1.5f);
	BSPListener listener((this));
	m_BSP.get()->traverseInOrder(&listener);
	/*for (int y = 0; y < m_height;y++)
	{
		std::cout << std::endl;
		for (int x = 0; x < m_width;x++)
		{
			std::cout << tile(sf::Vector2i(x, y))->passable() ? "." : "#";
		}
	}*/
	return;
}

/**Chekcs if a tile is walkable.*/
bool Map::isWalkable(Tile* tile)
{
	if (tile->occupant() == nullptr && tile->passable())
	{
		return true;
	}
	return false;
}

bool Map::occupant(sf::Vector2i pos)
{
	return tile(pos)->occupant() != nullptr;
}

/*Turns the tile into a wall*/
void Map::wall(Tile* tile)
{
	tile->passable(false);
	tile->opaque(true);
	tile->sprite(m_engine->sprite(0, "wall"));
}

/*Turns the tile into a floor*/
void Map::dig(Tile* tile)
{
	tile->passable(true);
	tile->opaque(false);
	tile->sprite(m_engine->sprite(0, "floor"));
}

void Map::dig(int x, int y, int width, int height)
{
	for (int i = y; i <= height; i++)
	{
		for (int j = x; j <= width; j++)
		{
			dig(tile(sf::Vector2i(j,i)));
		}
	}
}

void Map::createRoom(bool first, int x, int y, int width, int height)
{
	dig(x,y,width,height);//Turns the area between the coordinates into floor tiles
	if (first)//If this is the first room to be created, place player there
	{
		sf::Vector2i pos(RNG::randInt(x, width), RNG::randInt(y, height));
		addPlayer("Jack",pos,m_engine->spritesheet("human"));
		m_engine->player(tile(pos)->occupant());
		m_player = m_engine->player();
		player()->maxResource("Health", 9999);
		player()->currentResource("Health", 9999);
		player()->engine(m_engine);
		player()->ai(std::make_unique<PlayerAI>());
		player()->ai()->engine(m_engine);json j;
/*		std::ifstream i("Assets/Mobs/items.json", std::ifstream::binary);
		i >> j;
		addItem(j, "dummy", pos);*/
	
	}
	else//else, place enemies and items
	{
		if (RNG::randInt(1, 3) > 0)
		{
			sf::String monster = "zombie";
			if (RNG::randInt(1, 10) == 10)
			{
				monster = "abomination";
			}
			json j;
			std::ifstream i("Assets/Mobs/mobs.json", std::ifstream::binary);
			i >> j;
		
			sf::Vector2i pos(RNG::randInt(x, width), RNG::randInt(y, height));
			addCharacter(j,monster,pos);
			tile(pos)->occupant()->ai(std::make_unique<DummyAI>());
		}
	}

}

Character* Map::player()
{
	return m_player;
}


void Map::addPlayer(sf::String name, sf::Vector2i pos, Spritesheet *spritesheet)
{
	std::unique_ptr<Character> character;
	character = std::make_unique<Character>(name, pos, spritesheet);
	tile(pos)->occupant(character.get());
	m_characters.push_back(std::move(character));
}

/**Adds an character to the map in a given position.*/
void Map::addCharacter(sf::String name, sf::Vector2i pos, Spritesheet *spritesheet)
{
	std::unique_ptr<Enemy> character;
	character = std::make_unique<Enemy>(name, pos, spritesheet);

	tile(pos)->occupant(character.get());
	m_characters.push_back(std::move(character));
}

void Map::addItem(sf::String name, sf::Vector2i pos, sf::Sprite sprite)
{
	std::unique_ptr<Weapon> item;
	AttributeComponent attCom;

	attCom.attribute("Attack", 1);
	attCom.attribute("Weight", 1);
	attCom.attribute("Damage", 100);

	item = std::make_unique<Weapon>(name, pos, sprite, attCom);//PENDING - make items more varied
	tile(pos)->inventory()->addItem(item.get());
	m_items.push_back(std::move(item));
}

void Map::addItem(const json& file, sf::String name, sf::Vector2i pos)
{
	std::unique_ptr<Item> item;
	sf::String type(file.at(name).at("itemType"));
	if(type == "weapon")
	{
		item = std::make_unique<Weapon>(file, name, pos);
	}
	if(type =="healing")
	{
		item = std::make_unique<Healing>(file, name, pos);
	}
	tile(pos)->inventory()->addItem(item.get());
	m_items.push_back(std::move(item));
}


void Map::addCharacter(const json& file, sf::String mob, sf::Vector2i pos)
{
	std::unique_ptr<Enemy> character;
	character = std::make_unique<Enemy>(file, mob, pos);
	tile(pos)->occupant(character.get());
	m_characters.push_back(std::move(character));
}
