#pragma once
#include "Widget.h"
#include "WidgetManager.h"
#include "ResourceManager.h"

#include <queue>
#include <optional>

struct SwitchStateParams
{
	std::string targetStateName;
	std::map<std::string, std::string> args;

	SwitchStateParams() = default;

	SwitchStateParams(const std::string& inTargetStateName, std::map<std::string, std::string> inArgs) noexcept
	{
		targetStateName = inTargetStateName;
		args = std::move(inArgs);
	}
	SwitchStateParams(const std::string& inTargetStateName) noexcept
	{
		targetStateName = inTargetStateName;
	}
	SwitchStateParams(SwitchStateParams&& old) noexcept
	{
		targetStateName = std::move(old.targetStateName);
		args = std::move(old.args);
	}
};

class State
{
public:
	State()
	{
		auto& font = ResourceManager::GetInstance()->GetFont("consolas");

		m_debugText.setFont(font);
		m_debugText.setFillColor(sf::Color(255, 40, 40, 255));
		m_debugText.setCharacterSize(40);
		m_debugText.setPosition(50, 50);
	}

	virtual std::optional<SwitchStateParams> Update(float dt) noexcept = 0;
	virtual void Draw(sf::RenderWindow& renderTarget) noexcept = 0;
	virtual void Leave() noexcept(NDEBUG) = 0;
	virtual void Init(std::map<std::string, std::string> args) noexcept(NDEBUG) = 0;

protected:
	WidgetManager m_widgetManager;
	sf::Text m_debugText;
	bool m_drawDebugText = false;
};

