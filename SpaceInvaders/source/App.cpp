#include "App.h"
#include "StateCommon.h"

#include "WindowsBase.h"
#include "ResourceManager.h"

App::App()
{
	auto pRM = ResourceManager::GetInstance();
	pRM->LoadFont("data/fonts/default_font.otf");
	pRM->LoadFont("data/fonts/consolas.ttf", "consolas");
	pRM->LoadFont("data/fonts/pixel_font.ttf", "pixel_font");

	std::string tileMapPath = "data/textures/game_common.png";

	pRM->LoadTexture(tileMapPath, "WallWhole3", 51, 20, 26, 13);
	pRM->LoadTexture(tileMapPath, "WallLeft3", 51, 20, 13, 13);
	pRM->LoadTexture(tileMapPath, "WallRight3", 64, 20, 13, 13);

	pRM->LoadTexture(tileMapPath, "WallLeft2", 51, 36, 13, 13);
	pRM->LoadTexture(tileMapPath, "WallRight2", 64, 36, 13, 13);

	pRM->LoadTexture(tileMapPath, "WallLeft1", 51, 52, 13, 13);
	pRM->LoadTexture(tileMapPath, "WallRight1", 64, 52, 13, 13);

	pRM->LoadTexture(tileMapPath, "WallLeft0", 51, 68, 13, 13);
	pRM->LoadTexture(tileMapPath, "WallRight0", 64, 68, 13, 13);

	pRM->LoadTexture(tileMapPath, "AlienAndroid0", 3, 4, 11, 9);
	pRM->LoadTexture(tileMapPath, "AlienAndroid1", 19, 4, 11, 9);

	pRM->LoadTexture(tileMapPath, "AlienJellyfish0", 4, 20, 8, 8);
	pRM->LoadTexture(tileMapPath, "AlienJellyfish1", 20, 20, 8, 8);

	pRM->LoadTexture(tileMapPath, "AlienSkull0", 3, 36, 11, 8);
	pRM->LoadTexture(tileMapPath, "AlienSkull1", 19, 36, 11, 8);

	pRM->LoadTexture(tileMapPath, "AlienMask0", 4, 52, 8, 8);
	pRM->LoadTexture(tileMapPath, "AlienMask1", 20, 52, 8, 8);

	pRM->LoadTexture(tileMapPath, "AlienFunny0", 4, 68, 8, 8);
	pRM->LoadTexture(tileMapPath, "AlienFunny1", 20, 68, 8, 8);

	pRM->LoadTexture(tileMapPath, "BulletI", 38, 5, 3, 6);
	pRM->LoadTexture(tileMapPath, "BulletS", 38, 21, 3, 7);
	pRM->LoadTexture(tileMapPath, "BulletZ", 87, 21, 3, 7);

	pRM->LoadTexture(tileMapPath, "SpaceShip", 68, 4, 9, 10);
	pRM->LoadTexture(tileMapPath, "UFO", 48, 5, 16, 7);

	for (size_t i = 0; i < 6; i++)
		pRM->LoadTexture("data/textures/health_bars.png", "HealthBar" + std::to_string(i), 1, 3 + (5 - i) * 16, 30, 9);

	mp_window = std::make_unique<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "Space Invaders", sf::Style::Fullscreen);
	mp_stateManager = std::make_unique<StateManager>();

	mp_stateManager->AddState<MainMenuState>();
	mp_stateManager->AddState<EditorState>();
	mp_stateManager->AddState<GameState>();
	mp_stateManager->AddState<VictoryState>();
	mp_stateManager->AddState<DefeatState>();
	mp_stateManager->AddState<LevelMenuState>();
	mp_stateManager->AddState<ExitState>();
}

int App::Run()
{
	while (mp_window->isOpen())
	{
		sf::Event event;
		while (mp_window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				// TODO: if current State is GameState, handle it, before closing application
				// handleGameStateEnd();
				mp_window->close();
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			mp_window->close();
		}

		try
		{
			mp_stateManager->UpdateCurrent(m_clock.restart().asMilliseconds());
		}
		catch (const std::exception& e)
		{
			throw e;
		}

		mp_window->clear();
		mp_stateManager->DrawCurrent(*mp_window.get());
		mp_window->display();
	}

	return 0;
}
