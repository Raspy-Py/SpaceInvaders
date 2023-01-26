#pragma once
#include "Widget.h"
class TextInput : public Widget
{
public:
	TextInput() = default;
	TextInput(const std::string& font,
		sf::Vector2f size, sf::Vector2f pos,
		sf::Color color = sf::Color(0, 0, 0, 0),
		sf::Color colorHovered = sf::Color(0, 0, 0, 0),
		sf::Color textColor = sf::Color(255, 255, 255, 255));

	void Draw(sf::RenderWindow& renderTarget);
	void Update(float dt);

private:

	void AddChar(char c);
	void PopChar();

	bool m_active;
	float m_autorepeateCooldown = 150;
	float m_timeElapsed = 0;
	std::string m_string;
	sf::Text m_text;
	sf::RectangleShape m_background;
	sf::Color m_color;
	sf::Color m_colorHovered;
};

