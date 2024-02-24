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
  registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE"); // Toggle drawing (T)extures
  registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");// Toggle drawing (C)ollisio Boxes
  registerAction(sf::Keyboard::P, "TOGGLE_GRID");// Toggle drawing (G)rid
  registerAction(sf::Keyboard::A, "LEFT");
  registerAction(sf::Keyboard::W, "JUMP");
  registerAction(sf::Keyboard::D, "RIGHT");
  registerAction(sf::Keyboard::R, "FIRE");
  //  TODO: Register all other gameplay Actions

  m_gridText.setCharacterSize(12);
  m_gridText.setFont(m_game->getAssets().get_font("Arial"));

  loadLevel(levelPath);
}

Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY,
                                std::shared_ptr<Entity> entity) {
  //      TODO: This function takes in a grid (x,y) position and an Entity
  //            Return a Vec2 indicating where the CENTER position of the Entity
  //            should be You must use the Entity's Animation size to position
  //            it correctly The size of the grid width and height is stored in
  //            m_gridSize.x and m_gridSize.y The up-left corner of the
  //            Animation should align with the bottom left of the grid cell

  size_t windowHeight = m_game->window().getSize().y;

  Vec2 blBlock = Vec2(m_gridSize.x *gridX, (windowHeight - m_gridSize.y * gridY));

  
    auto& eAnimation = entity->getComponent<CAnimation>();

    Vec2 animation_size = eAnimation.animation.getSize();
  
  animation_size.y *= -1;

  animation_size /= 2;



  Vec2 delta = blBlock + animation_size;

  //std::cout << "blBlock x: " << blBlock.x << " " << "blBlock y: " << blBlock.y << std::endl;

  //std::cout << "animation_size x: " << animation_size.x << " " << "animation_size y: " << animation_size.y << std::endl;

  //std::cout << "delta x: " << delta.x << " " << "delta y: " << delta.y << std::endl;


  

  return delta;
}



