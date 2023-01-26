#include "Wall.h"
#include "ResourceManager.h"

Wall::Wall(const std::string& type, const sf::Vector2f& pos, float scaleMode)
	:
	Actor(scaleMode),
	m_type(type)
{
	auto pRM = ResourceManager::GetInstance();

	Actor::SetPosition(pos);

	m_healthLeft = m_healthRight = 4;

	if (type == "Left")
		m_healthRight = 0;
	else if (type == "Right")
		m_healthLeft = 0;

	float scale = GetScale();

	m_sprites.emplace_back(pRM->GetTexture("WallLeft0"));
	m_sprites.emplace_back(pRM->GetTexture("WallRight0"));

	m_sprites.emplace_back(pRM->GetTexture("WallLeft1"));
	m_sprites.emplace_back(pRM->GetTexture("WallRight1"));

	m_sprites.emplace_back(pRM->GetTexture("WallLeft2"));
	m_sprites.emplace_back(pRM->GetTexture("WallRight2"));

	m_sprites.emplace_back(pRM->GetTexture("WallLeft3"));
	m_sprites.emplace_back(pRM->GetTexture("WallRight3"));

	float wallSectionWidth = m_sprites[0].getGlobalBounds().width;
	sf::Vector2f rightPos = pos;
	if (type == "Whole") rightPos.x += wallSectionWidth * scale;
	for (size_t i = 0; i < m_sprites.size(); i++)
	{
		m_sprites[i].setPosition(((i & 1) ? rightPos : pos));
		m_sprites[i].setScale(scale, scale);
	}
}

void Wall::Draw(sf::RenderWindow& renderTerget) const
{
	if (m_healthLeft > 0)
	{
		renderTerget.draw(m_sprites[(m_healthLeft - 1) * 2]);
	}
	if (m_healthRight > 0)
	{
		renderTerget.draw(m_sprites[(m_healthRight - 1) * 2 + 1]);
	}
}

void Wall::Update(float dt)
{
}

bool Wall::CollideWith(const sf::Rect<float>& collider)
{
	bool hitLeft = m_healthLeft > 0 && m_sprites[(m_healthLeft - 1) * 2].getGlobalBounds().intersects(collider) ;
	bool hitRight = m_healthRight > 0 && m_sprites[(m_healthRight - 1) * 2 + 1].getGlobalBounds().intersects(collider);

	if (hitLeft) m_healthLeft--;
	if (hitRight) m_healthRight--;

	if (hitLeft && hitRight)
	{
		// randomly heal one wall, as we don't want two of them to be damaged at the same time
		(rand() % 2) ? m_healthRight++ : m_healthLeft++;
	}


	return hitRight || hitLeft;
}

const std::string& Wall::GetStrType() const 
{
	return m_type;
}

const sf::Rect<float> Wall::GetGlobalBoundingBox() const
{
	sf::Rect<float> gbb = m_sprites[0].getGlobalBounds();
	if (m_type == "Whole")
		gbb.width *= 2;
	return gbb;
}
