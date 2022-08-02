#include "Engine.h"

#include "Color.h"
#include "RNG.h"
#include "InputHandler.h"

#include "Map.h"
#include "GUI.h"
#include "Entity.h"
#include "Character.h"
#include "Item.h"
#include "FOV.h"
#include "Menu.h"

//Constructors & Desctructors
Engine::Engine(const int width, const int height, const int guiHeight, unsigned int tileSize, unsigned int tileGap, float framesPerSecond)
{
	m_gameState[CURRENT] = START_MENU;
	this->m_tileSize = tileSize;

	m_startScreen.loadFromFile("Assets/Graphics/Menu.png");


	Entity::spriteSize(tileSize);

	Inventory::engine(this);

	m_framesPerSecond = framesPerSecond;


	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), "Ergastulum");
	
	loadGraphics(openJson("Assets/Graphics/Graphics.json"), "Entities");
	loadGraphics(openJson("Assets/Graphics/Graphics.json"), "Terrain");
	loadGraphics(openJson("Assets/Graphics/Graphics.json"), "Items");
	debug = true;
	if (debug)
	{
		for (const auto& sheet : m_spritesheets)
		{
			std::cout << std::string(sheet.first) << std::endl;
		}
	}
	/*
	texture(loadTexture("Assets/Graphics/Entities/Entities.png"), "Entities");
	texture(loadTexture("Assets/Graphics/Entities/Human.png"), "Human");
	texture(loadTexture("Assets/Graphics/Entities/Zombie.png"), "Zombie");
	texture(loadTexture("Assets/Graphics/Entities/Abomination.png"), "Abomination");
	texture(loadTexture("Assets/Graphics/Terrain/Dummy.png"), "Dummy");
	texture(loadTexture("Assets/Graphics/Terrain/Dummywall.png"), "Dummywall");


	texture(loadTexture("Assets/Graphics/Items/BastardSword.png"), "BastardSword");
	texture(loadTexture("Assets/Graphics/Items/Flesh.png"), "Flesh");

	m_spritesheets["Entities"];
	m_spritesheets["Terrain"];
	m_spritesheets["Items"];

	makeTileSet(texture("Entities"), tileSize, tileGap, spritesheet("Entities"));
	makeTileSet(texture("Dummy"), tileSize, tileGap, spritesheet("Terrain"));
	makeTileSet(texture("Dummywall"), tileSize, tileGap, spritesheet("Terrain"));
	makeTileSet(texture("BastardSword"), tileSize, tileGap, spritesheet("Items"));
	makeTileSet(texture("Flesh"), tileSize, tileGap, spritesheet("Items"));
	*/

	m_gui = std::make_unique<GUI>(0, (height - guiHeight), width, guiHeight, Color::White, Color::Black, m_window.get(), this);
	this->m_fixed = m_window.get()->getView();
	this->m_view = m_fixed;
	this->m_handler = std::make_unique<InputHandler>();

	RNG::seed();
	initMobList("Assets/Mobs/mobs.json");
	initLootList("Assets/Mobs/items.json");

	m_elapsedTime = m_clock.restart().asSeconds();
	debug = true;

	return;
}

Engine::~Engine()
{
	return;
}


//Functions
void Engine::update()
{
	if (m_gameState[CURRENT] == START_MENU)
	{
		gui()->menu("mainMenu")->update();
	}
	else
	{
		m_currentMap.get()->update();
		if (m_gameState[CURRENT] == GAME_START || m_gameState[CURRENT] == PLAYER_TURN)
		{
			ShadowCastingFOV::computeFov(m_fovMap.get(), currentMap(), m_player->pos().x, m_player->pos().y, 8);//PENDING - Radius need to be changed
			m_player->update();
		}
		else if (m_gameState[CURRENT] == ENEMY_TURN)
		{
			for (std::vector<std::unique_ptr<Character>>::iterator it = m_currentMap.get()->characters()->begin();
				it != m_currentMap.get()->characters()->end(); it++)
			{
				if ((*it).get() == m_player)
				{
					continue;
				}
				else
				{
					(*it).get()->update();

				}
			}
			m_gameState[CURRENT] = PLAYER_TURN;
		}
		else if (m_gameState[CURRENT] == INVENTORY_MENU)
		{
			m_player->update();
		}
		else if (m_gameState[CURRENT] == CHARACTER_MENU)
		{
			m_player->update();
		}
		m_gui->update();
		return;
	}
}

