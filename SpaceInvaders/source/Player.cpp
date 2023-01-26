#include "Player.h"
#include "ResourceManager.h"

Player::Player()
{
	auto pRM = ResourceManager::GetInstance();
	auto vm = sf::VideoMode::getDesktopMode();
	float scale = GetScale();


	m_sprites.emplace_back(pRM->GetTexture("SpaceShip"));

	for (auto& sprite : m_sprites)
	{
		sprite.setScale(scale, scale);
	}

	SetPosition({ (float)vm.width / 2.0f, (float)vm.height - 100.0f });
}

void Player::Draw(sf::RenderWindow& renderTarget) const
{
	renderTarget.draw(m_sprites[0]);
}

void Player::Update(float dt)
{
}

bool Player::CollideWith(const sf::Rect<float>& collider)
{
	bool isHit = collider.intersects(GetGlobalBoundingBox());

	if (isHit)
	{
		m_health--;
	}

	return isHit;
}

void Player::SetHealth(int health)
{
	m_health = health;
}

void Player::SetPosition(const sf::Vector2f& pos)
{
	Actor::SetPosition(pos);

	for (auto& sprite : m_sprites)
	{
		sprite.setPosition(pos);
	}
}

int Player::GetHealth() const
{
	return m_health;
}

const sf::Rect<float> Player::GetGlobalBoundingBox() const
{
	return m_sprites[0].getGlobalBounds();
}

void Player::MoveLeft(float dt)
{
	sf::Vector2f pos = GetPosition();

	pos.x -= dt * m_speed;

	SetPosition(pos);
}

void Player::MoveRight(float dt)
{
	sf::Vector2f pos = GetPosition();

	pos.x += dt * m_speed;

	SetPosition(pos);
}
