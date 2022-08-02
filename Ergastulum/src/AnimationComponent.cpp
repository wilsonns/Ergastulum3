#include "AnimationComponent.h"
#include "Engine.h"

#include "Character.h"

AnimationComponent::AnimationComponent(Spritesheet* spritesheet, unsigned int spriteSize, float updateTime)
{
	this->m_updateTime = updateTime;
	m_toUpdate = true;
	unsigned int y = 0;
	unsigned int x = 0;
	m_spriteSize = spriteSize;
	for (y = 0; y * spriteSize < spritesheet->m_size.y*spriteSize;y++)
	{
		m_sprites.push_back(std::vector<std::unique_ptr<sf::Sprite>>());
		for (x = 0; x * spriteSize < spritesheet->m_size.x;x++)
		{
			sf::Sprite sp(*spritesheet->m_sprites[x + y * spritesheet->m_size.x].get());
			m_sprites[y].push_back(std::make_unique<sf::Sprite>(sp));
		}
	}
	m_maxFrames = sf::Vector2u(m_sprites[0].size()-1,m_sprites.size()-1);
}

AnimationComponent::~AnimationComponent()
{

}

sf::Vector2u AnimationComponent::frames()
{
	return m_frames;
}

float AnimationComponent::updateTime()
{
	return m_updateTime;
}

void AnimationComponent::update(sf::Vector2f displayPos, unsigned int y, float elapsedTime)
{
	if (m_toUpdate)
	{
		if (y == m_frames.y)//If no change on the row being iterated through, iterate
		{
			m_totalTime += elapsedTime;
			if (m_totalTime >= m_updateTime)//if the time passed is greater than the set update time, iterate to the next frame
			{
				m_totalTime -= m_updateTime;
				m_frames.x++;
				if (m_frames.x > m_maxFrames.x)//if we're on the last frame, wrap back to the 0th frame
				{
					m_frames.x = 0;
				}
			}
		}
		else//else, change the row to be iterated through and set x back to 0
		{
			m_frames.y = y;
			m_frames.x = 0;
		}
		m_sprites[m_frames.y][m_frames.x].get()->setPosition(displayPos);
	}
	return;
}

void AnimationComponent::render(sf::RenderTarget* window)
{
	window->draw(*m_sprites[m_frames.y][m_frames.x].get());
}

void AnimationComponent::die()
{
	m_toUpdate = false;
	m_frames.y = 0;
	m_frames.x = 0;
	m_sprites[m_frames.y][m_frames.x].get()->rotate(90.f);
	m_sprites[m_frames.y][m_frames.x].get()->move(m_spriteSize,0);	
}



void ItemAnimationComponent::syncWithCharacter(AnimationComponent* characterAnimation)
{
	this->m_frames.x = characterAnimation->frames().x;
	this->m_updateTime = characterAnimation->updateTime();
}