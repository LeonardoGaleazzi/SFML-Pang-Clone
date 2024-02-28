#include "../include/Scene_Play.h"
#include "../include/Scene_Menu.h"
#include "../include/Action.h"
#include "../include/Assets.h"
#include "../include/Components.h"
#include "../include/GameEngine.h"
#include <fstream>

Scene_Play::Scene_Play(GameEngine *gameEngine, const std::string &levelPath)
    : Scene(gameEngine), m_levelPath(levelPath) {
  init(m_levelPath);
}

void Scene_Play::init(const std::string &levelPath) {
  registerAction(sf::Keyboard::Z, "PAUSE");
  registerAction(sf::Keyboard::Escape, "QUIT");
  registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE"); 
  registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");
  registerAction(sf::Keyboard::P, "TOGGLE_GRID");
  registerAction(sf::Keyboard::A, "LEFT");
  registerAction(sf::Keyboard::W, "JUMP");
  registerAction(sf::Keyboard::D, "RIGHT");
  registerAction(sf::Keyboard::R, "FIRE");

  m_gridText.setCharacterSize(12);
  m_gridText.setFont(m_game->getAssets().get_font("Arial"));

  loadLevel(levelPath);
}

Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY,
                                std::shared_ptr<Entity> entity) {


  size_t windowHeight = m_game->window().getSize().y;

  Vec2 blBlock = Vec2(m_gridSize.x *gridX, (windowHeight - m_gridSize.y * gridY));

  
    auto& eAnimation = entity->getComponent<CAnimation>();

    Vec2 animation_size = eAnimation.animation.getSize();
  
  animation_size.y *= -1;

  animation_size /= 2;



  Vec2 delta = blBlock + animation_size;




  

  return delta;
}



void Scene_Play::loadLevel(const std::string &filename) {
  m_entityManager = EntityManager();





    std::ifstream file(filename);
	  std::string entityTag;
	  while (file >> entityTag)
	    {
        

		    

		    if (entityTag == "Tile")
		    {

          std::string animationName;
          float x, y;
			    
			    file >> animationName >> x >> y;
			    auto tileEntity = m_entityManager.addEntity(entityTag);
          std::cout << "animationName: " << animationName << " " << "x: " << x << " " << "y: " << y << std::endl;
          std::cout << "log" << std::endl;
          tileEntity->addComponent<CAnimation>(m_game->getAssets().get_animation(animationName), true);

          auto & b_animation = tileEntity->getComponent<CAnimation>();
          Vec2 scale(m_gridSize.x / b_animation.animation.getSize().x, m_gridSize.y / b_animation.animation.getSize().y);

          std::cout << "Grid y: " << m_gridSize.y << std::endl;
          std::cout << "Animation y: " << b_animation.animation.getSize().y << std::endl;
          std::cout << "Scale y: " << scale.y << std::endl;

          b_animation.animation.setScale(scale.x, scale.y);

          tileEntity->addComponent<CTransform>(gridToMidPixel(x, y, tileEntity));

          auto & b_transform = tileEntity->getComponent<CTransform>();

          b_transform.scale = scale;

          tileEntity->addComponent<CBounding_box>(b_animation.animation.getSize());
          tileEntity->addComponent<CState>("normal", "right");


		    }else if (entityTag == "Dec")
		    {

          std::string animationName;
          float x, y;
			    
			    file >> animationName >> x >> y;
          std::cout << "animationName: " << animationName << " " << "x: " << x << " " << "y: " << y << std::endl;
			    auto decEntity = m_entityManager.addEntity(entityTag);
          decEntity->addComponent<CAnimation>(m_game->getAssets().get_animation(animationName), true);
          auto & b_animation = decEntity->getComponent<CAnimation>();
          Vec2 scale(m_gridSize.x / b_animation.animation.getSize().x, m_gridSize.y / b_animation.animation.getSize().y);

          b_animation.animation.setScale(scale.x, scale.y);

          decEntity->addComponent<CTransform>(gridToMidPixel(x, y, decEntity));
          decEntity->addComponent<CState>("normal", "right");

          auto & b_transform = decEntity->getComponent<CTransform>();

          b_transform.scale = scale;

		    }else if (entityTag == "Background")
		    {

          std::string animationName;
          float x, y;
			    
			    file >> animationName >> x >> y;
			    auto decEntity = m_entityManager.addEntity(entityTag);
          decEntity->addComponent<CAnimation>(m_game->getAssets().get_animation(animationName), true);

          auto & b_animation = decEntity->getComponent<CAnimation>();

          Vec2 scale(m_game->getWindow().getSize().x / b_animation.animation.getSize().x, m_game->getWindow().getSize().y / b_animation.animation.getSize().y);

          

          b_animation.animation.setScale(scale.x, scale.y);


          decEntity->addComponent<CTransform>(gridToMidPixel(x, y, decEntity));
          decEntity->addComponent<CState>("normal", "right");


          auto & b_transform = decEntity->getComponent<CTransform>();

          b_transform.scale = scale;



		    }else if (entityTag == "Ball")
        {
          std::string animationName;
          float x, y;
			    
			    file >> animationName >> x >> y;
			    auto tileEntity = m_entityManager.addEntity(entityTag);
          std::cout << "animationName: " << animationName << " " << "x: " << x << " " << "y: " << y << std::endl;
          std::cout << "log" << std::endl;
          tileEntity->addComponent<CAnimation>(m_game->getAssets().get_animation(animationName), true);

          auto & b_animation = tileEntity->getComponent<CAnimation>();
          print("Ballsize: ", std::to_string(b_animation.animation.getSize().x), std::to_string(b_animation.animation.getSize().y));
          Vec2 scale(3,3);

          b_animation.animation.setScale(scale.x, scale.y);

          tileEntity->addComponent<CTransform>(gridToMidPixel(x, y, tileEntity), Vec2(1,1), Vec2(1,1), 0);

          auto & b_transform = tileEntity->getComponent<CTransform>();

          b_transform.scale = scale;

          tileEntity->addComponent<CBounding_box>(b_animation.animation.getSize());
          tileEntity->addComponent<CState>("normal", "right");
          tileEntity->addComponent<CGravity>(0.03);
        }else if (entityTag == "Player")
		    {

          file >> m_playerConfig.X >> m_playerConfig.Y >> m_playerConfig.CX >> m_playerConfig.CY >> m_playerConfig.SPEED >> m_playerConfig.JUMP >> m_playerConfig.MAXSPEED >> m_playerConfig.GRAVITY >> m_playerConfig.WEAPON;


          spawnPlayer();

		    }
		else
		{
			std::cerr << "Unknow Asset Type: " << entityTag << std::endl;
		}
	}

}

