#include "GameState.h"
#include "Text.h"
#include "ResourceManager.h"

GameState::GameState()
{
	using sfv = sf::Vector2f;
	using sfc = sf::Color;
	auto vm = sf::VideoMode::getDesktopMode();
	auto pRM = ResourceManager::GetInstance();

	sfv pgSize = { 0.8f * (float)vm.height, (float)vm.height };
	sfv pgPos = { ((float)vm.width - pgSize.x) / 2.0f, 0 };
	sfv lbPos = pgPos;
	sfv rbPos = pgPos + sfv(pgSize.x, 0);
	sfv bbSize = {rbPos.x - lbPos.x, 3.0f};
	sfv bbPos = { lbPos.x, (float)vm.height * 0.9f };

	m_leftBorder.setFillColor(sfc(255, 255, 255));
	m_leftBorder.setSize(sfv(3, (float)vm.height));
	m_leftBorder.setPosition(lbPos);
	m_rightBorder = m_leftBorder;
	m_rightBorder.setPosition(rbPos);
	m_bottomBorder.setFillColor(sfc(255, 255, 255));
	m_bottomBorder.setPosition(bbPos);
	m_bottomBorder.setSize(bbSize);

	m_leftSpace = { sfv{0,0}, sfv{pgPos.x, pgSize.y} };
	m_rightSpace = { sfv{pgPos.x + pgSize.x,0}, sfv{pgPos.x, pgSize.y} };

	m_debugInfo.setPosition(20, 100);
	m_debugInfo.setCharacterSize(20);
	m_debugInfo.setFillColor(sf::Color(240, 60, 60));
	m_debugInfo.setFont(pRM->GetFont("consolas"));

	m_pauseBackground.setPosition(0, 0);
	m_pauseBackground.setSize(sfv((float)vm.width, (float)vm.height));
	m_pauseBackground.setFillColor(sfc(0, 0, 0, 150));

	m_widgetManager.AddWidget<Text>("PauseTxt", "Pause", "DefaultFont", 40, sfv((float)vm.width / 2.0f, (float)vm.height / 2.0f - 20.0f));
	ResetState();
}

std::optional<SwitchStateParams> GameState::Update(float dt) noexcept
{


	// checking pause
	m_tabElapsed += dt;
	using sfk = sf::Keyboard;
	if (sfk::isKeyPressed(sfk::Tab) && m_tabElapsed >= m_tabCooldown)
	{
		m_tabElapsed = 0.0f;
		m_isOnPause = !m_isOnPause;
	}

	if (m_isOnPause)
	{
		return std::nullopt;
	}

	if (auto endGameResult = CheckEndGame())
	{
		return endGameResult;
	}

	// incrementing all clocks;
	m_playerGunElapsed += dt;
	m_alienGunElapsed += dt;

	// reading input from user


	if (sfk::isKeyPressed(sfk::A))
	{
		m_player->MoveLeft(dt);
	}
	else if (sfk::isKeyPressed(sfk::D))
	{
		m_player->MoveRight(dt);
	}

	if (sfk::isKeyPressed(sfk::Space) && m_playerGunElapsed >= m_playerGunCooldown)
	{
		m_bullets.emplace_back(m_player->GetPosition(), 1.0, true);
		m_playerGunElapsed = 0.0f;
	}


	// checking in-world changes
	int initAlien = rand() % m_aliens.size();
	if (m_alienGunElapsed > m_alienGunCooldown)
	for (int i = initAlien;;)
	{
		auto& alien = m_aliens[i];
		{
			int randN = rand() % 100;
			float chanceToFire =  100.0f * m_alienShotsPerSecond / (float)m_aliens.size();

			bool gunFired = (float)randN < chanceToFire;

			if (gunFired)
			{
				m_bullets.emplace_back(alien.GetPosition(), 0.7f, false);
				m_alienGunElapsed = 0.0f;
			} 
		}
		i = (i + 1) % m_aliens.size();
		if (i == initAlien) break;
	}
	if (auto strafe = NeedChangeDirection())
	{
		for (auto& alien : m_aliens)
		{
			alien.ChangeDirection(strafe.value());
			alien.MoveDown();
			m_alienBatchBounds.top -= (float)sf::VideoMode::getDesktopMode().height / 100.0f;
		}
	}

	if (m_player->GetGlobalBoundingBox().intersects(m_leftSpace))
	{
		sf::Vector2f pos = m_player->GetPosition();
		pos.x = m_leftBorder.getPosition().x;
		m_player->SetPosition(pos);
	}
	else if (m_player->GetGlobalBoundingBox().intersects(m_rightSpace))
	{
		sf::Vector2f pos = m_player->GetPosition();
		pos.x = m_rightBorder.getPosition().x - m_player->GetGlobalBoundingBox().width;
		m_player->SetPosition(pos);
	}

	CheckCollisions();

	try
	{
		// getting input from widgets
	}
	catch (std::exception& e)
	{
		// handling
	}

	for (auto& alien : m_aliens)
	{
		alien.Update(dt);
	}

	for (auto& wall : m_walls)
	{
		wall.Update(dt);
	}

	for (auto& bullet : m_bullets)
	{
		bullet.Update(dt);
	}

	UpdateAlienBatchBounds();

	m_player->Update(dt);
	m_healthBar->Update(dt);

	std::string debugString = 
		"aliens: " + std::to_string(m_aliens.size()) + 
		"\nwalls: " + std::to_string(m_walls.size()) +
		"\nbullets: " + std::to_string(m_bullets.size()) + 
		"\nscore: " + std::to_string(m_score);

	m_debugInfo.setString(debugString);

	return std::nullopt;
}

