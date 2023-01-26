#pragma once
#include "Actor.h"
class Alien : public Actor
{
public:
	enum class Type
	{
		Android,
		Jellyfish,
		Skull,
		Mask,
		Funny,
		UFO
	};
public:
	Alien(const std::string& strType, const sf::Vector2f& pos, float scaleMode = 1.0f);

	void Draw(sf::RenderWindow& renderTarget) const override;
	void Update(float dt) override;
	void ChangeDirection(float adj);
	void MoveDown();
	void SetPosition(const sf::Vector2f& pos);
	void SetSpeedScale(float speedScale);
	bool CollideWith(const sf::Rect<float>& collider) override;

	const sf::Rect<float> GetGlobalBoundingBox() const;
	const std::string& GetStrType() const
	{
		return m_strType;
	}
private:
	float m_speedScale = 1.0f;
	float m_speed;
	Type m_type;
	std::string m_strType;
	sf::Vector2f m_speedVector;
	std::vector<sf::Sprite> m_sprites;
	float m_timeElapsed = 0;
	float m_animationTick = 800;
	size_t m_currentSprite = 0;
};