void Scene_Play::spawnPlayer() {
  // here is a sample player entity which you can use to construct other
  // entities

  m_player = m_entityManager.addEntity("player");
  m_player->addComponent<CAnimation>(m_game->getAssets().get_animation("Stand"), true);
  m_player->addComponent<CTransform>(gridToMidPixel(m_playerConfig.X, m_playerConfig.Y, m_player), Vec2(m_playerConfig.SPEED, 1), Vec2(1,1), 0);
  auto & p_animation = m_player->getComponent<CAnimation>();

  Vec2 scale((float) 64 / p_animation.animation.getSize().x, (float) 64 / p_animation.animation.getSize().y);

  //p_animation.animation.setScale(scale.x, scale.y);


  m_player->addComponent<CBounding_box>(Vec2(32 * scale.x, 32 * scale.y));
  m_player->addComponent<CGravity>(m_playerConfig.GRAVITY);
  m_player->addComponent<CState>("standing", "right");

  auto & p_transform = m_player->getComponent<CTransform>();

  p_transform.scale = scale;

  m_player->addComponent<CInput>();
  std::cout << "playerSpawned" << std::endl;

  // TODO: be sure to add the remaining components to the player
}

void Scene_Play::spawnBullet(const std::shared_ptr<Entity> entity) {
                                                   
  // TODO: this should spawn a bullet at the given entity, going in the
  // direction the entity is facing
  auto& bullet = m_entityManager.getEntities("bullet");

  if(!bullet.empty())
  {
    std::cout << "Bullet already present" << std::endl;
    return;
  }

  auto e = m_entityManager.addEntity("bullet");
  auto& p_state = m_player->getComponent<CState>();
  auto& p_transform = entity->getComponent<CTransform>();
  auto& p_bounding_box = entity->getComponent<CBounding_box>();

  e->addComponent<CAnimation>(m_game->getAssets().get_animation("BulletHead"), true);
  e->addComponent<CTransform>(Vec2(p_transform.position.x, p_transform.position.y - p_bounding_box.half_size.y - m_gridSize.y), Vec2(0,0), Vec2(1,1), 0);
  e->addComponent<CBounding_box>(e->getComponent<CAnimation>().animation.getSize());
  e->addComponent<CFrameCounter>();
  
}

