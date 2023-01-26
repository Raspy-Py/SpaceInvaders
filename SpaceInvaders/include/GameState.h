#pragma once
#include "State.h"
#include "Alien.h"
#include "Wall.h"
#include "Player.h"
#include "Bullet.h"
#include "HealthBar.h"


class GameState : public State
{
public:
	GameState();

	std::optional<SwitchStateParams> Update(float dt) noexcept override;
	void Draw(sf::RenderWindow& renderTarget) noexcept override;
	void Leave() noexcept(NDEBUG)override;
	void Init(const std::map<std::string, std::string> args) noexcept(NDEBUG)override;

private:
	void UpdateAlienBatchBounds();
	void UpdateAlienSpeedScale();
	void SetGameSetting();

	std::optional<SwitchStateParams> CheckEndGame();
	std::optional<float> NeedChangeDirection();
	void CheckCollisions();

	void ResetState();
private:

	std::vector<Alien> m_aliens;
	std::vector<Wall> m_walls;
	std::vector<Bullet> m_bullets;
	std::unique_ptr<Player> m_player;
	std::unique_ptr<HealthBar> m_healthBar;
	std::unique_ptr<Alien> m_UFO;

	sf::RectangleShape m_leftBorder;
	sf::RectangleShape m_rightBorder;
	sf::RectangleShape m_bottomBorder;
	sf::RectangleShape m_pauseBackground;
	
	sf::Rect<float> m_leftSpace;
	sf::Rect<float> m_rightSpace;


	sf::Rect<float> m_alienBatchBounds;
	std::string m_levelName;

	bool m_isOnPause = false;
	float m_tabCooldown = 400.0f;
	float m_tabElapsed = 0.0f;

	float m_playerGunCooldown = 700.0f;
	float m_playerGunElapsed = 0.0f;

	float m_alienGunCooldown = 500.0f;
	float m_alienGunElapsed = 0.0f;
	float m_alienShotsPerSecond = 5.0f;

	float m_initAlienSpeed = 1.0f;
	float m_oneSlideTime = 2000.0f;
	float m_alienSpeedScale = 1.0f;
	int m_initAliensCount = 0;

	int m_score;
	int m_difficulty;

	sf::Text m_debugInfo;
};

