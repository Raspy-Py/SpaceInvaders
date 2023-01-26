#include "LevelMenuState.h"
#include "Text.h"
#include "Button.h"
#include "Utils.h"

#include <filesystem>

LevelMenuState::LevelMenuState()
{
	using sfv = sf::Vector2f;
	using sfc = sf::Color;

	auto vm = sf::VideoMode::getDesktopMode();

	m_widgetManager.AddWidget<Text>("LevelNameTxt", "NO_NAME", "pixel_font", 40, sfv(vm.width / 2.0f, 150), sfc(60, 200, 60));
	m_widgetManager.AddWidget<Button>("BackBtn", "Back", "DefaultFont", sfv(120, 50), sfv(20, vm.height - 70), sfc(0, 0, 0), sfc(40, 40, 40));
	m_widgetManager.AddWidget<Button>("PrevBtn", "Prev", "DefaultFont", sfv(120, 50), sfv(20, vm.height / 2 - 25), sfc(0, 0, 0), sfc(40, 40, 40));
	m_widgetManager.AddWidget<Button>("NextBtn", "Next", "DefaultFont", sfv(120, 50), sfv(vm.width - 140, vm.height / 2 - 25), sfc(0, 0, 0), sfc(40, 40, 40));
	m_widgetManager.AddWidget<Button>("StartBtn", "Start", "DefaultFont", sfv(120, 50), sfv(vm.width/2.0f - 60, vm.height  - 100), sfc(0, 0, 0), sfc(40, 40, 40));
}

std::optional<SwitchStateParams> LevelMenuState::Update(float dt) noexcept
{
	m_widgetManager.Update(dt);
	m_btnTimeElapsed += dt;

	for (auto& actor : m_actors)
	{
		actor->Update(dt);
	}

	std::map<std::string, std::string> args;

	try
	{
		if (m_widgetManager.CheckIf("BackBtn", Widget::Command::Type::Pressed))
			return std::optional<SwitchStateParams>("MainMenuState");
		else if (m_widgetManager.CheckIf("StartBtn", Widget::Command::Type::Pressed))
		{
			std::map<std::string, std::string> args = { {"level", m_levelPreviews[m_displayedLevel].first}};
			return std::optional<SwitchStateParams>({ "GameState", std::move(args)});
		}

		else if (m_widgetManager.CheckIf("NextBtn", Widget::Command::Type::Pressed) && m_btnTimeElapsed > m_btnCooldownTime)
		{
			if(m_displayedLevel < m_levelPreviews.size()-1) m_displayedLevel++;
			m_btnTimeElapsed = 0;
		}
		else if (m_widgetManager.CheckIf("PrevBtn", Widget::Command::Type::Pressed) && m_btnTimeElapsed > m_btnCooldownTime)
		{
			if (m_displayedLevel > 0) m_displayedLevel--;
			m_btnTimeElapsed = 0;
		}
	}
	catch (const std::exception& e)
	{
		m_drawDebugText = true;
		m_debugText.setString(e.what());
	}

	m_widgetManager.GetWidget<Text>("LevelNameTxt").SetString(m_levelPreviews[m_displayedLevel].first);

	return std::nullopt;
}

void LevelMenuState::Draw(sf::RenderWindow& renderTarget) noexcept
{
	m_widgetManager.Draw(renderTarget);
	
	if (m_drawDebugText)
	{
		renderTarget.draw(m_debugText);
	}

	for (auto& actor : m_actors)
	{
		actor->Draw(renderTarget);
	}

	if (m_displayedLevel < m_levelPreviews.size())
	{
		auto level = m_levelPreviews[m_displayedLevel].second.get();
		for (auto& alien : level->GetAliens())
		{
			alien.Draw(renderTarget);
		}

		for (auto& wall : level->GetWalls())
		{
			wall.Draw(renderTarget);
		}
	}
}

void LevelMenuState::Leave() noexcept(NDEBUG)
{
	auto pRM = ResourceManager::GetInstance();
	for (auto& preview : m_levelPreviews)
	{
		pRM->UnloadLevelData(preview.first);
	}

	m_levelPreviews.clear();
	m_displayedLevel = 0;
}

void LevelMenuState::Init(const std::map<std::string, std::string> args) noexcept(NDEBUG)
{
	for (auto& entry : std::filesystem::directory_iterator("data/levels/"))
	{
		std::string path = entry.path().string();
		auto level = std::make_pair(GetNameFromPath(entry.path().string()), LevelData::LoadFromFile(path, true));
		m_levelPreviews.emplace_back(std::move(level));
	}

	m_widgetManager.GetWidget<Text>("LevelNameTxt").SetString(m_levelPreviews[m_displayedLevel].first);
}