void Scene_Play::spawnEnemy() {
  auto ball = m_entityManager.addEntity("ball");
  ball->addComponent<CAnimation>(m_game->getAssets().get_animation("Ball"), true);
  ball->addComponent<CTransform>(gridToMidPixel(1,1,ball));
}

void Scene_Play::sMovement() {
  // TODO: implement player movement / jumping based on its CInput component
  // TODO: implement gravity's effect on the player
  // TODO: implement the maximum player speed in both X and Y directions
  // NOTE: setting an  entity's scale.x to -1/1 will make it face to the
  // left/right

  auto& p_transform = m_player->getComponent<CTransform>();

  auto& p_state = m_player->getComponent<CState>();

  Vec2 playerVelocity(0,p_transform.velocity.y);

  auto& p_input = m_player->getComponent<CInput>();



  if((p_input.up && p_input.right) || (p_input.up && p_input.left))
  {
    if(p_state.state == "running")
    {
      playerVelocity =  Vec2(p_transform.velocity.x,m_playerConfig.JUMP);
      p_state.state = "jumping";
    }
  }
  if(p_input.right)
  {
    if(p_state.state == "standing") p_state.state = "running";
    if(p_state.direction == "left") p_state.direction = "right";
    playerVelocity = Vec2(m_playerConfig.SPEED,playerVelocity.y);
  }else if(p_input.up && p_state.state == "standing")
  {
    playerVelocity =  Vec2(0,m_playerConfig.JUMP);
    p_input.up = false;
    p_state.state = "jumping";

  }else if(p_input.down)
  {
    playerVelocity =  Vec2(playerVelocity.x,1);
  }else if(p_input.left)
  {
    if(p_state.state == "standing") p_state.state = "running";
    if(p_state.direction == "right") p_state.direction = "left";
    playerVelocity =  Vec2(-m_playerConfig.SPEED,playerVelocity.y);
  }else
  {
    if(p_state.state == "running")
    {
      p_state.state = "standing";
    }
  }

  p_transform.velocity = playerVelocity;


  for(auto e : m_entityManager.getEntities())
  {
    auto& p_tranform = e->getComponent<CTransform>();
    if(e->hasComponent<CGravity>() && (p_tranform.velocity.y != 0 || (p_state.state != "standing" && p_state.state != "running")))
    {
      auto& p_gravity = e->getComponent<CGravity>();
      e->getComponent<CTransform>().velocity.y += p_gravity.gravity;
    }

    p_tranform.previous_position = e->getComponent<CTransform>().position;
    p_tranform.position += e->getComponent<CTransform>().velocity;


    if(p_tranform.velocity.y > m_playerConfig.MAXSPEED)
    {
      p_tranform.velocity.y = m_playerConfig.MAXSPEED;
    }


    /*
    if(e->hasComponent<CBounding_box>() && e->tag() != m_player->tag())
    {
      Vec2 overlap = m_physics.GetOverlap(m_player, e);
      if(overlap != Vec2(0,0)) std::cout << "(" << overlap.x << "," << overlap.y << ")" << std::endl;
    }*/

  }


  for(auto e: m_entityManager.getEntities("Ball"))
  {
    auto& b_tranfsform = e->getComponent<CTransform>();
    auto& b_gravity = e->getComponent<CGravity>();

    e->getComponent<CTransform>().velocity.y += b_gravity.gravity;
    b_tranfsform.position += e->getComponent<CTransform>().velocity;


  }

  


}

void Scene_Play::sLifespan() {
  // TODO: check lifespan of entities that have them, and destroy them if they
  // go over

  for(auto e : m_entityManager.getEntities())
  {
    if(e->hasComponent<CLifespan>())
    {
      auto& e_lifespan = e->getComponent<CLifespan>();
      e_lifespan.frame_created++;
      print(std::to_string(e_lifespan.frame_created));
      if(e_lifespan.frame_created > e_lifespan.lifespan)
      {
        e->destroy();
      }
    }

    if(e->hasComponent<CFrameCounter>())
    {
      e->getComponent<CFrameCounter>().frameCount++;
    }
  }
}

