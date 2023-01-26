#include "World.h"
#include "ResourceManager.h"

World::World(const std::string& levelName)
{
	auto pRM = ResourceManager::GetInstance();
	pRM->LoadLevelData(levelName);
	auto& pLevelData = pRM->GetLevelData(levelName);

	m_player = std::make_unique<Player>();

	// TODO: check if such ptr inititalization actually works
	for (auto& wall : pLevelData.GetWalls())
	{
		m_walls.push_back(std::make_unique<Wall>(wall));
	}
	for (auto& alien : pLevelData.GetAliens())
	{
		m_aliens.push_back(std::make_unique<Alien>(alien));
	}
}

void World::Draw(sf::RenderWindow& renderTarget)
{
	for (auto& alien : m_aliens)
	{
		alien->Draw(renderTarget);
	}

	for (auto& wall : m_walls)
	{
		wall->Draw(renderTarget);
	}

	for (auto& bullet : m_bullets)
	{
		bullet->Draw(renderTarget);
	}

	m_player->Draw(renderTarget);
}

void World::Update(float dt)
{
	// listening and processing input from user
	using sfk = sf::Keyboard;

	if (sfk::isKeyPressed(sfk::A))
		m_player->MoveLeft(dt);
	else if (sfk::isKeyPressed(sfk::D))
		m_player->MoveRight(dt);
	else if (sfk::isKeyPressed(sfk::Space))
		m_bullets.push_back(std::make_unique<Bullet>(m_player->GetPosition(), 10, true));



	// updating all actors
	m_player->Update(dt);

	for (auto& wall : m_walls)
	{
		wall->Update(dt);
	}

	for (auto& bullet : m_bullets)
	{
		bullet->Update(dt);
	}

	// Checking all collisions

	std::vector<size_t> bulletsToErase;
	auto vm = sf::VideoMode::getDesktopMode();
	sf::Rect<float> displayArea{0,0, (float)vm.width, (float)vm.height};
	for (size_t j = 0; j < m_bullets.size(); j++)
	{
		auto& bullet = m_bullets[j];
		auto bulletHitBox = bullet->GetGlobalBoundingBox();

		bool collided = false;
		if (bullet->IsPlayerBullet())
		{
			for (size_t i = 0; i < m_aliens.size(); i++)
			{
				auto& alien = m_aliens[i];

				if (collided = alien->CollideWith(bulletHitBox))
				{
					m_aliens.erase(m_aliens.begin() + i);
					break;
				}

			}
		}
		else
		{
			collided = m_player->CollideWith(bulletHitBox);
		}

		
		if (collided ||
			!bullet->GetGlobalBoundingBox().intersects(displayArea))
		{
			bulletsToErase.push_back(j);
		}
	}

	for (auto n = bulletsToErase.rbegin(); n != bulletsToErase.rend(); n++)
	{
		m_bullets.erase(m_bullets.begin() + (*n));
	}
}
