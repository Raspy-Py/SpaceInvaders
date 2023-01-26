#include "ExitState.h"

ExitState::ExitState()
{
}

std::optional<SwitchStateParams> ExitState::Update(float dt) noexcept
{
	return std::optional<SwitchStateParams>();
}

void ExitState::Draw(sf::RenderWindow& renderTarget) noexcept
{
	renderTarget.close();
}

void ExitState::Leave() noexcept(NDEBUG)
{
}

void ExitState::Init(const std::map<std::string, std::string> args) noexcept(NDEBUG)
{
}
