#include "Alien.h"
#include "ResourceManager.h"

Alien::Alien(const std::string& strType, const sf::Vector2f& pos, float scaleMode)
	:
	Actor(scaleMode),
	m_type(Type::Android),
	m_strType(strType)
{
	m_speedVector = { 1.0f, 0.0f };
	m_speed = 0.1f;
	m_timeElapsed = float(rand()% int(m_animationTick)/2);

	float scale = GetScale();

	Actor::SetPosition(pos);

	auto pRM = ResourceManager::GetInstance();

	if (strType == "UFO")
	{
		m_sprites.emplace_back(pRM->GetTexture("UFO"));
	}
	else
	{
		m_sprites.emplace_back(pRM->GetTexture("Alien" + strType + "0"));
		m_sprites.emplace_back(pRM->GetTexture("Alien" + strType + "1"));
	}

	for (size_t i = 0; i < m_sprites.size(); i++)
	{
		m_sprites[i].setScale(scale, scale);
		m_sprites[i].setPosition(pos);
	}
}


void Alien::Draw(sf::RenderWindow& renderTarget) const
{
	renderTarget.draw(m_sprites[m_currentSprite]);
}

void Alien::Update(float dt)
{
	m_timeElapsed += dt;

	sf::Vector2f pos = GetPosition();

	pos.x += m_speedVector.x * dt * m_speedScale;

	SetPosition(pos);

	if (m_timeElapsed > m_animationTick)
	{
		m_timeElapsed = 0;
		m_currentSprite = (m_currentSprite + 1) % m_sprites.size();
	}
}

void Alien::ChangeDirection(float adj)
{
	m_speedVector.x *= -1;
	sf::Vector2f pos = GetPosition();

	pos.x += m_speedVector.x * adj;

	SetPosition(pos);
}

void Alien::MoveDown()
{
	sf::Vector2f pos = GetPosition();

	pos.y += (float)sf::VideoMode::getDesktopMode().height / 100.0f; // TODO: replace with not a hardcoded value

	SetPosition(pos);
}

void Alien::SetPosition(const sf::Vector2f& pos)
{
	Actor::SetPosition(pos);

	for (auto& sprite : m_sprites)
	{
		sprite.setPosition(pos);
	}
}

void Alien::SetSpeedScale(float speedScale)
{
	m_speedScale = speedScale;
}

bool Alien::CollideWith(const sf::Rect<float>& collider)
{
	// TODO: add death animation
	return collider.intersects(GetGlobalBoundingBox());
}

const sf::Rect<float> Alien::GetGlobalBoundingBox() const
{
	return m_sprites[m_currentSprite].getGlobalBounds();
}
