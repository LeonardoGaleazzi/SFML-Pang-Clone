#pragma once
#include "Common.h"
#include "Physics.h"
#include "Scene.h"


class Scene_Play : public Scene {
  struct PlayerConfig {
    float X, Y, CX, CY, SPEED, MAXSPEED, JUMP, GRAVITY;
    std::string WEAPON;
  };

  protected:
  std::shared_ptr<Entity> m_player;
  std::string m_levelPath;
  PlayerConfig m_playerConfig;
  bool m_drawTextures = true;
  bool m_drawCollision = false;
  bool m_drawGrid = false;
  const Vec2 m_gridSize = {16, 16};
  sf::Text m_gridText;
  Physics m_physics;

  void init(const std::string &);
  void loadLevel(const std::string &);
  void spawnPlayer();
  void onEnd();

  void spawnBullet(const std::shared_ptr<Entity>);
  void spawnEnemy();

  Vec2 gridToMidPixel(float , float , std::shared_ptr<Entity> );

  void sMovement();
  void sLifespan();
  void sCollision();
  void sDoAction(const Action &);
  void sAnimation();
  void sRender();
  void sCreate();
  void sClean();
  
  bool checkBottom();

  template<typename T1, typename T2>
  void print(const std::string& , const T1& , const T2&) ;

  void print(std::string);
  void spawnCoin(std::shared_ptr<Entity>);
  void spawnChain(std::shared_ptr<Entity>);
  void destroyChainBullet();
  void splitBall(std::shared_ptr<Entity>);
  public:
  Scene_Play(GameEngine *, const std::string &);
  void update();
};
