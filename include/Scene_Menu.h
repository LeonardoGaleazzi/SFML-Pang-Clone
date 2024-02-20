#pragma once

#include "../include/Common.h"
#include "../include/Scene.h"
#include "../include/GameEngine.h"
#include <vector> // ?
#include <memory> // ?

typedef std::vector<std::string> StringsVec;
class Scene_Menu : public Scene
{
protected:
	std::string m_title;
	StringsVec	m_menu_strings;
	StringsVec	m_level_paths;
	int			m_menu_index= 0;
	sf::Text	m_menu_text;

	void initialize();

	void update();

	void onEnd();

	void sDoAction(const Action &action);
	
public:
	
	Scene_Menu(GameEngine *game_engine);
	void sRender();
};