void GameState::Draw(sf::RenderWindow& renderTarget) noexcept
{
	// Static objects
	renderTarget.draw(m_leftBorder);
	renderTarget.draw(m_rightBorder);
	renderTarget.draw(m_bottomBorder);

	// Actors
	for (auto& alien : m_aliens)
	{
		alien.Draw(renderTarget);
	}

	for (auto& wall : m_walls)
	{
		wall.Draw(renderTarget);
	}

	for (auto& bullet : m_bullets)
	{
		bullet.Draw(renderTarget);
	}

	m_player->Draw(renderTarget);
	m_healthBar->Draw(renderTarget);
	
	renderTarget.draw(m_debugInfo);

	if (m_isOnPause)
	{
		renderTarget.draw(m_pauseBackground);
		m_widgetManager.Draw(renderTarget);
	}
}

void GameState::Leave() noexcept(NDEBUG)
{
	ResetState();
}

void GameState::Init(const std::map<std::string, std::string> args) noexcept(NDEBUG)
{
	auto pRM = ResourceManager::GetInstance();
	auto pArgLevel = args.find("level");

	if (pArgLevel == args.end())
	{
		return;
	}
	m_levelName = pArgLevel->second;

	auto pArgDifficulty = args.find("difficulty");
	auto pArgScore = args.find("score");
	
	m_difficulty = ((pArgDifficulty != args.end()) ? stoi(pArgDifficulty->second) : 5);
	m_score = ((pArgScore != args.end()) ? stoi(pArgScore->second) : 0);

	try
	{
		auto& level = pRM->GetLevelData(m_levelName);
		m_aliens = level.GetAliens();
		m_walls = level.GetWalls();
		m_player = std::make_unique<Player>();
		m_healthBar = std::make_unique<HealthBar>();
		m_initAliensCount = m_aliens.size();

		UpdateAlienBatchBounds();
		SetGameSetting();
		UpdateAlienSpeedScale();
	}
	catch (std::exception& e)
	{
		pRM->LoadLevelData("data/levels/" + m_levelName + ".lvl", m_levelName);
		Init(args);
	}
}

void GameState::UpdateAlienBatchBounds()
{
	using sfv = sf::Vector2f;
	auto vm = sf::VideoMode::getDesktopMode();
	m_alienBatchBounds = { sfv((float)vm.width / 2.0f, 0), sfv(0,0) };

	for (auto& alien : m_aliens)
	{
		auto& bb = alien.GetGlobalBoundingBox();

		if (bb.left < m_alienBatchBounds.left)
		{
			m_alienBatchBounds.left = bb.left;
		}
		if (bb.top < m_alienBatchBounds.top)
		{
			m_alienBatchBounds.top = bb.top;
		}

		if (bb.left + bb.width > m_alienBatchBounds.left + m_alienBatchBounds.width)
		{
			m_alienBatchBounds.width = bb.left + bb.width - m_alienBatchBounds.left;
		}
		if (bb.top + bb.height > m_alienBatchBounds.top + m_alienBatchBounds.height)
		{
			m_alienBatchBounds.height = bb.top + bb.height - m_alienBatchBounds.top;
		}
	}
}

void GameState::UpdateAlienSpeedScale()
{
	float countCoef = 1.0 + float(m_initAliensCount - m_aliens.size()) / m_initAliensCount * 1.5f;
	m_alienSpeedScale = m_initAlienSpeed * countCoef;

 	for (auto& alien : m_aliens)
	{
		alien.SetSpeedScale(m_alienSpeedScale);
	}
}

