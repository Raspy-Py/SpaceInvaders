#include "Text.h"


Text::Text(const std::string& text, const std::string& fontName, uint8_t size, sf::Vector2f pos, sf::Color color)
{
	const sf::Font& font = ResourceManager::GetInstance()->GetFont(fontName);

	m_text.setFont(font);
	m_text.setCharacterSize(size);
	m_text.setFillColor(color);
	m_text.setString(text);

	auto bb = m_text.getGlobalBounds();
	auto biasedPos = pos;
	biasedPos.x -= bb.width / 2.0f;
	m_text.setPosition(biasedPos);
}

void Text::Draw(sf::RenderWindow& renderTarget)
{
	renderTarget.draw(m_text);
}

void Text::Update(float dt)
{
	// nothing to update here yet
}

void Text::SetString(const std::string& str)
{
	sf::Vector2f pos = m_text.getPosition();
	pos.x += m_text.getGlobalBounds().width / 2.0f;
	m_text.setString(str);
	pos.x -= m_text.getGlobalBounds().width / 2.0f;
	m_text.setPosition(pos);
}
