#pragma once
#include <SFML/Graphics.hpp>

class Actor
{
public:
	Actor(float scaleModifier = 1.0f) { m_normScale *= scaleModifier; }

	virtual void Draw(sf::RenderWindow& renderTarget) const = 0;
	virtual void Update(float dt) = 0;

	virtual bool CollideWith(const sf::Rect<float>& collider) = 0;

	const std::string& GetName() const
	{
		return m_name;
	}
	const sf::Vector2f& GetPosition() const
	{
		return m_position;
	}
	const sf::Vector2f& GetSize() const
	{
		return m_size;
	}
	const sf::Rect<float> GetGlobalBoundingBox() const
	{
		return sf::Rect<float>(m_position, m_size);
	}

	void SetName(const std::string& name)
	{
		m_name = name;
	}
	void SetPosition(const sf::Vector2f& position)
	{
		m_position = position;
	}
	void SetSize(const sf::Vector2f& size)
	{
		m_size = size;
	}

	float GetScale() const
	{
		return (float)sf::VideoMode::getDesktopMode().height / 1080 * m_normScale;
	}

private:
	float m_normScale = 4.0f;
	float m_speed = 0;
	float m_direction = 0;
	sf::Vector2f m_position;
	sf::Vector2f m_size;
	std::string m_name = "BaseActor";
};

