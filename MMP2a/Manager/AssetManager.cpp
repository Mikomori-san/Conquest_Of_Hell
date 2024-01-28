#include "stdafx.h"
#include "AssetManager.h"

std::map<std::string, std::shared_ptr<sf::Texture>> AssetManager::Textures;
std::map<std::string, std::shared_ptr<sf::Sound>> AssetManager::Sounds;
std::map<std::string, std::shared_ptr<sf::Music>> AssetManager::Music;
std::map<std::string, std::shared_ptr<sf::Font>> AssetManager::Fonts;

void AssetManager::loadTexture(const std::string& name, const std::string& fileName)
{
	std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
	
	if (!texture->loadFromFile(fileName))
	{
		std::cout << "The file has not been found..." << std::endl;
		return;
	}

	Textures[name] = texture;
}

void AssetManager::loadSound(const std::string& name, const std::string& fileName)
{
	sf::SoundBuffer buffer;

	if (!buffer.loadFromFile(fileName))
	{
		std::cout << "The file has not been found..." << std::endl;
		return;
	}

	std::shared_ptr<sf::Sound> sound = std::make_shared<sf::Sound>();
	sound->setBuffer(buffer);

	Sounds[name] = sound;
}

void AssetManager::loadMusic(const std::string& name, const std::string& fileName)
{
	std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();

	if (!music->openFromFile(fileName))
	{
		std::cout << "The file has not been found..." << std::endl;
	}
	
	Music[name] = music;

}

void AssetManager::loadFont(const std::string& name, const std::string& fileName)
{
	std::shared_ptr<sf::Font> font = std::make_shared<sf::Font>();

	if (!font->loadFromFile(fileName))
	{
		std::cout << "The file has not been found..." << std::endl;
		return;
	}

	Fonts[name] = font;
}

void AssetManager::unloadAssets()
{
	for (auto& texturePair : Textures)
	{
		texturePair.second.reset();
	}
	Textures.clear();

	for (auto& soundPair : Sounds)
	{
		soundPair.second.reset();
	}
	Sounds.clear();

	for (auto& musicPair : Music)
	{
		musicPair.second->stop();
		musicPair.second.reset();
	}
	Music.clear();

	for (auto& fontPair : Fonts)
	{
		fontPair.second.reset();
	}
	Fonts.clear();
}