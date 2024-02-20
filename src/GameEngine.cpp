#include "../include/GameEngine.h"
#include "../include/Assets.h"
#include "../include/Scene_Play.h"
#include "../include/Scene_Menu.h"

GameEngine::GameEngine(const std::string &path)
{
	init(path);
}

void GameEngine::init(const std::string &path)
{
	m_assets.load_from_file(path);

	m_window.create(sf::VideoMode(1280, 768), "Definitely Not Mario");
	m_window.setFramerateLimit(60);

	changeScene("MENU", std::make_shared<Scene_Menu>(this));
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
	return m_sceneMap[m_currentScene];
}

bool GameEngine::isRunning()
{
	return m_running && m_window.isOpen();
}

const Assets &GameEngine::getAssets() const
{
	return m_assets;
}

sf::RenderWindow &GameEngine::window()
{
	return m_window;
}

void GameEngine::run()
{
	while (isRunning())
	{
		update();
	}
}

void GameEngine::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			quit();
		}

		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
		{
			// if the current scene does not have an action associated with this key, skip the event
			if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end()) { continue; }

			// determine the start or end action by whether it was key press or release
			const std::string action_type= (event.type == sf::Event::KeyPressed) ? "START" : "END";

			// look up the action and send the action to the scene
			currentScene()->sDoAction(Action(currentScene()->getActionMap().at(event.key.code), action_type));
		}
	}
}

void GameEngine::changeScene(const std::string &scene_name, const std::shared_ptr<Scene>& scene, bool end_current_scene)
{
	// If a scene was passed, add it to the map with the scene name
	if (scene)
	{
		m_sceneMap[scene_name]= scene;
	}
	else
	{
		// If no scene was passed and the scene name is not in the map then return a warning and exit
		if (m_sceneMap.find(scene_name) == m_sceneMap.end())
		{
			std::cerr << "Could not find scene!";
			exit(-1);
		}
	}

	if (end_current_scene)
	{
		m_sceneMap.erase(m_sceneMap.find(m_currentScene));
	}

	m_currentScene= scene_name;
}

void GameEngine::update()
{
	if (!isRunning()) { return; }

	if (m_sceneMap.empty()) { return; }

	sUserInput();
	currentScene()->update();	
	window().display();
}

void GameEngine::quit()
{
	m_running= false; 
}

const sf::RenderWindow& GameEngine::getWindow() const
{
	return m_window;
}

void GameEngine::toMenu()
{
	changeScene("MENU", std::make_shared<Scene_Menu>(this), true);
}
