#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <map>

#include "LevelData.h"

class ResourceManager
{
public:
	void LoadFont(const std::string& file, const std::string& fontName = "DefaultFont");
	void LoadLevelData(const std::string& file, const std::string& levelName = "DefaultLevel");
	void LoadTexture(const std::string& file, const std::string& textureName);
	void LoadTexture(const std::string& file, const std::string& textureName,
		uint16_t left, uint16_t top, uint16_t width, uint16_t height);

	void UnloadLevelData(const std::string& name);
	void SaveLevelData(const std::string& file, const LevelData& level);

	const sf::Font& GetFont(const std::string& name = "DefaultFont");
	const LevelData& GetLevelData(const std::string& name = "DefaultLevel");
	const sf::Texture& GetTexture(const std::string& name);

	static ResourceManager* GetInstance();

private:
	ResourceManager() = default;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager operator=(const ResourceManager&) = delete;

private:
	std::map<std::string, sf::Font> m_fontMap;
	std::map<std::string, std::unique_ptr<LevelData>> m_levelDataMap;
	std::map<std::string, std::unique_ptr<sf::Texture>> m_textureMap;

	static std::unique_ptr<ResourceManager> msp_instance;
};

