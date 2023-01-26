#pragma once
#include "StateManager.h"

#include <SFML/Graphics.hpp>
#include <memory>

class App
{
	using WindowPtr = std::unique_ptr<sf::RenderWindow>;
	using StateManagerPtr = std::unique_ptr<StateManager>;
public:
	App();

	int Run();
private:
	StateManagerPtr mp_stateManager;
	WindowPtr mp_window;

	sf::Clock m_clock;
};

