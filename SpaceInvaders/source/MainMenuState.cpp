#include "MainMenuState.h"
#include "Text.h"
#include "Button.h"
#include "TextInput.h"
#include "Utils.h"

MainMenuState::MainMenuState()
{
	using sfc = sf::Color;
	using sfv = sf::Vector2f;

	auto vm = sf::VideoMode::getDesktopMode();

	m_widgetManager.AddWidget<Text>("TitleTxt", "Space Invaders", "DefaultFont", 50, sfv(240, 40), sfc(240, 240, 50, 255));
	m_widgetManager.AddWidget<Text>("ScoreLaybelTxt", "Hi-Score", "DefaultFont", 50, sfv(vm.width - 350, 40), sfc(240, 240, 50, 255));
	m_widgetManager.AddWidget<Text>("ScoreTxt", "0000", "DefaultFont", 60, sfv(vm.width - 350, 100), sfc(240, 240, 50, 255));
	
	m_widgetManager.AddWidget<Button>("PlayBtn", "Play", "DefaultFont", sfv(200, 70), sfv(125, vm.height - 350), sfc(0, 0, 0), sfc(40, 40, 40));
	m_widgetManager.AddWidget<Button>("EditorBtn", "Editor", "DefaultFont", sfv(200, 70), sfv(125, vm.height - 280), sfc(0, 0, 0), sfc(40, 40, 40));
	m_widgetManager.AddWidget<Button>("ExitBtn", "Exit", "DefaultFont", sfv(200, 70), sfv(125, vm.height - 210), sfc(0, 0, 0), sfc(40, 40, 40));

	LoadHightScore();

	//m_widgetManager.AddWidget<TextInput>("TestInpt", "consolas", sfv(300, 70), sfv(500, 500), sfc(30, 30, 30), sfc(40, 40, 40));
}

std::optional<SwitchStateParams> MainMenuState::Update(float dt) noexcept
{
	m_widgetManager.Update(dt);

	try
	{
		if (m_widgetManager.CheckIf("EditorBtn", Widget::Command::Type::Pressed))
			return std::optional<SwitchStateParams>("EditorState");
		else if (m_widgetManager.CheckIf("PlayBtn", Widget::Command::Type::Pressed))
			return std::optional<SwitchStateParams>("LevelMenuState");
		else if (m_widgetManager.CheckIf("ExitBtn", Widget::Command::Type::Pressed))
			return std::optional<SwitchStateParams>("ExitState");
	}
	catch (const std::exception& e)
	{
		m_debugText.setString(e.what());
		m_drawDebugText = true;
	}

	return std::nullopt;
}

void MainMenuState::Draw(sf::RenderWindow& renderTarget) noexcept
{
	m_widgetManager.Draw(renderTarget);
	renderTarget.draw(m_debugText);
}

void MainMenuState::Leave() noexcept(NDEBUG)
{

}

void MainMenuState::Init(const std::map<std::string, std::string> args) noexcept(NDEBUG)
{
	LoadHightScore();
}

void MainMenuState::LoadHightScore()
{
	std::ifstream file("./data/high_score.txt");
	std::string buf;

	if (!file.is_open() || !std::getline(file, buf))
	{
		m_widgetManager.GetWidget<Text>("ScoreTxt").SetString("----");
		return;
	}

	m_widgetManager.GetWidget<Text>("ScoreTxt").SetString(buf);

	file.close();
}
