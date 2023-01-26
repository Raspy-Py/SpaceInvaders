#include "ResourceManager.h"
#include "LevelData.h"

#include <exception>
#include <fstream>

std::unique_ptr<ResourceManager> ResourceManager::msp_instance;

const sf::Font& ResourceManager::GetFont(const std::string& name)
{
	auto pFont = m_fontMap.find(name);

	if (pFont != m_fontMap.end())
	{
		return pFont->second;
	}
	else
	{
		std::string msg = "ERROR: font [" + name + "] is not loaded.";
		throw std::exception(msg.c_str());
	}
}

const LevelData& ResourceManager::GetLevelData(const std::string& name)
{
	auto pLevel = m_levelDataMap.find(name);

	if (pLevel != m_levelDataMap.end())
	{
		return *pLevel->second.get();
	}
	else
	{
		std::string msg = "ERROR: level [" + name + "] is not loaded.";
		throw std::exception(msg.c_str());
	}
}

const sf::Texture& ResourceManager::GetTexture(const std::string& name)
{
	auto pTexture = m_textureMap.find(name);

	if (pTexture != m_textureMap.end())
	{
		return *pTexture->second.get();
	}
	else
	{
		std::string msg = "ERROR: texture [" + name + "] is not loaded.";
		throw std::exception(msg.c_str());
	}
}

void ResourceManager::LoadFont(const std::string& file, const std::string& fontName)
{
	sf::Font font;
	if (!font.loadFromFile(file))
	{
		std::string msg = "ERROR: failed to load font [" + fontName + "] from file [" + file + "].";
		throw std::exception(msg.c_str());
	}

	m_fontMap.emplace(fontName, std::move(font));
}

void ResourceManager::LoadLevelData(const std::string& file, const std::string& levelName)
{
	auto pLevel = m_levelDataMap.find(levelName);

	if (pLevel != m_levelDataMap.end())
		return;
	
	m_levelDataMap.emplace(levelName, std::move(LevelData::LoadFromFile(file)));
}

void ResourceManager::LoadTexture(const std::string& file, const std::string& textureName)
{
	auto pTexture = std::make_unique<sf::Texture>();

	if (pTexture->loadFromFile(file))
	{
		m_textureMap.emplace(textureName, std::move(pTexture));
	}
	else
	{
		std::string msg = "ERROR: failed to load texture [" + textureName + "] from file [" + file + "].";
		throw std::exception(msg.c_str());
	}
}

void ResourceManager::LoadTexture(const std::string& file, const std::string& textureName, uint16_t left, uint16_t top, uint16_t width, uint16_t height)
{
	auto pTexture = std::make_unique<sf::Texture>();

	if (pTexture->loadFromFile(file, sf::IntRect(left, top, width, height)))
	{
		m_textureMap.emplace(textureName, std::move(pTexture));
	}
	else
	{
		std::string msg = "ERROR: failed to load texture tile [" + textureName + "] from file [" + file + "].";
		throw std::exception(msg.c_str());
	}
}

void ResourceManager::UnloadLevelData(const std::string& name)
{
	auto pLevel = m_levelDataMap.find(name);

	if (pLevel != m_levelDataMap.end())
	{
		m_levelDataMap.erase(name);
	}
}

void ResourceManager::SaveLevelData(const std::string& file, const LevelData& level)
{
	LevelData::WriteToFile(file, level);
}

ResourceManager* ResourceManager::GetInstance()
{
	if (msp_instance.get() == nullptr)
	{
		msp_instance.reset(new ResourceManager);
	}
	
	return msp_instance.get();
}
