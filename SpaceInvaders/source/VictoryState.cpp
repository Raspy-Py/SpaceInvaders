#include "VictoryState.h"
#include "Text.h"

VictoryState::VictoryState()
{
	auto vm = sf::VideoMode::getDesktopMode();

	m_widgetManager.AddWidget<Text>("VictoryTxt", "Victory", "DefaultFont", 50, sf::Vector2f((float)vm.width/2.0f,(float)vm.height / 2.0f - 25.0f), sf::Color(60,240,60));
}

std::optional<SwitchStateParams> VictoryState::Update(float dt) noexcept
{
	m_widgetManager.Update(dt);
	m_timeElapsed += dt;

	if (m_timeElapsed >= m_lifeTime)
	{
		return std::optional<SwitchStateParams>({ "GameState", std::move(m_args) });
	}
	return std::nullopt;
}

void VictoryState::Draw(sf::RenderWindow& renderTarget) noexcept
{
	m_widgetManager.Draw(renderTarget);
}

void VictoryState::Leave() noexcept(NDEBUG)
{
	m_timeElapsed = 0.0f;
}

void VictoryState::Init(const std::map<std::string, std::string> args) noexcept(NDEBUG)
{
	m_args = args;
}
