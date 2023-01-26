#pragma once
#include "Widget.h"

class Button : public Widget
{
public:
	Button();
	Button(const std::string& text, const std::string& fontName,
		sf::Vector2f size, sf::Vector2f pos,
		sf::Color color = sf::Color(0,0,0,0), 
		sf::Color colorHovered = sf::Color(0,0,0,0),
		sf::Color textColor = sf::Color(255,255,255,255),
		sf::Color textColorHovered = sf::Color(255,255,55,255));

	void Draw(sf::RenderWindow& renderTarget) override;
	void Update(float dt) override;

	void SetPosition(float x, float y);
	void SetSize(float x, float y);
private:
	sf::RectangleShape m_background;
	sf::Text m_laybel;

	sf::Color m_color;
	sf::Color m_colorHovered;
	sf::Color m_textColor;
	sf::Color m_textColorHovered;
};

