#pragma once
#include "Actor.h"
class HealthBar :
    public Actor
{
public:
    HealthBar(int health = 5);

    void Draw(sf::RenderWindow& renderTarget) const override;
    void Update(float dt) override;
    bool CollideWith(const sf::Rect<float>& collider) override;

    void SetHealth(int health);

    void SetPosition(const sf::Vector2f& pos);

private:
    std::vector<sf::Sprite> m_sprites;
    int m_health;
};

