#include "TextInput.h"
#include "ResourceManager.h"

TextInput::TextInput(const std::string& fontName,
	sf::Vector2f size, sf::Vector2f pos,
	sf::Color color, sf::Color colorHovered,
	sf::Color textColor)
	:
	m_color(color), m_colorHovered(colorHovered),
	m_string(""), m_active(false)
{
	m_background.setFillColor(color);
	m_background.setSize(size);
	m_background.setPosition(pos);

	auto& font = ResourceManager::GetInstance()->GetFont(fontName);
	uint16_t charSize = 0.6 * size.y;

	m_text.setFont(font);
	m_text.setFillColor(textColor);
	m_text.setCharacterSize(charSize);
	m_text.setString(m_string);

	float offset = (size.y - (float)charSize) / 2.0f;
	sf::Vector2f textPos = pos + sf::Vector2f(offset, offset);
	m_text.setPosition(textPos);
}

void TextInput::Draw(sf::RenderWindow& renderTarget)
{
	renderTarget.draw(m_background);
	renderTarget.draw(m_text);
}

void TextInput::Update(float dt)
{
	m_timeElapsed += dt;
	// handling input
	sf::Vector2f mpf{(float)sf::Mouse::getPosition().x, (float)sf::Mouse::getPosition().y};
	if (m_active)
	{
		bool isShiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
			!m_background.getGlobalBounds().contains(mpf))
		{
			m_active = false;
			m_background.setFillColor(m_color);
		}
		else
		{	
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
			{
				PopChar();
			}
			else if (isShiftPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Hyphen))
			{
				AddChar('_');
			}
			else
			{
				uint16_t upperCaseBias = (isShiftPressed ? ('A' - 'a') : 0);
				for (uint16_t i = 0; i <= 35; i++)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(i)))
					{
						if (i < 26) AddChar('a' + i + upperCaseBias);
						else AddChar('0' + i - 26);
						break;
					}
				}
			}
		}
	}
	else
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
			m_background.getGlobalBounds().contains(mpf))
		{
			m_background.setFillColor(m_colorHovered);
			m_active = true;
		}
	}

	// updating command
	m_command = Command(m_string);
}

void TextInput::AddChar(char c)
{
	if (m_string.size() && c == m_string.back() && m_timeElapsed <= m_autorepeateCooldown)
		return;

	m_timeElapsed = 0;
	m_string.push_back(c);
	m_text.setString(m_string);

	auto tbb = m_text.getGlobalBounds();
	auto gbb = m_background.getGlobalBounds();

	//m_background.setPosition(tbb.left, tbb.top);
	//m_background.setSize({ tbb.width, tbb.height });

	if (tbb.left + tbb.width > gbb.left + gbb.width)
	{
		m_timeElapsed += 2 * m_autorepeateCooldown;
		PopChar();
	}
}

void TextInput::PopChar()
{

	if (m_string.empty() || m_timeElapsed * 1.6f <= m_autorepeateCooldown)
		return;

	m_timeElapsed = 0;
	m_string.pop_back();
	m_text.setString(m_string);
}
