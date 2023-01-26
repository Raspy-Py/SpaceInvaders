#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "Actor.h"
#include "Alien.h"
#include "Wall.h"
#include "Player.h"
#include "Widget.h"
#include "Bullet.h"

class World : public Widget
{
public:
	World(const std::string& levelName);

	void Draw(sf::RenderWindow& renderTarget) override;
	void Update(float dt) override;

private:
	std::vector<std::unique_ptr<Alien>> m_aliens;
	std::vector<std::unique_ptr<Wall>> m_walls;
	std::vector<std::unique_ptr<Bullet>> m_bullets;
	std::unique_ptr<Player> m_player;

	bool m_aliensMovingRight = true;
	sf::Rect<float> m_playableArea;
	sf::Rect<float> m_leftNotPlayableZone;
	sf::Rect<float> m_rightNotPlayableZone;
};