void Scene_Play::loadLevel(const std::string &filename) {
  //  reset the entity manager every time we load a level
  m_entityManager = EntityManager();
  // TODO : read in the level file and add the appropriate entities
  //        use the PlayerConfig struct m_playerConfig to store player
  //        proprieties this struct is defined at the top of Scene_Play.h

  // NOTE : all the code below is a sample code which shows you how to
  //        set up and use entities with the new syntax, it should be removed




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
          //std::cout << "animationName: " << animationName << " " << "x: " << x << " " << "y: " << y << std::endl;
			    auto decEntity = m_entityManager.addEntity(entityTag);
          decEntity->addComponent<CAnimation>(m_game->getAssets().get_animation(animationName), true);

          auto & b_animation = decEntity->getComponent<CAnimation>();

          Vec2 scale(m_game->getWindow().getSize().x / b_animation.animation.getSize().x, m_game->getWindow().getSize().y / b_animation.animation.getSize().y);

          //std::cout << "Window y: " << m_game->getWindow().getSize().y << std::endl;
          //std::cout << "Animation y: " << decEntity->getComponent<CAnimation>().animation.getSize().y << std::endl;

          b_animation.animation.setScale(scale.x, scale.y);


          decEntity->addComponent<CTransform>(gridToMidPixel(x, y, decEntity));
          decEntity->addComponent<CState>("normal", "right");

          //auto & b_animation = decEntity->getComponent<CAnimation>();

          auto & b_transform = decEntity->getComponent<CTransform>();

          b_transform.scale = scale;


          //b_animation.animation.getSprite().setScale(2,2);

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


    /*
    spawnPlayer();
    // some sample entities
    auto brick = m_entityManager.addEntity("tile");
    // IMPORTANT : always add the CAnimation component first so that
    //gridToMidPixel can compute correctly
    brick->addComponent<CAnimation>(m_game->getAssets().get_animation("Brick"), true);
    //brick->addComponent<CTransform>(Vec2(96,480));
    brick->addComponent<CTransform>(gridToMidPixel(10, 6, brick));





    //gridToMidPixel(1, 1, brick);
    // NOTE : you finally code should position the entity with the grid x,y
    //position read from the file:

    //brick->addComponent<CTranform>(gridToMidPixel(gridX, gridY, brick));


    if(brick->getComponent<CAnimation>().animation.getName() == "Brick")
    {
      std::cout << "This could be a good way of identifying if a tile is a brick!\n";
    }


    auto block = m_entityManager.addEntity("tile");
    block->addComponent<CAnimation>(m_game->getAssets().get_animation("Block"),
    true); 
    block->addComponent<CTransform>(Vec2(224, 480));
    // add a bounding box, this will now show up if we press the 'C' key
    block->addComponent<CBounding_box>(m_game->getAssets().get_animation("Block").getSize());

  
    auto question = m_entityManager.addEntity("tile");
    question->addComponent<CAnimation>(m_game->getAssets().get_animation("Question"),true); 
    question->addComponent<CTransform>(Vec2(352, 480));

    */

  // NOTE : THIS IS INCREDIBLY IMPORTANT PLEASE READ THIS EXAMPLE
  //        Components are now returned as reference rather than pointers
  //        if you do not specify a reference variable type, it will COPY the
  //        component here is an example
  //
  //        This will COPY the transform into the variable 'transform1' - it's
  //        INCORRECT any changes you make to transform1 will not be changed
  //        inside the entity auto transform1 = entity->get<CTransform>();
  //
  //        This will REFERENCE the transform with the variable 'transform2' -
  //        it's CORRECT Now any changes you make to transform2 will be changed
  //        inside the entity auto & transform2 = entity->get<CTransform>()
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
  // TODO(due: Friday): implement all the spawn enemy stuff
  // outh use the goomba animation and be added as Tile entity
  // see the current implementation of how the goomba is built
  // form the loadLevel method
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
   //print(std::string("Player Velocity"), std::to_string(print_transform.velocity.x), std::to_string(print_transform.velocity.y));
  // REMEMBER: SFML's (0,0) position is in the TOP-Left corner
  //           this means jumping will have negative y-component
  //           and gravity will have positive y-component
  //           Also, something BELOW something else will have a y value GREATER
  //           than it Also, something ABOVE something else will have a y value
  //           LOWER than it
  // TODO: implement Physics::GetOverlap() function, use it inside this function
  // TODO: implement bullet / tile collisions
  //       destroy the tile if it has a Brick animation
  // TODO: implement player / tile collisions and resolutions
  //       update the CState component of the player to store whether
  //       used by the Animation system
  // TODO: Check to see if the player has fallen doen a hole (y > height())
  // TODO: Don't let the player walk off the left side of the map  
  auto& p_state = m_player->getComponent<CState>();
  //std::cout << "Player State: " << p_state.state << std::endl;

  bool is_there_a_bottom = checkBottom();
  //std::cout << is_there_a_bottom << std::endl;

  for(auto e : m_entityManager.getEntities("Tile"))
  {

    Vec2 previous_collision = m_physics.GetPreviousOverlap(m_player, e);
    Vec2 current_collision = m_physics.GetOverlap(m_player, e);
    auto& player_transform = m_player->getComponent<CTransform>();
    auto& e_transform = e->getComponent<CTransform>();
    auto& e_state = e->getComponent<CState>();
    

    if(current_collision.x > 0 && current_collision.y > 0)
    {

      if(e->getComponent<CAnimation>().animation.getName() != "Ground")
      {
        //std::cout << "Current Collision: (" << current_collision.x << "," << current_collision.y << ")" << std::endl;
        //std::cout << "Previous Collision: (" << previous_collision.x << "," << previous_collision.y << ")" << std::endl;
        //std::cout << "Player Velocity: (" << player_transform.velocity.x << "," << player_transform.velocity.y << ")" << std::endl;
      }

      //std::cout << "Collision Detected!" << std::endl;

      if(previous_collision.x > 0)
      {
        //std::cout << "Playey Position: (" << player_transform.position.x << "," << player_transform.position.y << ")" << std::endl;
        //std::cout << "Tile Position: (" << e_transform.position.x << "," << e_transform.position.y << ")" << std::endl;
        //std::cout << "Previous horizontal overlap" << std::endl;
        if(player_transform.position.y < e_transform.position.y)
        {
          //std::cout << "Collision From Above!" << std::endl;
          //std::cout << "Playey Position: (" << player_transform.position.x << "," << player_transform.position.y << ")" << std::endl;
          //std::cout << "Tile Position: (" << e_transform.position.x << "," << e_transform.position.y << ")" << std::endl;
          player_transform.position.y -= current_collision.y;
          //std::cout << "COLLIDED" << std::endl;
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
          //std::cout << "Collision From Above!" << std::endl;
          //std::cout << "Playey Position: (" << player_transform.position.x << "," << player_transform.position.y << ")" << std::endl;
          //std::cout << "Tile Position: (" << e_transform.position.x << "," << e_transform.position.y << ")" << std::endl;
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
      //std::cout << "Bullet collision: (" << bullet_collision.x << "," << bullet_collision.y << ")" << std::endl;


    if(bullet_collision.x > 0 && bullet_collision.y > 0 && e->tag() == "Tile")
    {
      if(e->getComponent<CAnimation>().animation.getName() == "Ground")
      {
        std::cout << "Brick Hit!" << std::endl;
        //e_state.state = "hit";
        //bullet->destroy();
        p_state.firing = false;
        destroyChainBullet();
      }else
      {
        //bullet->destroy();
      }
      //std::cout << "Tag: " << e->tag() << std::endl;
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
    
  }

  auto& p_transform = m_player->getComponent<CTransform>();

  //std::cout << "Window Height: " << m_game->getWindow().getSize().y << std::endl;

  //std::cout << "Player Height: " << p_transform.position.y << std::endl;   

  if(p_transform.position.y > m_game->getWindow().getSize().y)
  {
    //std::cout << "Fallen!" << std::endl;
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
  // TODO: complete the Animation class code first

  // TODO: set the animation of the player based on its CState component
  // TODO: for each entity with an animation, call
  // entity->getComponent<CAnimation>().animation.update();
  //       if the animation is not repeated, and it has ended, destroy the
  //       entity

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

      /*
      if(e_animation.animation.getName() == "Question")
      {
        sf::Sprite& question_sprite = e_animation.animation.getSprite();
        Vec2 size = e_animation.animation.getSize();

        question_sprite.setTextureRect(sf::IntRect(3*size.x, 0, size.x, size.y));
      }

      */

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
  // TODO: when the scene ends, change back to the MENU scene
  //       use m_game->changeState(correct params)
}

void Scene_Play::sRender()
{
  //std::cout<<"Zero" << std::endl;
	// color the background darker so you know that the game is paused
	if (!m_paused) { m_game->window().clear(sf::Color(255, 100, 255)); }
	else		   { m_game->window().clear(sf::Color(50, 50, 150)); }

  //std::cout<<"Zero1" << std::endl;
	// set the viewpoint of the window to be centered on the player if it's far enough right
	auto &player_position= m_player->getComponent<CTransform>().position;
  //std::cout<<"player_position: " << player_position.x << " " << player_position.y <<std::endl;
  //std::cout<<"Zero2" << std::endl;
	float window_center_x= std::max(m_game->window().getSize().x / 2.0f, player_position.x);
  //std::cout<<"Zero3" << std::endl;
	//sf::View view= m_game->window().getView();
  //std::cout<<"Zero4" << std::endl;
	//view.setCenter(window_center_x, m_game->window().getSize().y - view.getCenter().y);
  //std::cout<<"Zero5" << std::endl;
	//m_game->window().setView(view);

  //std::cout<<"One" << std::endl;

	// draw all Entity textures / animations
	if (m_drawTextures)
	{
		for (auto e : m_entityManager.getEntities())
		{
			auto &transform= e->getComponent<CTransform>();

			if (e->hasComponent<CAnimation>())
			{
        //if(e->getComponent<CAnimation>().animation.getName() == "Background") std::cout << e->getComponent<CAnimation>().animation.getSprite()
				auto &animation= e->getComponent<CAnimation>().animation;
				animation.getSprite().setRotation(transform.angle);
				animation.getSprite().setPosition(transform.position.x, transform.position.y);
				animation.getSprite().setScale(transform.scale.x, transform.scale.y);
				m_game->window().draw(animation.getSprite());
			}
		}
	}

  //std::cout<<"Two" << std::endl;

	// draw all Entity collision bounding boxes with a rectangle shape
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

  //std::cout<<"Three" << std::endl;

	// draw the grid so that students can easily debug
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
  //std::cout << "Main Loop Start: " << std::endl;

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
  //std::cout << "Check" << std::endl;
  bool is_there_a_bottom = false;
  for(auto e : m_entityManager.getEntities("Tile"))
  {
      //std::cout << "For" << std::endl;

    if(e->hasComponent<CAnimation>())
    {
      //std::cout << "Calculate" << std::endl;

      Vec2 overlap = m_physics.GetOverlap(m_player, e);
      float distance = abs(e->getComponent<CTransform>().position.y - m_player->getComponent<CTransform>().position.y);

      bool distant = distance > e->getComponent<CAnimation>().animation.getSize().y;

      if(overlap.x > 0 && !distant) is_there_a_bottom = true;
      
      


     

    }

  }
    //std::cout << "Leaving" << std::endl;

    return is_there_a_bottom;

}


void Scene_Play::print(std::string body, const std::string first, const std::string second)
{

  std::cout << body << "(" << first << "," << second << ")" << std::endl;

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
  //std::cout << "Spawning chain" << std::endl;
  auto chain_entity = m_entityManager.addEntity("chain");
  chain_entity->addComponent<CAnimation>(m_game->getAssets().get_animation("Chain"), true);
  h_transform.position = Vec2(h_transform.position.x, h_transform.position.y - m_gridSize.y);
  chain_entity->addComponent<CTransform>(Vec2(h_transform.position.x, h_transform.position.y + h_bounding_box.half_size.y + 8));
  chain_entity->addComponent<CBounding_box>(chain_entity->getComponent<CAnimation>().animation.getSize());
  //p_state.firing = false;
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
