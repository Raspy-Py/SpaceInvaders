#include "Bullet.h"

#include "ResourceManager.h"

Bullet::Bullet(const sf::Vector2f& pos, float speed, bool isPlayerBullet)
	:
	m_isPlayerBullet(isPlayerBullet),
	m_speed(speed)
{
	SetPosition(pos);

	auto pRM = ResourceManager::GetInstance();

	float scale = GetScale();

	if (isPlayerBullet)
	{
		m_direction = - (3.14159f / 2.0f);
		m_sprites.emplace_back(pRM->GetTexture("BulletI"));
	}
	else
	{
		m_direction = 3.14159f / 2.0f;
		m_sprites.emplace_back(pRM->GetTexture("BulletZ"));
		m_sprites.emplace_back(pRM->GetTexture("BulletS"));
	}

	for (auto& sprite : m_sprites)
	{
		sprite.setScale(scale, scale);
		sprite.setPosition(pos);
	}
}

void Bullet::Draw(sf::RenderWindow& renderTarget) const
{
	renderTarget.draw(m_sprites[m_currentSprite]);
}

void Bullet::Update(float dt)
{
	m_timeElapsed += dt;

	sf::Vector2f pos = GetPosition();

	pos.x += dt * m_speed * std::cos(m_direction);
	pos.y += dt * m_speed * std::sin(m_direction);

	SetPosition(pos);

	if (m_timeElapsed >= m_animationTick)
	{
		m_timeElapsed = 0;
		m_currentSprite = (m_currentSprite + 1) % m_sprites.size();
	}
}

bool Bullet::CollideWith(const sf::Rect<float>& collider)
{
	return false;
}

const sf::Rect<float> Bullet::GetGlobalBoundingBox() const
{
	return m_sprites[m_currentSprite].getGlobalBounds();
}

bool Bullet::IsPlayerBullet()
{
	return m_isPlayerBullet;
}

void Bullet::SetPosition(const sf::Vector2f& pos)
{
	Actor::SetPosition(pos);

	for (auto& sprite : m_sprites)
	{
		sprite.setPosition(pos);
	}
}
