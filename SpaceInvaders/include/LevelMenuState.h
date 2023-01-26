#pragma once
#include "State.h"
#include "Alien.h"

class LevelMenuState : public State
{
	using LevelPreview = std::pair<std::string, std::unique_ptr<LevelData>>;
public:
	LevelMenuState();

	std::optional<SwitchStateParams> Update(float dt) noexcept override;
	void Draw(sf::RenderWindow& renderTarget) noexcept override;
	void Leave() noexcept(NDEBUG)override;
	void Init(const std::map<std::string, std::string> args) noexcept(NDEBUG)override;
private:
	size_t m_displayedLevel = 0;
	std::vector<std::unique_ptr<Actor>> m_actors;
	std::vector<LevelPreview> m_levelPreviews;
	float m_btnCooldownTime = 300;
	float m_btnTimeElapsed = 0;
};

 