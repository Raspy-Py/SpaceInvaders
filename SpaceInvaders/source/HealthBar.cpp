#include "HealthBar.h"
#include "ResourceManager.h"

HealthBar::HealthBar(int health)
{
	auto pRM = ResourceManager::GetInstance();
	auto vm = sf::VideoMode::getDesktopMode();
	using sfv = sf::Vector2f;

	float scale = GetScale();
	m_health = health;

	for (size_t i = 0; i < 6; i++)
	{
		m_sprites.emplace_back(pRM->GetTexture("HealthBar" + std::to_string(i)));
		m_sprites[i].setScale(scale, scale);
	}

	SetPosition(sfv(float(vm.width - (float)vm.height * 0.8f)/ 2 + 10, (float)vm.height-40));
}

void HealthBar::Draw(sf::RenderWindow& renderTarget) const
{
	renderTarget.draw(m_sprites[std::max(0, m_health)]);
}

void HealthBar::Update(float dt)
{
}

bool HealthBar::CollideWith(const sf::Rect<float>& collider)
{
	return false;
}

void HealthBar::SetHealth(int health)
{
	m_health = health;
}

void HealthBar::SetPosition(const sf::Vector2f& pos)
{
	Actor::SetPosition(pos);

	for (auto& sprite : m_sprites)
	{
		sprite.setPosition(pos);
	}
}