void Scene_Play::sCollision() {
   auto& print_transform = m_player->getComponent<CTransform>();
    
  auto& p_state = m_player->getComponent<CState>();

  bool is_there_a_bottom = checkBottom();

  for(auto e : m_entityManager.getEntities("Tile"))
  {

    Vec2 previous_collision = m_physics.GetPreviousOverlap(m_player, e);
    Vec2 current_collision = m_physics.GetOverlap(m_player, e);
    auto& player_transform = m_player->getComponent<CTransform>();
    auto& e_transform = e->getComponent<CTransform>();
    auto& e_state = e->getComponent<CState>();
    

    if(current_collision.x > 0 && current_collision.y > 0)
    {

    


      if(previous_collision.x > 0)
      {
        
        if(player_transform.position.y < e_transform.position.y)
        {
         
          player_transform.position.y -= current_collision.y;
          
          if(player_transform.velocity.y != 0) player_transform.velocity.y = 0;
          if(p_state.state != "standing") p_state.state = "standing";

        }else
        {
          e_state.state = "hit";
          player_transform.position.y += current_collision.y;
          player_transform.velocity.y = -0.25f;
        }
      }else if(previous_collision.y > 0)
      {
        if(player_transform.position.x < e_transform.position.x)
        {
          
          player_transform.position.x -= current_collision.x;
        }else
        {
          player_transform.position.x += current_collision.x;
        }
      }

    }


    for (auto bullet : m_entityManager.getEntities("bullet"))
    {
      Vec2 bullet_collision = m_physics.GetOverlap(bullet, e);
      


    if(bullet_collision.x > 0 && bullet_collision.y > 0 && e->tag() == "Tile")
    {
      if(e->getComponent<CAnimation>().animation.getName() == "Ground")
      {
        std::cout << "Brick Hit!" << std::endl;
        
        p_state.firing = false;
        destroyChainBullet();
      }else
      {
        //bullet->destroy();
      }
    }




    }


    if(e->getComponent<CAnimation>().animation.getName() != "Ground")
    {

      Vec2 previous_standing_collision = m_physics.GetPreviousOverlap(m_player, e);
      Vec2 current_standing_collision = m_physics.GetOverlap(m_player, e);

      if(previous_standing_collision.x > 0)
      {

      //std::cout << "Previous Collision: (" << previous_standing_collision.x << "," << previous_standing_collision.y << ")" << std::endl;

      //std::cout << "Current Collision: (" << current_standing_collision.x << "," << current_standing_collision.y << ")" << std::endl;

      }

      if(previous_standing_collision.x > 0 && current_standing_collision.x == 0 && (p_state.state == "standing" || p_state.state == "running") && !is_there_a_bottom)
      {
        p_state.state = "falling";
        std::cout << "Fall!" << std::endl;
      }
    }
    

    bool top_bottom = false;
    bool right_left = false;

    if(e->getComponent<CAnimation>().animation.getName() == "Ground")
    {
      int y_cell_number = (m_game->getWindow().getSize().y / m_gridSize.y) - 1;
      int x_cell_number = (m_game->getWindow().getSize().x / m_gridSize.x) - 1;

      //print("Cell count", 0, x_cell_number);

      int e_cell_y = e_transform.position.y / m_gridSize.y;
      int e_cell_x = e_transform.position.x / m_gridSize.x;

      if(e_cell_y == 0 || e_cell_y == y_cell_number) { top_bottom = true; }
      else if(e_cell_x == 0 || e_cell_x == x_cell_number) { right_left = true; }




      //print("Cell number", 0, e_cell);
      //print("Cell number", 0, e_cell_x);

    }


    for(auto ball : m_entityManager.getEntities("Ball"))
    {
      auto & b_transform = ball->getComponent<CTransform>();
      Vec2 ball_overlap = m_physics.GetOverlap(ball, e);
        
      if(ball_overlap.x > 0 && ball_overlap.y > 0)
      {
        std::cout << "Overlap" << std::endl;
        if(top_bottom)
        {
          
          b_transform.velocity.y = -6.0;

        }else if(right_left)
        {
          b_transform.velocity.x *= -1.0;
        }
        
      }

    }




  }

  auto& p_transform = m_player->getComponent<CTransform>();


  for(auto e : m_entityManager.getEntities("Ball"))
  {
    for(auto h : m_entityManager.getEntities("bullet"))
    {
      Vec2 previous_collision = m_physics.GetPreviousOverlap(h, e);
      Vec2 current_collision = m_physics.GetOverlap(h, e);

      if(current_collision.x > 0 && current_collision.y > 0)
      {
          splitBall(e);
          print("Hit!");
      }

      
    }
  }



  if(p_transform.position.y > m_game->getWindow().getSize().y)
  {
    m_player->destroy();
    spawnPlayer();
  }




}

