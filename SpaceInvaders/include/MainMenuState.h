#pragma once
#include "State.h"

class MainMenuState : public State
{
public:
	MainMenuState();

	std::optional<SwitchStateParams> Update(float dt) noexcept override;
	void Draw(sf::RenderWindow& renderTarget) noexcept override;
	void Leave() noexcept(NDEBUG) override;
	void Init(const std::map<std::string, std::string> args) noexcept(NDEBUG)override;

private:
	void LoadHightScore();
};

