#include "../include/Scene_Menu.h"
#include "../include/Scene_Play.h"
#include "../include/Common.h"
#include "../include/Assets.h"
#include "../include/GameEngine.h"
#include "../include/Components.h"
#include "../include/Action.h"

Scene_Menu::Scene_Menu(GameEngine *game_engine)
	: Scene(game_engine)
{
	initialize();
}

void Scene_Menu::initialize()
{
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::D, "PLAY");
	registerAction(sf::Keyboard::Escape, "QUIT");

	m_title= "Mega Plumber Man";
	m_menu_strings.push_back("Level 1");
	m_menu_strings.push_back("Level 2");
	m_menu_strings.push_back("Level 3");

	m_level_paths.push_back("levels/level1.txt");
	m_level_paths.push_back("levels/level2.txt");
	m_level_paths.push_back("llevels/level3.txt");

	m_menu_text.setFont(m_game->getAssets().get_font("Mario"));
	m_menu_text.setCharacterSize(60);
	m_menu_text.setFillColor(sf::Color(0, 0, 0));
}

void Scene_Menu::onEnd()
{
	m_game->quit();
}

void Scene_Menu::update()
{
	m_currentFrame++;
	sRender();
}

void Scene_Menu::sDoAction(const Action &action)
{
	if (action.type() == "START")
	{
		if (action.name() == "UP")
		{
			if (m_menu_index > 0) { m_menu_index--; }
			else { m_menu_index= m_menu_strings.size() - 1; }
		}
		else if (action.name() == "DOWN")
		{
			m_menu_index= (m_menu_index + 1) % m_menu_strings.size();
		}
		else if (action.name() == "PLAY")
		{
			m_game->changeScene("PLAY", std::make_shared<Scene_Play>(m_game, m_level_paths[m_menu_index]));
		}
		else if (action.name() == "QUIT")
		{
			onEnd();
		}
	}
}

void Scene_Menu::sRender()
{
	m_game->window().clear(sf::Color(51, 51, 255));

	m_menu_text.setString(m_title);
	m_menu_text.setCharacterSize(64);
	m_menu_text.setPosition(5, 8);
	m_game->window().draw(m_menu_text);

	m_menu_text.setPosition(5, 40);
	for (int i= 0; i < m_menu_strings.size(); i++)
	{
		m_menu_text.setString(m_menu_strings[i]);
		m_menu_text.setPosition(5, m_menu_text.getPosition().y + 80);

		if (i == m_menu_index)
		{
			m_menu_text.setFillColor(sf::Color(255, 255, 255));
		}
		else
		{
			m_menu_text.setFillColor(sf::Color(0, 0, 0));
		}
		m_game->window().draw(m_menu_text);
	}

	m_menu_text.setString("UP:W  DOWN:S  PLAY:D  BACK:ESC");
	m_menu_text.setFillColor(sf::Color(0, 0, 0));
	m_menu_text.setCharacterSize(24);
	m_menu_text.setPosition(5, (int)height() - 64);
	m_game->window().draw(m_menu_text);
}