void Scene_Play::sDoAction(const Action &action) 
{

    if (action.type() == "START") {
    
    if(action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; }
    else if(action.name() == "QUIT"){m_game->toMenu();}
    else if(action.name() == "TOGGLE_COLLISION") { m_drawCollision = ! m_drawCollision; }
    else if(action.name() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid; }
    else if(action.name() == "PAUSE") { setPaused(!m_paused); }
    else if(action.name() == "QUIT") { onEnd();}
    else if(action.name() == "RIGHT")
    {

      m_player->getComponent<CInput>().right = true;
      
    }else if(action.name() == "JUMP")
    {

      m_player->getComponent<CInput>().up = true;
      
    }else if(action.name() == "LEFT")
    {

      m_player->getComponent<CInput>().left = true;
      
    }


  } else if (action.type() == "END")
  {
    if(action.name() == "RIGHT")
    {

      m_player->getComponent<CInput>().right = false;
      
    }else if(action.name() == "JUMP")
    {

      m_player->getComponent<CInput>().up = false;
      
    }else if(action.name() == "DOWN")
    {

      m_player->getComponent<CInput>().down = false;
      
    }else if(action.name() == "LEFT")
    {

      m_player->getComponent<CInput>().left = false;
      
    }else if(action.name() == "FIRE")
    {
      std::cout << "Fire!" << std::endl;
      m_player->getComponent<CState>().firing = true;
      spawnBullet(m_player);
    }



  }
}

void Scene_Play::sAnimation() {
  

  for(auto e : m_entityManager.getEntities())
  {
    if(e->hasComponent<CAnimation>())
    {
      auto& e_animation = e->getComponent<CAnimation>();
      auto& e_state = e->getComponent<CState>();

      if(e_animation.animation.getName() == "Brick" && e_state.state == "hit")
      {
        std::cout << "Brick has been hit" << std::endl;
        e->addComponent<CAnimation>(m_game->getAssets().get_animation("Explosion"), false);
      }

      if(e_animation.animation.getName() == "Question" && e_state.state == "hit")
      {
        std::cout << "Question has been hit" << std::endl;
        e->addComponent<CAnimation>(m_game->getAssets().get_animation("Question2"), true);
        spawnCoin(e);
      }

      
      if(e_animation.repeat)
      {
        e_animation.animation.update();
      }else
      {
        if(!e_animation.animation.hasEnded())
        {
          e_animation.animation.update();
        }else
        {
          e->destroy();
        }
        
      }


    }
    
  }

  auto& p_animation = m_player->getComponent<CAnimation>();

  auto& p_state = m_player->getComponent<CState>();

  auto& p_transform = m_player->getComponent<CTransform>();

  if(p_state.state == "jumping" || p_state.state == "falling")
  {
    m_player->addComponent<CAnimation>(m_game->getAssets().get_animation("Air"), true);
  }else if(p_state.state == "standing" && p_animation.animation.getName() != "Stand")
  {
        m_player->addComponent<CAnimation>(m_game->getAssets().get_animation("Stand"), true);
      }else if(p_state.state == "running" && p_animation.animation.getName() != "Run")
      {
      m_player->addComponent<CAnimation>(m_game->getAssets().get_animation("Run"), true);
      }


  if(p_state.direction == "right") p_transform.scale = Vec2(
        p_transform.scale.x > 0 ?  p_transform.scale.x : p_transform.scale.x * -1,
        p_transform.scale.y > 0 ?  p_transform.scale.y : p_transform.scale.y * -1);
      else if(p_state.direction == "left") p_transform.scale = Vec2(
        p_transform.scale.x < 0 ?  p_transform.scale.x : p_transform.scale.x * -1,
        p_transform.scale.y > 0 ?  p_transform.scale.y : p_transform.scale.y * -1);


 
}

