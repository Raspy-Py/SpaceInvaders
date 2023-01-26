#include "LevelData.h"
#include "Utils.h"

#

void LevelData::WriteToFile(const std::string& fileName, const LevelData& LevelData)
{
	std::ofstream file(fileName);

	file << "[walls]" << std::endl;

	auto vm = sf::VideoMode::getDesktopMode();
	sf::Vector2f pgSize = { (float)vm.height * 0.8f, (float)vm.height};

	for (auto& wall : LevelData.m_walls)
	{
		file << (wall.GetPosition().x - ((float)vm.width-pgSize.x)/2.0f)  / pgSize.x << " " << wall.GetPosition().y / pgSize.y << ";";
		file << wall.GetStrType() << std::endl;
	}

	file << "[aliens]" << std::endl;

	for (auto& alien : LevelData.m_aliens)
	{
		file << (alien.GetPosition().x - ((float)vm.width - pgSize.x) / 2.0f) / pgSize.x << " " << alien.GetPosition().y / pgSize.y << ";";
		file << alien.GetStrType() << std::endl;
	}
	
	file.close();
}

std::unique_ptr<LevelData> LevelData::LoadFromFile(const std::string& fileName, bool menuMode)
{
	std::ifstream file(fileName);

	if (!file.is_open())
	{
		std::string msg = "ERROR: failed to open level file [" + fileName + "].";
		throw std::exception(msg.c_str());
	}

	auto vm = sf::VideoMode::getDesktopMode();
	sf::Vector2f pgSize = { (float)vm.height * 0.8f, (float)vm.height};

	if (menuMode)
	{
		pgSize.x /= 2.0f;
		pgSize.y /= 2.0f;
	}

	LevelData* data = new LevelData;
	std::string buf = "";
	data->m_name = GetNameFromPath(fileName);
	bool isWallParsing = true;

	while (std::getline(file, buf))
	{
		if (buf == "[walls]")
		{
			isWallParsing = true;
		}
		else if (buf == "[aliens]")
		{
			isWallParsing = false;
		}
		else
		{
			size_t delim = buf.find(';');
			float scale = 1.0;
			auto parsedVector = ParseVector(buf.substr(0, delim));
			parsedVector.x *= pgSize.x;
			parsedVector.x += ((float)vm.width - pgSize.x) / 2.0f;
			parsedVector.y *= pgSize.y;
			if (menuMode)
			{
				parsedVector.y += (float)vm.height / 4.0f;
				scale /= 2.0f;
			}
			if (isWallParsing)
			{
				data->m_walls.emplace_back(buf.substr(delim + 1, buf.size() - delim - 1), parsedVector,scale);
			}
			else
			{
				
				data->m_aliens.emplace_back(buf.substr(delim + 1, buf.size() - delim - 1), parsedVector, scale);
			}
		}
	}

	file.close();

	std::unique_ptr<LevelData> pData;
	pData.reset(data);

	return pData;
}

const std::vector<Wall>& LevelData::GetWalls() const
{
	return m_walls;
}

const std::vector<Alien>& LevelData::GetAliens() const
{
	return m_aliens;
}

void LevelData::AddAlien(const sf::Vector2f& pos, const std::string& type)
{
	m_aliens.emplace_back(type, pos);

	if (m_aliens.size() == 1)
		return;

	Alien& lastAdded = m_aliens[m_aliens.size() - 1];
	
	for (size_t i = 0; i < m_aliens.size() - 1; i++)
	{
		auto& alien = m_aliens[i];
		if (alien.GetGlobalBoundingBox().intersects(lastAdded.GetGlobalBoundingBox()))
		{
			m_aliens.pop_back();
			break;
		}
	}
}

void LevelData::AddWall(const sf::Vector2f& pos, const std::string& type)
{
	m_walls.emplace_back(type, pos);

	if (m_walls.size() == 1)
		return;

	Wall& lastAdded = m_walls[m_walls.size() - 1];

	for (size_t i = 0; i < m_walls.size()-1; i++)
	{
		auto& wall = m_walls[i];
		if (wall.GetGlobalBoundingBox().intersects(lastAdded.GetGlobalBoundingBox()))
		{
			m_walls.pop_back();
			break;
		}
	}
}

void LevelData::Clear()
{
	m_walls.clear();
	m_aliens.clear();
}
