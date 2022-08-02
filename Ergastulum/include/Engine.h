#ifndef ENGINE_H
#define ENGINE_H

#include<SFML/Graphics.hpp>
#include<nlohmann/json.hpp>



using json = nlohmann::json;

class Map;
class GUI;
class Character;
class Item;
class AI;
class InputHandler;

struct FOVMap;

struct Mob
{
public:
	Mob(sf::String name, int rarity)
	{
		this->m_name = name;
		this->m_rarity = rarity;
	}
	sf::String m_name;
	int m_rarity;
};

struct Loot
{
public:
	Loot(sf::String name)
	{
		this->m_name = name;
	}
	sf::String m_name;
};


struct Spritesheet
{
public:
	Spritesheet()
	{
		m_size = sf::Vector2i(0, 0);
	}
	std::vector<std::unique_ptr<sf::Sprite>> m_sprites;
	sf::Vector2i m_size;
	sf::Sprite* sprite(sf::Vector2i position)
	{
		return m_sprites[position.x + position.y * m_size.x].get();
	}
};

class Engine
{
public:
	enum GameStates
	{
		START_MENU = 0,
		GAME_START,
		PLAYER_TURN,
		ENEMY_TURN,
		INVENTORY_MENU,
		CHARACTER_MENU,
		ANY = 999
	};
	enum StateNumber
	{
		CURRENT = 0,
		PREVIOUS
	};
	//Constructors & Desctructors
	Engine(const int width, const int height, const int guiHeight, unsigned int tileSize, unsigned int tileGap, float framesPerSecond);
	~Engine();


	///Accessors
	//int
	int tileSize();
	int gameLevel();
	
	//float
	float elapsedTime();
	
	//sf::Texture
	sf::Texture* texture(std::string st);
	sf::Texture loadTexture(std::string file, bool colorKey = false);
	//sf::Sprite
	sf::Sprite makeSprite(sf::Texture* texture, sf::IntRect rect);
	sf::Sprite sprite(unsigned int sp, sf::String map);
	Character* player();
	Map* currentMap();
	GUI* gui();
	Spritesheet* spritesheet(sf::String index);
	sf::RenderWindow* window();
	InputHandler* handler();
	

	inline GameStates gameState(StateNumber state)
	{
		//return the GameState on the StateNumber
		return m_gameState[state];
	}

	//Mutators
	void texture(sf::Texture texture, std::string st); //adciona uma textura nova ao mapa m_texturas
	void makeTileSet(sf::Texture* texture, unsigned int tileSize, unsigned int tileGap, Spritesheet *spritesheet);
	void player(Character* player);
	void gameLevel(int level);

	void setGameState(int gameState);

	//Functions
	void update();
	void render();
	json openJson(std::string file);
	sf::View m_view;

	//Initializators
	void initMobList(sf::String file);
	void initLootList(sf::String file);
	void initMap(Engine *engine, int width, int height);
	void loadGraphics(const json& file, sf::String spriteSheet);

	//StartMenu Stuff
	sf::Texture m_startScreen;
private:
	//sf::Window resources
	std::unique_ptr<sf::RenderWindow> m_window;

	sf::View m_fixed;

	//InputHandler resources
	std::unique_ptr<InputHandler> m_handler;

	//Textures and Sprite resources
	std::map<std::string, std::unique_ptr<sf::Texture>> m_textures;
	std::map<sf::String, Spritesheet> m_spritesheets;


	///Game Logic Resources
	//Map and Entities
	Character* m_player;
	std::unique_ptr<Map> m_currentMap;
	std::unique_ptr<FOVMap> m_fovMap;
	sf::Clock m_clock;
	float m_elapsedTime;
	float m_updatesPerSecond;
	float m_framesPerSecond;

	//Content distribution
	std::vector<std::unique_ptr<Mob>> m_mobs;
	std::vector<std::unique_ptr<Loot>> m_loot;

	//GameStates
	std::map<StateNumber, GameStates> m_gameState;

	//Misc.
	bool debug;

protected:
	int m_tileSize;
	std::unique_ptr<GUI> m_gui;
	std::vector<sf::String> m_attributes;
	int m_gameLevel;
};

#endif