void Scene_Play::onEnd() {
  
}

void Scene_Play::sRender()
{
  
	if (!m_paused) { m_game->window().clear(sf::Color(255, 100, 255)); }
	else		   { m_game->window().clear(sf::Color(50, 50, 150)); }

  
	auto &player_position= m_player->getComponent<CTransform>().position;
  
	float window_center_x= std::max(m_game->window().getSize().x / 2.0f, player_position.x);
  
	if (m_drawTextures)
	{
		for (auto e : m_entityManager.getEntities())
		{
			auto &transform= e->getComponent<CTransform>();

			if (e->hasComponent<CAnimation>())
			{
				auto &animation= e->getComponent<CAnimation>().animation;
				animation.getSprite().setRotation(transform.angle);
				animation.getSprite().setPosition(transform.position.x, transform.position.y);
				animation.getSprite().setScale(transform.scale.x, transform.scale.y);
				m_game->window().draw(animation.getSprite());
			}
		}
	}


	if (m_drawCollision)
	{
		for (auto e : m_entityManager.getEntities())
		{
			if (e->hasComponent<CBounding_box>())
			{
				auto &box= e->getComponent<CBounding_box>();
				auto &transform= e->getComponent<CTransform>();
				sf::RectangleShape rectangle;
				rectangle.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
				rectangle.setOrigin(sf::Vector2f(box.half_size.x, box.half_size.y));
				rectangle.setPosition(transform.position.x, transform.position.y);
				rectangle.setFillColor(sf::Color(0, 0, 0, 0));
				rectangle.setOutlineColor(sf::Color(255, 255, 255, 255));
				rectangle.setOutlineThickness(1);
				m_game->window().draw(rectangle);
			}
		}
	}


	if (m_drawGrid)
	{
		float left_x= m_game->window().getView().getCenter().x - width() / 2;
		float right_x= left_x + width() + m_gridSize.x;
		float next_grid_x= left_x - ((int)left_x % (int)m_gridSize.x);

		for (float x= next_grid_x; x < right_x; x+= m_gridSize.x)
		{
			drawLine(Vec2(x, 0), Vec2(x, height()));
		}

		for (float y= 0; y < height(); y+= m_gridSize.y)
		{
			drawLine(Vec2(left_x, height() - y), Vec2(right_x, height() - y));

			for (float x= next_grid_x; x < right_x; x+= m_gridSize.x)
			{
				std::string x_cell= std::to_string((int)x / (int)m_gridSize.x);
				std::string y_cell= std::to_string((int)y / (int)m_gridSize.y);
				m_gridText.setString("( " + x_cell + " , " + y_cell + " )");
				m_gridText.setPosition(x + 3, height() - y - m_gridSize.y + 2);
        m_gridText.setCharacterSize(10);
				m_game->window().draw(m_gridText);
			}
		}
	}
}

void Scene_Play::update()
{

	m_currentFrame++;
	m_entityManager.update();

	if (!m_paused)
	{
		sMovement();
		sLifespan();
		sAnimation();
    sCreate();
	}

	sCollision();
  sClean();
	sRender();
}

bool Scene_Play::checkBottom()
{
  bool is_there_a_bottom = false;
  for(auto e : m_entityManager.getEntities("Tile"))
  {

    if(e->hasComponent<CAnimation>())
    {

      Vec2 overlap = m_physics.GetOverlap(m_player, e);
      float distance = abs(e->getComponent<CTransform>().position.y - m_player->getComponent<CTransform>().position.y);

      bool distant = distance > e->getComponent<CAnimation>().animation.getSize().y;

      if(overlap.x > 0 && !distant) is_there_a_bottom = true;
      
      


     

    }

  }

    return is_there_a_bottom;

}

template<typename T1, typename T2>
void Scene_Play::print(const std::string& body, const T1& first, const T2& second) {
    std::stringstream ss;
    ss << body << "(" << first << "," << second << ")";
    std::cout << ss.str() << std::endl;
}

void Scene_Play::print(std::string body)
{
  std::cout << body << std::endl;
}

