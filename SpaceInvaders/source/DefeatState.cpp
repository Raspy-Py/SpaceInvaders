#include "DefeatState.h"
#include "Text.h"

#include <fstream>

DefeatState::DefeatState()
{
	auto vm = sf::VideoMode::getDesktopMode();

	m_widgetManager.AddWidget<Text>("DefeatTxt", "Defeat", "DefaultFont", 50, sf::Vector2f((float)vm.width / 2.0f, (float)vm.height / 2.0f - 25.0f), sf::Color(240, 60, 60));
	}

std::optional<SwitchStateParams> DefeatState::Update(float dt) noexcept
{
	m_widgetManager.Update(dt);
	m_timeElapsed += dt;

	if (m_timeElapsed >= m_lifeTime)
	{
		return std::optional<SwitchStateParams>("MainMenuState");
	}

	return std::nullopt;
}

void DefeatState::Draw(sf::RenderWindow& renderTarget) noexcept
{
	m_widgetManager.Draw(renderTarget);
}

void DefeatState::Leave() noexcept(NDEBUG)
{
	m_timeElapsed = 0.0f;
}

void DefeatState::Init(const std::map<std::string, std::string> args) noexcept(NDEBUG)
{
	auto pArgScore = args.find("score");

	if (pArgScore != args.end())
	{
		std::ofstream file("./data/high_score.txt");

		if (!file.is_open())
		{
			return;
		}

		file << pArgScore->second;

		file.close();
	}
}