void Engine::render()
{
	m_elapsedTime = m_clock.restart().asSeconds();
	m_window->clear();



	if (gameState(CURRENT) == START_MENU)
	{
		gui()->menu("mainMenu")->render(m_window.get());
	}
	else if (gameState(CURRENT) == INVENTORY_MENU)
	{
		gui()->menu("inventory")->render(m_window.get());
	}
	else if (gameState(CURRENT) == CHARACTER_MENU)
	{
		gui()->menu("characterMenu")->render(m_window.get());
	}
	else
	{

		m_window.get()->setView(m_view);
		m_view.setCenter(sf::Vector2f((float)(m_player->pos().x * m_tileSize), (float)((m_player->pos().y * m_tileSize) + (m_tileSize * 3))));
		m_currentMap->render(m_window.get(), m_elapsedTime);
		m_window.get()->setView(m_fixed);
		m_gui->render(m_window.get());
		
	}
	if (debug)
	{
		float x = m_handler.get()->getMouseCurrentPosition().x;
		float y = m_handler.get()->getMouseCurrentPosition().y;
		sf::Text txt(sf::String("X:" + std::to_string(int(x)) + "Y:" + std::to_string(int(y))), *gui()->font("Arial"), 10);
		txt.setFillColor(Color::Black);
		txt.setOutlineColor(Color::White);
		txt.setOutlineThickness(2.f);
		txt.setPosition(5, 5);
		m_window.get()->draw(txt);
		bool clicked = m_handler.get()->isMouseButtonDown(sf::Mouse::Button::Left);
		sf::CircleShape c(5.f);
		clicked ? c.setFillColor(Color::Green) : c.setFillColor(Color::Red);
		c.setPosition(70.f, 7.f);
		m_window.get()->draw(c);
	}
	m_window.get()->display();
	return;
}

json Engine::openJson(std::string file)
{
	std::ifstream i(file, std::ifstream::binary);
	json j;
	i >> j;
	return j;
}

//Accessors
/**Returns the size of the Tiles in the grid*/
int Engine::tileSize()
{
	return m_tileSize;
}


int Engine::gameLevel()
{
	return m_gameLevel;
}

/**Returns a sf::Texture from the m_textures map.*/
sf::Texture* Engine::texture(std::string st)
{
	if (m_textures.find(st) != m_textures.end())
	{
		return m_textures[st].get();
	}
	std::cout << "Texture not found" << std::endl;
	return m_textures["Dummy"].get();
}

/**Loads and returns a texture from a string representing the directory.
* std::string file refers to the directory.
* bool colorKey, if true, gets the background color of the loaded image and swaps it for alpha 0.
*/
sf::Texture Engine::loadTexture(std::string file, bool colorKey)
{
	sf::Image image;
	try
	{
		throw image.loadFromFile(file);
	}
	catch (bool answer)
	{
		if (!answer)
		{
			std::cout << "File not found" << std::endl;
			throw;
		}
	}
	if (colorKey)
	{
		image.createMaskFromColor(image.getPixel(0, 0));
	}
	sf::Texture texture;
	texture.loadFromImage(image);
	return texture;
}

/**Makes and returns an sf::Sprite from a texture and a rectangle.*/
sf::Sprite Engine::makeSprite(sf::Texture *texture, sf::IntRect rect)
{
	sf::Sprite sprite;
	sprite.setTexture(*texture);
	sprite.setTextureRect(rect);
	return sprite;
}

/**Returns a sf::Sprite from a given map.
* int sp is the index.
* map sprites is the map.
*/
sf::Sprite Engine::sprite(unsigned int sp, sf::String map)
{
	std::cout << "Spritesheet:" << std::string(map) << "/sprite:" << sp << std::endl;
	return *m_spritesheets[map].m_sprites[sp];
}