void Scene_Play::spawnCoin(std::shared_ptr<Entity> tile)
{
  auto& t_transform = tile->getComponent<CTransform>();
  auto coin = m_entityManager.addEntity("Coin");
  coin->addComponent<CAnimation>(m_game->getAssets().get_animation("Coin"), true);
  coin->addComponent<CLifespan>(300, 0);
  coin->addComponent<CTransform>(Vec2(t_transform.position.x, t_transform.position.y - 64));

}

void Scene_Play::sCreate()
{
  if(m_player->hasComponent<CState>())
  {
    auto& p_state = m_player->getComponent<CState>();

    if(p_state.firing)
    {
      std::cout << "Creating" << std::endl;
      auto bulletHead = m_entityManager.getEntities("bullet")[0];
      if(bulletHead->hasComponent<CFrameCounter>()) 
      {
        auto& b_frame_counter = bulletHead->getComponent<CFrameCounter>();
        std::cout << "Frame Counter: " << b_frame_counter.frameCount << std::endl;
        if((b_frame_counter.frameCount % 2) == 0)
        {
          spawnChain(bulletHead);
        }
      }
      
      
      
    }

  }

  
}

void Scene_Play::spawnChain(std::shared_ptr<Entity> bulletHead)
{
  auto& p_state = m_player->getComponent<CState>();
  auto& h_transform = bulletHead->getComponent<CTransform>();
  auto& h_bounding_box = bulletHead->getComponent<CBounding_box>();
  auto chain_entity = m_entityManager.addEntity("chain");
  chain_entity->addComponent<CAnimation>(m_game->getAssets().get_animation("Chain"), true);
  h_transform.position = Vec2(h_transform.position.x, h_transform.position.y - m_gridSize.y);
  chain_entity->addComponent<CTransform>(Vec2(h_transform.position.x, h_transform.position.y + h_bounding_box.half_size.y + 8));
  chain_entity->addComponent<CBounding_box>(chain_entity->getComponent<CAnimation>().animation.getSize());
}

void Scene_Play::destroyChainBullet()
{
  std::cout << "Destroy" << std::endl;
  for (auto e : m_entityManager.getEntities("chain"))
  {
    e->destroy();
  }

  for(auto e : m_entityManager.getEntities("bullet"))
  {
    e->destroy();
  }
}

void Scene_Play::sClean()
{
  auto& bullet = m_entityManager.getEntities("bullet");
  auto& p_state = m_player->getComponent<CState>();

  if(!p_state.firing && bullet.empty())
  {
    for(auto e : m_entityManager.getEntities("chain"))
    {
      e->destroy();
    }
  }
}

void Scene_Play::splitBall(std::shared_ptr<Entity> ball)
{
  auto b_transform = ball->getComponent<CTransform>();
  auto b_bounding_box = ball->getComponent<CBounding_box>();

  ball->destroy();

  
  Vec2 b1_position(b_transform.position.x + b_bounding_box.half_size.x, b_transform.position.y);

  Vec2 b2_position(b_transform.position.x - b_bounding_box.half_size.x, b_transform.position.y);

  auto ball_1 = m_entityManager.addEntity("Ball");

  auto ball_2 = m_entityManager.addEntity("Ball");

  Vec2 scale(2,2);

  Vec2 b1_velocity(b_transform.velocity.x, b_transform.velocity.y);
  Vec2 b2_velocity(b_transform.velocity.x *(-1), b_transform.velocity.y);



  ball_1->addComponent<CAnimation>(m_game->getAssets().get_animation("Ball"), true);
  ball_1->addComponent<CTransform>(b1_position, b1_velocity, scale, 0);

  ball_2->addComponent<CAnimation>(m_game->getAssets().get_animation("Ball"), true);
  ball_2->addComponent<CTransform>(b2_position, b2_velocity, scale, 0);

  auto b1_animation = ball_1->getComponent<CAnimation>();
  auto b2_animation = ball_2->getComponent<CAnimation>();



  b1_animation.animation.setScale(scale.x, scale.y);
  b2_animation.animation.setScale(scale.x, scale.y);

  ball_1->addComponent<CBounding_box>(b1_animation.animation.getSize());
  ball_2->addComponent<CBounding_box>(b2_animation.animation.getSize());


  ball_1->addComponent<CGravity>(0.03);
  ball_2->addComponent<CGravity>(0.03);





}