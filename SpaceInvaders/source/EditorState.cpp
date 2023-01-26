#include "EditorState.h"
#include "Text.h"
#include "Button.h"
#include "TextInput.h"
#include "ResourceManager.h"

#include <algorithm>

EditorState::EditorState()
{
	using sfc = sf::Color;
	using sfv = sf::Vector2f;

	auto vm = sf::VideoMode::getDesktopMode();
	auto pRM = ResourceManager::GetInstance();

	// editor objects' params
	sfv pgSize	= { 0.8f * vm.height, (float)vm.height * 0.9f };
	sfv pgPos	= { ((float)vm.width - pgSize.x) / 2.0f, 0 };
	sfv sSize	= { ((float)vm.width - pgSize.x) / 2.0f, (float)vm.height / 2.0f };
	sfv sPos	= { ((float)vm.width + pgSize.x) / 2.0f, 0 };
	sfv btnSize = { ((float)vm.width - pgSize.x) / 4.0f, 50 };
	sfv btnPos	= { ((float)vm.width + pgSize.x) / 2.0f + 20, (float)vm.height / 2.0f + 20.0f };
	sfv inpSize = { ((float)vm.width - pgSize.x) / 2.0f - 40.0f, 50};

	m_widgetManager.AddWidget<Text>("StateNameTxt", "Editor", "DefaultFont", 20, sfv(120.0, 20.0));
	m_widgetManager.AddWidget<Button>("BackBtn", "Back", "DefaultFont", sfv(120, 50), sfv(20, vm.height - 70), sfc(10, 10, 10), sfc(40, 40, 40));
	m_widgetManager.AddWidget<TextInput>("LevelNameInpt", "consolas", inpSize, btnPos, sfc(30, 30, 30), sfc(40, 40, 40));

	m_widgetManager.AddWidget<Button>("SaveBtn", "Save", "DefaultFont", btnSize, btnPos + sfv{ 0, 60 }, sfc(10, 10, 10), sfc(40, 40, 40));
	m_widgetManager.AddWidget<Button>("LoadBtn", "Load", "DefaultFont", btnSize, btnPos + sfv{0, 120}, sfc(10, 10, 10), sfc(40, 40, 40));
	m_widgetManager.AddWidget<Button>("ClearBtn", "Clear", "DefaultFont", btnSize, btnPos + sfv{ 0, 180 }, sfc(10, 10, 10), sfc(40, 40, 40));

	m_playAreaBackground.setFillColor(sfc(20, 20, 50));
	m_playAreaBackground.setSize(pgSize);
	m_playAreaBackground.setPosition(pgPos);

	m_storeBackground.setFillColor(sfc(50, 50, 0));
	m_storeBackground.setSize(sSize);
	m_storeBackground.setPosition(sPos);

	m_safeLine.setFillColor(sfc(255,255,255));
	//m_safeLine.setSize(sfv(1000, 3));
	m_safeLine.setPosition((vm.width - 1000) / 2.0f, vm.height-100);

	m_editedLevelData = std::make_unique<LevelData>();

	m_alienTypes = { "Android", "Jellyfish", "Skull", "Mask", "Funny"};
	m_wallTypes = { "Left" , "Right", "Whole" };

	float scale = (float)sf::VideoMode::getDesktopMode().height / 1080 * 4.0f;

	for (auto& alienType : m_alienTypes)
	{
		m_aliens.emplace_back(pRM->GetTexture("Alien" + alienType + "0"));// a + b + c -> plus(plus(a, b), c)
	}
	for (auto& wallType : m_wallTypes)
	{
		m_walls.emplace_back(pRM->GetTexture("Wall" + wallType + "3"));
	}

	for (size_t i = 0; i < m_walls.size(); i++)
	{
		m_walls[i].setScale(scale, scale);
		int row = i / 3;
		int col = i % 3;
		m_walls[i].setPosition(m_storeBackground.getPosition() + sfv(10.0f, 10.0f) + sfv(col * 70, row * 60));
	}
	for (size_t i = 0; i < m_aliens.size(); i++)
	{
		m_aliens[i].setScale(scale, scale);
		int row = i / 3;
		int col = i % 3;
		m_aliens[i].setPosition(m_storeBackground.getPosition() + sfv(10.0f, 80.0f) + sfv(col * 60, row * 60));
	}
}

