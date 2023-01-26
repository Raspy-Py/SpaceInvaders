#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>

#include "Alien.h"
#include "Wall.h"

class LevelData
{
public:
	LevelData() = default;

	LevelData(LevelData&& o) noexcept
	{
		m_aliens = std::move(o.m_aliens);
		m_walls = std::move(o.m_walls);
	}
	
	static void WriteToFile(const std::string& fileName, const LevelData& LevelData);
	static std::unique_ptr<LevelData> LoadFromFile(const std::string& fileName, bool menuMode = false);

	const std::vector<Wall>& GetWalls() const;
	const std::vector<Alien>& GetAliens() const;

	void AddAlien(const sf::Vector2f& pos, const std::string& type);
	void AddWall(const sf::Vector2f& pos, const std::string& type);

	void Clear();
private:
	std::string m_name;
	std::vector<Wall> m_walls;
	std::vector<Alien> m_aliens;
};