void GameState::SetGameSetting()
{
	float difMode = pow(1.2f, m_difficulty);
	float s = m_rightBorder.getPosition().x - m_leftBorder.getPosition().x - m_alienBatchBounds.width;

	m_initAlienSpeed = m_alienSpeedScale = s / m_oneSlideTime * difMode;
	m_oneSlideTime /= difMode;
	m_alienGunCooldown /= difMode;
	m_alienShotsPerSecond *= difMode;

	m_playerGunCooldown = std::min((100.0f * m_oneSlideTime) / m_aliens.size() * 0.4f, 1000.0f);
}

std::optional<SwitchStateParams> GameState::CheckEndGame()
{
	// Checking for game end
	if (m_player->GetHealth() <= 0 || m_alienBatchBounds.top + m_alienBatchBounds.height > m_leftBorder.getSize().y)
	{
		m_bullets.clear();
		m_aliens.clear();
		m_walls.clear();
		std::map<std::string, std::string> args = {
			{"score", std::to_string(m_score)}
		};
		return std::optional<SwitchStateParams>({ "DefeatState", args });
	}

	if (m_aliens.size() == 0)
	{
		m_bullets.clear();
		m_aliens.clear();
		m_walls.clear();
		std::map<std::string, std::string> args = {
			{"level",		m_levelName},
			{"score",		std::to_string(m_score)},
			{"difficulty",	std::to_string(m_difficulty + 1)}
		};
		return std::optional<SwitchStateParams>({ "VictoryState" , args });
	}

	return std::nullopt;
}

std::optional<float> GameState::NeedChangeDirection()
{

	float left = m_alienBatchBounds.left;
	float right = m_alienBatchBounds.left + m_alienBatchBounds.width;

	if (right >= m_rightBorder.getPosition().x)
	{
		return std::optional<float>(right - m_rightBorder.getPosition().x);
	}
	if (left <= m_leftBorder.getPosition().x)
	{
		return std::optional<float>(m_leftBorder.getPosition().x - left);
	}

	return std::nullopt;



	return std::optional<float>();
}

void GameState::CheckCollisions()
{
	for (int bulletIndex = m_bullets.size() - 1; bulletIndex >= 0; bulletIndex--)
	{
		auto& bullet = m_bullets[bulletIndex];

		if (bullet.GetPosition().y < 0 ||
			bullet.GetPosition().y > m_leftBorder.getSize().y)
		{
			m_bullets.erase(m_bullets.begin() + bulletIndex);
			continue;
		}

		if (bullet.IsPlayerBullet())
		{
			for (size_t alienIndex = 0; alienIndex < m_aliens.size(); alienIndex++)
			{
				if (m_aliens[alienIndex].GetGlobalBoundingBox().intersects(bullet.GetGlobalBoundingBox()))
				{
					m_aliens.erase(m_aliens.begin() + alienIndex);
					m_bullets.erase(m_bullets.begin() + bulletIndex);
					UpdateAlienBatchBounds();
					UpdateAlienSpeedScale();
					m_score += 50;
					break;
				}
			}
		}
		else
		{
			bool wallHit = false;
			for (size_t wallIndex = 0; wallIndex < m_walls.size(); wallIndex++)
			{
				auto& wall = m_walls[wallIndex];

				if (wall.CollideWith(bullet.GetGlobalBoundingBox()))
				{
					m_bullets.erase(m_bullets.begin() + bulletIndex);
					wallHit = true;
					break;
				}
			}

			if (!wallHit && m_player->GetGlobalBoundingBox().intersects(bullet.GetGlobalBoundingBox()))
			{
				m_player->SetHealth(m_player->GetHealth() - 1);
				m_healthBar->SetHealth(m_player->GetHealth());
				m_bullets.erase(m_bullets.begin() + bulletIndex);
			}
		}
	}
}

void GameState::ResetState()
{
	m_playerGunCooldown = 700.0f;
	m_playerGunElapsed = 0.0f;

	m_alienGunCooldown = 500.0f;
	m_alienGunElapsed = 0.0f;
	m_alienShotsPerSecond = 0.0003f;

	m_initAlienSpeed = 1.0f;
	m_oneSlideTime = 2000.0f;
	m_alienSpeedScale = 1.0f;
	m_initAliensCount = 0;

	m_score = 0;
	m_difficulty = 0;
}
