#pragma once
#include "State.h"
class VictoryState : public State
{
public:
	VictoryState();

	std::optional<SwitchStateParams> Update(float dt) noexcept override;
	void Draw(sf::RenderWindow& renderTarget) noexcept override;
	void Leave() noexcept(NDEBUG)override;
	void Init(const std::map<std::string, std::string> args) noexcept(NDEBUG)override;

private:
	std::map<std::string, std::string> m_args;
	float m_lifeTime = 2000.0f;
	float m_timeElapsed = 0.0f;
};

