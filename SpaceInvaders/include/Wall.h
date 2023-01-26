#pragma once
#include "Actor.h"

class Wall : public Actor
{
public:
	Wall(const std::string& type, const sf::Vector2f& pos,float scaleMode = 1.0f);

	void Draw(sf::RenderWindow& renderTerget) const override;
	void Update(float dt) override;
	bool CollideWith(const sf::Rect<float>& collider) override;
	const std::string& GetStrType() const;

	const sf::Rect<float> GetGlobalBoundingBox() const;
private:
	int m_healthLeft;
	int m_healthRight;
	std::string m_type; // {whole, left, right}
	std::vector<sf::Sprite> m_sprites;
};