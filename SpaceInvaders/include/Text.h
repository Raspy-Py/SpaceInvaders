#pragma once
#include "Widget.h"
#include "ResourceManager.h"

class Text : public Widget
{
public:
	Text(const std::string& text, const std::string& font, uint8_t size, sf::Vector2f pos, sf::Color color = sf::Color(255,255,255));

	void Draw(sf::RenderWindow& renderTarget) override;
	void Update(float dt) override;

	void SetString(const std::string& str);

private:
	sf::Text m_text;
};

