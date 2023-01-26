#pragma once
#include "Actor.h"

class Bullet : public Actor
{
public:
	Bullet(const sf::Vector2f& pos, float speed, bool isPlayerBullet);

	void Draw(sf::RenderWindow& renderTarget) const override;
	void Update(float dt) override;
	bool CollideWith(const sf::Rect<float>& collider) override;

	const sf::Rect<float> GetGlobalBoundingBox() const;
	bool IsPlayerBullet();
	void SetPosition(const sf::Vector2f& pos);

private:
	bool m_isPlayerBullet;
	float m_speed;
	float m_direction;
	float m_timeElapsed = 0;
	float m_animationTick = 200;
	size_t m_currentSprite = 0;
	std::vector<sf::Sprite> m_sprites;
};

