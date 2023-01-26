#include "Button.h"
#include "ResourceManager.h"

Button::Button()
{
}

Button::Button(const std::string& text, const std::string& fontName,
	sf::Vector2f size, sf::Vector2f pos,
	sf::Color color, sf::Color colorHovered,
	sf::Color textColor, sf::Color textColorHovered)
	:
	m_color(color), m_colorHovered(colorHovered),
	m_textColor(textColor), m_textColorHovered(textColorHovered)
{
	m_background.setFillColor(color);
	m_background.setSize(size);
	m_background.setPosition(pos);

	auto& font = ResourceManager::GetInstance()->GetFont(fontName);
	uint16_t charSize = 0.6 * size.y;

	m_laybel.setFont(font);
	m_laybel.setFillColor(textColor);
	m_laybel.setCharacterSize(charSize);
	m_laybel.setString(text);

	sf::Vector2f tr = { m_laybel.getLocalBounds().width, (float)charSize };
	sf::Vector2f textPos = pos + (size - tr) / 2.0f;
	m_laybel.setPosition(textPos);
}

void Button::Draw(sf::RenderWindow& renderTarget)
{
	renderTarget.draw(m_background);
	renderTarget.draw(m_laybel);
}

void Button::Update(float dt)
{
	sf::Vector2i mp = sf::Mouse::getPosition();

	auto gb = m_background.getGlobalBounds();

	if (mp.x >= gb.left && mp.x <= gb.left + gb.width &&
		mp.y >= gb.top && mp.y <= gb.top + gb.height)
	{
		m_laybel.setFillColor(m_textColorHovered);
		m_background.setFillColor(m_colorHovered);

		// TODO: impl command on press

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			m_command.SetType(Command::Type::Pressed);
		}
		else
		{
			m_command.SetType(Command::Type::None);
		}
	}
	else
	{
		m_laybel.setFillColor(m_textColor);
		m_background.setFillColor(m_color);

		m_command.SetType(Command::Type::None);
	}
}

void Button::SetPosition(float x, float y)
{
}

void Button::SetSize(float x, float y)
{
}
