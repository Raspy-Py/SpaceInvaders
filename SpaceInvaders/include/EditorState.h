#pragma once
#include "State.h"
#include "LevelData.h"

class EditorState : public State
{
public:
	EditorState();

	std::optional<SwitchStateParams> Update(float dt) noexcept override;
	void Draw(sf::RenderWindow& renderTarget) noexcept override;
	void Leave() noexcept(NDEBUG)override;
	void Init(const std::map<std::string, std::string> args) noexcept(NDEBUG)override;


private:
	bool IsCursorSpriteInPlayArea();

	std::unique_ptr<LevelData> m_editedLevelData;
	sf::RectangleShape m_storeBackground;
	sf::RectangleShape m_playAreaBackground;
	sf::RectangleShape m_safeLine;

	std::vector<sf::Sprite> m_aliens;
	std::vector<sf::Sprite> m_walls;

	float m_debugStringLifeTime = 0;
	float m_maxlifeTime = 3000.f;
	bool m_objectChosen = false;
	sf::Sprite* m_cursorSprite;
	std::string m_chosenObjectType;
	sf::Vector2f m_mousePosition;
	std::vector<std::string> m_alienTypes;
	std::vector<std::string> m_wallTypes;
};

