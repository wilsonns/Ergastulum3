#include "CharacterMenu.h"

#include "Character.h"
#include "Engine.h"
#include "GUI.h"

//Constructors & Destructors
CharacterMenu::CharacterMenu(sf::RectangleShape panel, sf::Color innerColor, sf::Color outlineColor,
	float outlineThickness, unsigned int stateToDraw)
{
	m_panel = panel;
	m_panel.setOutlineColor(outlineColor);
	m_panel.setOutlineThickness(outlineThickness);
	m_panel.setFillColor(innerColor);
	m_stateToDraw = stateToDraw;
}

CharacterMenu::~CharacterMenu()
{

}

//Accessors

//Mutators

//Functions
void CharacterMenu::update()
{
	if (engine()->gameState(engine()->CURRENT) == m_stateToDraw)
	{
		sf::Text nameLabel = sf::Text(m_engine->player()->name(), *engine()->gui()->font("Arial"), 20);
		nameLabel.setPosition(sf::Vector2f(m_panel.getGlobalBounds().left + 14, m_panel.getGlobalBounds().top + 10));
		nameLabel.setFillColor(Color::Black);
		m_text["name"] = nameLabel;

		m_text["attributes"] = sf::Text("Attributes", *engine()->gui()->font("Arial"), 17);
		m_text["attributes"].setPosition(sf::Vector2f(m_panel.getGlobalBounds().left + 15.f, m_panel.getGlobalBounds().top + 30));
		m_text["attributes"].setFillColor(Color::Black);
		int ite = 1;
		for (auto& attribute : engine()->player()->attributes().attributes())
		{
			sf::Vector2f pos(m_text["attributes"].getPosition());
			pos.y += (ite*m_text["attributes"].getCharacterSize());
			m_text[attribute.first] = sf::Text(attribute.first, *engine()->gui()->font("Arial"), 14);
			m_text[attribute.first].setPosition(pos);
			m_text[attribute.first].setFillColor(Color::Black);
			m_text[attribute.first + "lvl"] = sf::Text(std::to_string(attribute.second.level()), *engine()->gui()->font("Arial"), 14);
			pos.x += m_text[attribute.first].getCharacterSize()*m_text[attribute.first].getString().getSize();
			m_text[attribute.first +"lvl"].setFillColor(Color::Black);
			m_text[attribute.first + "lvl"].setPosition(pos);

			ite++;
		}
	}
}

void CharacterMenu::render(sf::RenderWindow *window)
{
	sf::Vector2f size(window->getSize().x*0.9, window->getSize().y*0.9);
	m_panel.setSize(sf::Vector2f(size));
	m_panel.setOrigin(m_panel.getSize().x / 2, m_panel.getSize().y / 2);
	m_panel.setPosition(window->getSize().x / 2, window->getSize().y / 2);
	window->draw(m_panel);
}