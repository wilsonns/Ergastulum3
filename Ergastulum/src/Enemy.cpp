#include "Enemy.h"
#include "Engine.h"
#include "RNG.h"
#include "Map.h"
#include "Stats.h"
#include "AnimationComponent.h"

Enemy::Enemy(const json& file, sf::String mob, sf::Vector2i pos)
{
	this->m_name = file.at(mob).at("name");
	this->m_pos = pos;
	this->m_animation = std::make_unique<AnimationComponent>(m_engine->spritesheet(mob), m_spriteSize, 0.5f);

	//Create unique_ptrs
	this->m_attributes = std::make_unique<AttributeComponent>();
	this->m_skills = std::make_unique<SkillComponent>();
	this->m_resources = std::make_unique<ResourceComponent>();

	this->m_inventory = std::make_unique<Inventory>(file.at(mob).at("inventory"));

	m_attributes.get()->attribute("Strength", RNG::randInt(file.at(mob).at("attributes").at("Strength").at(0), file.at(mob).at("attributes").at("Strength").at(1)));
	m_attributes.get()->attribute("Dexterity", RNG::randInt(file.at(mob).at("attributes").at("Dexterity").at(0), file.at(mob).at("attributes").at("Dexterity").at(1)));
	m_attributes.get()->attribute("Intelect", RNG::randInt(file.at(mob).at("attributes").at("Intelect").at(0), file.at(mob).at("attributes").at("Intelect").at(1)));
	m_attributes.get()->attribute("Vitality", RNG::randInt(file.at(mob).at("attributes").at("Vitality").at(0), file.at(mob).at("attributes").at("Vitality").at(1)));
	m_attributes.get()->attribute("DR", RNG::randInt(file.at(mob).at("attributes").at("DR").at(0), file.at(mob).at("attributes").at("DR").at(1)));
	m_attributes.get()->attribute("Speed", 90 + attribute("Dexterity"));

	for (auto const& skilllvl : file.at(mob).at("skills").items())
	{
		m_skills.get()->skill(skilllvl.key(), RNG::randInt(skilllvl.value().at(0), skilllvl.value().at(1)));
	}

	m_resources.get()->maxLevel("Health", attribute("Strength") * 3 + attribute("Vitality") * 2);
	m_resources.get()->currentLevel("Health", m_resources.get()->maxLevel("Health"));
	m_resources.get()->maxLevel("Energy", attribute("Vitality") * 10);
	m_resources.get()->currentLevel("Energy", m_resources.get()->maxLevel("Energy"));

	for (auto const& loot : file.at(mob).at("loot").items())
	{
		m_loot.push_back(std::make_pair(Loot(loot.key()),10));
	}
}


Enemy::Enemy(sf::String name, sf::Vector2i pos, Spritesheet* spritesheet)
{

	this->m_name = name;
	this->m_pos = pos;
	/*this->m_sprite = sprite;
	this->m_sprite.setPosition((float)m_pos.x * m_spriteSize, (float)m_pos.y * m_spriteSize);
	*/

	//Create unique_ptrs
	this->m_attributes = std::make_unique<AttributeComponent>();
	this->m_skills = std::make_unique<SkillComponent>();
	this->m_resources = std::make_unique<ResourceComponent>();
	this->m_animation = std::make_unique<AnimationComponent>(spritesheet, spriteSize(), 0.6f);
	this->m_inventory = std::make_unique<Inventory>(10);

	m_attributes.get()->attribute("Strength", 10);
	m_attributes.get()->attribute("Dexterity", 10);
	m_attributes.get()->attribute("Intelect", 10);
	m_attributes.get()->attribute("Vitality", 10);
	m_attributes.get()->attribute("DR", 0);
	m_attributes.get()->attribute("Speed", 90 + attribute("Dexterity"));

	m_skills.get()->skill("Melee", 0);
	m_skills.get()->skill("Dodge", 0);


	m_resources.get()->maxLevel("Health", attribute("Strength") * 3 + attribute("Vitality") * 2);
	m_resources.get()->currentLevel("Health", m_resources.get()->maxLevel("Health"));
	m_resources.get()->maxLevel("Energy", attribute("Vitality") * 10);
	m_resources.get()->currentLevel("Energy", m_resources.get()->maxLevel("Energy"));

	std::cout << "Character created." << std::endl;
}


void Enemy::die()
{
	Character::die();
	if (died())
	{
		m_dropChance = 10;
		if (RNG::randInt(0, 10) < m_dropChance)
		{
			int item = RNG::randInt(1, m_loot.size() - 1);
			m_engine->currentMap()->addItem(m_engine->openJson("Assets/Mobs/items.json"), m_loot[item].first.m_name, this->pos());
		}
	}
}