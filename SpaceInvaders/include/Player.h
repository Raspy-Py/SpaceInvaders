#pragma once
#include "Actor.h"
class Player : public Actor
{
public:
	Player();

	void Draw(sf::RenderWindow& renderTarget) const override;
	void Update(float dt) override;
	bool CollideWith(const sf::Rect<float>& collider) override;

	void SetHealth(int health);
	void SetPosition(const sf::Vector2f& pos);
	int GetHealth() const;
	const sf::Rect<float> GetGlobalBoundingBox() const;

	void MoveLeft(float dt);
	void MoveRight(float dt);

private:
	float m_speed = 0.5;
	int m_health = 5;
	std::vector<sf::Sprite> m_sprites;
};

