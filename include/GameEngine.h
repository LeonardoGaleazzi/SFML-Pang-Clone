#pragma once

#include "Assets.h"
#include "Common.h"
#include "Scene.h"
#include <memory>

class Scene;

typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine {
protected:
  sf::RenderWindow m_window;
  Assets m_assets;
  std::string m_currentScene;
  SceneMap m_sceneMap;
  size_t m_simulationSpeed = 1;
  bool m_running = true;
  size_t m_currentFrames = 0;

  void init(const std::string &path);
  void update();

  void sUserInput();

  std::shared_ptr<Scene> currentScene();

public:
  GameEngine(const std::string &path);

  void changeScene(const std::string&  sceneName,
                   const std::shared_ptr<Scene> &scene,
                   bool endCurrentScene = false);

  void toMenu();

  void quit();
  void run();

  sf::RenderWindow &window();
  const Assets &getAssets() const;
  const sf::RenderWindow& getWindow() const;
  bool isRunning();
};