/**Gets m_player.*/
Character* Engine::player()
{
	return m_player;
}

/** Returns correntMap.*/
Map* Engine::currentMap()
{

	return m_currentMap.get();
}

/**Returns GUI*/
GUI* Engine::gui()
{
	return m_gui.get();
}

/**Returns a std::map which stores unique ptrs to sf::Sprites, in other words, a spritesheet.*/
Spritesheet* Engine::spritesheet(sf::String index)
{
	return (&m_spritesheets[index]);
}


/**Returns the sf::RenderWindow * window*/
sf::RenderWindow* Engine::window()
{
	return m_window.get();
}

/**Returns the input handler*/
InputHandler* Engine::handler()
{
	return m_handler.get();
}

float Engine::elapsedTime()
{
	return m_elapsedTime;
}

//Mutators
/**Checks if a texture exists within the texture map. If it doesn't, adds it.*/
void Engine::texture(sf::Texture texture, std::string st)
{
	if (m_textures.find(st) == m_textures.end())
	{
		m_textures.insert(std::make_pair(st, std::make_unique<sf::Texture>(texture)));
	}
	else
	{
		std::cout << "There's already a texture with this name" << std::endl;
	}
}

/**
* Makes a tileset from a texture.
* sf::Texture *texture is a pointer to a sf::Texture.
* int tileSize is the size of each tile in pixels.
* int tileGap is the gap between each tile in the texture. 0 if there is no gap.
* map sprites  is a given map to add the sprites to.
*/
void Engine::makeTileSet(sf::Texture* texture, unsigned int tileSize, unsigned int tileGap, Spritesheet* spritesheet)
{
	int tiles = (texture->getSize().x / tileSize) * (texture->getSize().y / tileSize);
	for (int y = 0; y < (texture->getSize().y / tileSize); y++, spritesheet->m_size.y++)
	{
		for (int x = 0; x < (texture->getSize().x / tileSize); x++, spritesheet->m_size.x++)
		{
			sf::Sprite sp;
			sp.setTexture(*texture);
			sp.setTextureRect(sf::IntRect(x*tileSize, y*tileSize, tileSize, tileSize));
			spritesheet->m_sprites.push_back(std::make_unique<sf::Sprite>(sp));
		}
	}
	return;
}

/** Sets an character as the m_player.*/
void Engine::player(Character* player)
{
	m_player = player;
}

void Engine::gameLevel(int level)
{
	this->m_gameLevel = level;
}

void Engine::setGameState(int gameState)
{
	GameStates tmp = m_gameState[CURRENT];
	m_gameState[CURRENT] = static_cast<GameStates>(gameState);
	m_gameState[PREVIOUS] = tmp;
}

//Initializators

/**Initializes a Map as the currentMap.*/
void Engine::initMap(Engine* engine, int width, int height)
{
	if (m_currentMap.get() == nullptr)
	{
		m_currentMap.reset();
	}
	m_currentMap = std::make_unique<Map>();
	currentMap()->engine(engine);
	currentMap()->init(width, height, m_tileSize, this);
	m_fovMap = std::make_unique<FOVMap>(width, height);
}


void Engine::initMobList(sf::String file )
{
	json j = openJson(file);
	for(const auto& it: j.items())
	{ 
		sf::String name = it.key();
		int rarity = it.value().at("rarity");
		m_mobs.push_back(std::make_unique<Mob>(name,rarity));
	}
}

void Engine::initLootList(sf::String file)
{
	json j = openJson(file);
	for (const auto& it : j.items())
	{
		sf::String name = it.key();
		int rarity = it.value().at("rarity");
		sf::String type = it.value().at("itemType");
		m_loot.push_back(std::make_unique<Loot>(name));
	}
}

void Engine::loadGraphics(const json& file, sf::String spriteSheet)
{
	for (const auto &sprites:file.at(spriteSheet).items())
	{
		texture(loadTexture("Assets/Graphics/" + spriteSheet + "/" + sprites.value().get<std::string>()), sprites.key());
		makeTileSet(texture(sprites.key()),m_tileSize,0,spritesheet(sprites.key()));
	}
}
