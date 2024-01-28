#pragma once

class AssetManager
{
public:

	static AssetManager& getInstance() 
	{ 
		static AssetManager instance;
		return instance; 
	}

	static std::map<std::string, std::shared_ptr<sf::Texture>> Textures;
	static std::map<std::string, std::shared_ptr<sf::Sound>> Sounds;
	static std::map<std::string, std::shared_ptr<sf::Music>> Music;
	static std::map<std::string, std::shared_ptr<sf::Font>> Fonts;

	static void loadTexture(const std::string& name, const std::string& fileName);
	static void loadSound(const std::string& name, const std::string& fileName);
	static void loadMusic(const std::string& name, const std::string& fileName);
	static void loadFont(const std::string& name, const std::string& fileName);

	static void unloadAssets();

private:

	AssetManager() = default;
	~AssetManager() = default;
	AssetManager(const AssetManager&) = delete;
	AssetManager& operator=(const AssetManager&) = delete;
};