std::optional<SwitchStateParams> EditorState::Update(float dt) noexcept
{
	// Reading input from user
	sf::Vector2i mpRaw = sf::Mouse::getPosition();
	m_mousePosition = {
		static_cast<float>(mpRaw.x),
		static_cast<float>(mpRaw.y)
	};

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (m_storeBackground.getGlobalBounds().contains(m_mousePosition))
		{
			if (m_objectChosen)
			{
				m_objectChosen = false;
			} 

			for (size_t i = 0; i < m_aliens.size(); i++)
			{
				auto& alien = m_aliens[i];
				if (alien.getGlobalBounds().contains(m_mousePosition))
				{
					m_cursorSprite = &alien;
					m_objectChosen = true;
					m_chosenObjectType = m_alienTypes[i];
				}
			}
			for (size_t i = 0; i < m_walls.size(); i++)
			{
				auto& wall = m_walls[i];
				if (wall.getGlobalBounds().contains(m_mousePosition))
				{
					m_cursorSprite = &wall;
					m_objectChosen = true;
					m_chosenObjectType = m_wallTypes[i];
				}
			}
		}
		else if (m_objectChosen && IsCursorSpriteInPlayArea())
		{
			if (std::find(m_wallTypes.begin(), m_wallTypes.end(), m_chosenObjectType) != m_wallTypes.end())
				m_editedLevelData->AddWall(m_mousePosition, m_chosenObjectType);
			else
				m_editedLevelData->AddAlien(m_mousePosition, m_chosenObjectType);
		}
		else
		{
			// reset cursor
			m_objectChosen = false;
		}
	}

	// Default update actions
	m_widgetManager.Update(dt);

	try
	{
		if (m_widgetManager.CheckIf("BackBtn", Widget::Command::Type::Pressed))
			return std::optional<SwitchStateParams>("MainMenuState");
		else if (m_widgetManager.CheckIf("ClearBtn", Widget::Command::Type::Pressed))
			m_editedLevelData->Clear();
		else if (m_widgetManager.CheckIf("LoadBtn", Widget::Command::Type::Pressed))
			m_editedLevelData = LevelData::LoadFromFile("./data/levels/" + m_widgetManager.ReadInputFrom("LevelNameInpt") + ".lvl", false);
		else if (m_widgetManager.CheckIf("SaveBtn", Widget::Command::Type::Pressed))
			LevelData::WriteToFile("./data/levels/" + m_widgetManager.ReadInputFrom("LevelNameInpt") + ".lvl", *m_editedLevelData);
	}
	catch (const std::exception& e)
	{
		m_drawDebugText = true;
		m_debugText.setString(e.what());
		m_debugStringLifeTime = m_maxlifeTime;
	}

	m_debugStringLifeTime = std::max(0.0f, m_debugStringLifeTime - dt);
	m_debugText.setFillColor(sf::Color(255, 40, 40, (int(m_debugStringLifeTime / m_maxlifeTime * 255))));

	return std::nullopt;
}

void EditorState::Draw(sf::RenderWindow& renderTarget) noexcept
{
	renderTarget.draw(m_storeBackground);
	renderTarget.draw(m_playAreaBackground);
	renderTarget.draw(m_safeLine);

	m_widgetManager.Draw(renderTarget);
	if(m_debugStringLifeTime > 0)
		renderTarget.draw(m_debugText);

	for (auto& alien : m_aliens)
	{
		renderTarget.draw(alien);
	}
	for (auto& wall : m_walls)
	{
		renderTarget.draw(wall);
	}


	// placed objects
	for (const auto& alien : m_editedLevelData->GetAliens())
	{
		alien.Draw(renderTarget);
	}
	for (const auto& wall : m_editedLevelData->GetWalls())
	{
		wall.Draw(renderTarget);
	}

	if (m_objectChosen && IsCursorSpriteInPlayArea())
	{
		if (m_playAreaBackground.getGlobalBounds().contains(m_mousePosition))renderTarget.setMouseCursorVisible(false);
		auto savePos = m_cursorSprite->getPosition();
		m_cursorSprite->setPosition(m_mousePosition);
		renderTarget.draw(*m_cursorSprite);
		m_cursorSprite->setPosition(savePos);
	}
	else
	{
		renderTarget.setMouseCursorVisible(true);
	}
}

void EditorState::Leave() noexcept(NDEBUG)
{
	m_objectChosen = false;
	//m_editedLevelData.Clear();
}

void EditorState::Init(const std::map<std::string, std::string> args) noexcept(NDEBUG)
{
}

bool EditorState::IsCursorSpriteInPlayArea()
{
	if (m_cursorSprite == nullptr) return false;

	auto savePos = m_cursorSprite->getPosition();
	m_cursorSprite->setPosition((float)sf::Mouse::getPosition().x, (float)sf::Mouse::getPosition().y);

	auto gb = m_cursorSprite->getGlobalBounds();
	m_cursorSprite->setPosition(savePos);
	sf::Vector2f lt = { gb.left, gb.top };
	sf::Vector2f rt = { gb.left + gb.width, gb.top };
	sf::Vector2f lb = { gb.left, gb.top + gb.height };
	sf::Vector2f rb = { gb.left + gb.width, gb.top + gb.height };

	auto pa = m_playAreaBackground.getGlobalBounds();
	return pa.contains(lt) && pa.contains(rt) && pa.contains(lb) && pa.contains(rb);
}
