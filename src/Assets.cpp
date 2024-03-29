#include "../include/Assets.h"
#include <cassert>

Assets::Assets()
{

}

void Assets::load_from_file(const std::string &path)
{
	std::ifstream file(path);
	std::string string;
	while (file >> string)
	{
		//std::cout << string << std::endl;
		if (string == "Texture")
		{
			std::string name, path;
			file >> name >> path;
			add_texture(name, path);
		}
		else if (string == "Animation")
		{
			std::string name, texture;
			size_t frames, speed;
			file >> name >> texture >> frames >> speed;
			add_animation(name, texture, frames, speed);
		}
		else if (string == "Font")
		{
			std::string name, path;
			file >> name >> path;
			add_font(name, path);
		}
		else
		{
			std::cerr << "Unknow Asset Type: " << string << std::endl;
		}
	}
}

void Assets::add_texture(const std::string &texture_name, const std::string &path, bool smooth)
{
	m_texture_map[texture_name] = sf::Texture();

	if (!m_texture_map[texture_name].loadFromFile(path))
	{
		std::cerr << "Could not load texture file: " << path << std::endl;
	}
	else
	{
		m_texture_map[texture_name].setSmooth(smooth);
		//std::cout << "Loaded Texture: " << path << std::endl;
	}
}

const sf::Texture &Assets::get_texture(const std::string &texture_name) const
{
	assert(m_texture_map.find(texture_name) != m_texture_map.end());
	return m_texture_map.at(texture_name);
}

void Assets::add_animation(const std::string &animation_name, const std::string &texture_name, size_t frame_count, size_t speed)
{
	m_animation_map[animation_name]= Animation(animation_name, get_texture(texture_name), frame_count, speed);
	//std::cout << "Loaded Animation: " << animation_name << std::endl;
}

const Animation &Assets::get_animation(const std::string &animation_name) const
{
	assert(m_animation_map.find(animation_name) != m_animation_map.end());
	return m_animation_map.at(animation_name);
}

void Assets::add_font(const std::string &font_name, const std::string &path)
{
	sf::Font font;
	if (!font.loadFromFile(path))
	{
		std::cerr << "Could not load font: " + path + "\n";
		exit(-1);
	}

	m_font_map[font_name]= font;

	//std::cout << "Loaded Font: " << path << std::endl;
}

const sf::Font &Assets::get_font(const std::string &font_name) const
{
	assert(m_font_map.find(font_name) != m_font_map.end());
	return m_font_map.at(font_name);
}