#include "InventoryMenu.h"

#include "GUI.h"
#include "Character.h"
#include "Item.h"
#include "Inventory.h"


InventoryMenu::InventoryMenu(sf::RectangleShape panel, 
	sf::Color innerColor, sf::Color outlineColor,
	float outlineThickness, unsigned int stateToDraw)
{
	m_panel = panel;
	m_panel.setOutlineColor(outlineColor);
	m_panel.setOutlineThickness(outlineThickness);
	m_panel.setFillColor(innerColor);
	m_stateToDraw = stateToDraw;
}


void InventoryMenu::render(sf::RenderTarget* window)
{
	if (m_engine->gameState(m_engine->CURRENT) == m_stateToDraw || m_stateToDraw == m_engine->ANY)
	{
		m_panel.setPosition(window->getSize().x / 2, window->getSize().y / 2);
		window->draw(m_panel);
		int shortcut = 'a';
		int i = 0;
		for (Item& item : m_itemsToDraw)
		{
			item.sprite()->setPosition(m_panel.getGlobalBounds().left, m_panel.getGlobalBounds().top+(item.spriteSize()*i));
			item.render(window);
			std::string text = item.name();
			std::string::iterator it;
			it = text.begin();
			text.insert(it, '-');
			text.insert(it, (char)shortcut);
			sf::Text tmp(sf::String(text), *m_engine->gui()->font("Arial"), 16);
			tmp.setFillColor(sf::Color::Black);
			tmp.setPosition(sf::Vector2f(item.sprite()->getPosition().x + item.spriteSize()+6, item.sprite()->getPosition().y));
			i++;
			shortcut++;
			window->draw(tmp);
		}
		for (const auto& button : m_buttons)
		{
			button.get()->render(window);
		}
	}
}

void InventoryMenu::update()
{
	if (m_engine->gameState(m_engine->CURRENT) == m_stateToDraw || m_stateToDraw == m_engine->ANY)
	{
		m_itemsToDraw.clear();
		//updates the inventory contents
		std::vector<Item*> items = *m_engine->player()->inventory()->contents();
		int h = 0;
		for (auto& item : items)
		{
			//item->sprite()->setPosition((float)m_panel.getPosition().x + 9, (float)m_panel.getPosition().y + item->spriteSize() * h);
			m_itemsToDraw.push_back((*item));
			h++;
		}

		int width = 0;
		for (auto& item : m_itemsToDraw)
		{
			if (item.name().getSize() > width)
			{
				width = item.name().getSize();
			}
		}
		width *= 30;
		m_panel.setSize(sf::Vector2f((float)width, (float)Item::spriteSize()*m_itemsToDraw.size()));
		m_panel.setOrigin(m_panel.getSize().x / 2, m_panel.getSize().y / 2);
	}
}