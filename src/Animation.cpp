#include "../include/Animation.h"
#include <cmath>
#include<iostream>

Animation::Animation() {}

Animation::Animation(const std::string & name, const sf::Texture &t)
    : Animation(name, t, 1, 0) {}

Animation::Animation(const std::string & name, const sf::Texture &t,
                     size_t frameCount, size_t speed)
    : m_name(name), m_sprite(t), m_frameCount(frameCount), m_currentFrame(0), m_speed(speed) {
  m_size = Vec2((float)t.getSize().x / frameCount, (float)t.getSize().y);
  m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
  m_sprite.setTextureRect(sf::IntRect(std::floor(m_currentFrame) * m_size.x, 0,
                                      m_size.x, m_size.y));
}

// updates the animation to show the next frame, depending on its speed
// animation loops when it reaches the end
void Animation::update() {
  m_currentFrame++;
  

  if(m_speed > 0)
  {
    size_t animation_frame = (m_currentFrame / m_speed) % m_frameCount;

    //if(m_name == "Stand") std::cout << "Animation Frame: " << animation_frame << std::endl;

   //if(m_name == "Stand") std::cout << "m_currentFrame: " << m_currentFrame << std::endl;


    //if(m_name == "Stand") std::cout << "m_speed: " << m_speed << std::endl;


    //if(m_name == "Stand") std::cout << "m_frameCount: " << m_frameCount << std::endl;

    //if(m_name == "Question") std::cout << "Before Scale: " << "(" << m_sprite.getScale().x << "," << m_sprite.getScale().y << ")" << std::endl;

    //std::cout << "Rect: [" << animation_frame * m_size.x << "," << 0 << "," << m_size.x << "," << m_size.y << "]" << std::endl;

    if(m_name != "BulletHead")
    {
      m_sprite.setTextureRect(sf::IntRect(animation_frame * m_size.x, 0, m_size.x, m_size.y));
    }

    //if(m_name == "Question") std::cout << "After Scale: " << "(" << m_sprite.getScale().x << "," << m_sprite.getScale().y << ")" << std::endl;

    
  }
  

  //TODO:
  //1) calculate the correct frame of animation to play based on currentFrame and speed
  //2) set the tecture rectangle properly (see constructor for sample)

}

bool Animation::hasEnded() const {
    // TODO: detect when animation has ended (last frame was played) and return true


    if((m_currentFrame / m_speed) == (m_frameCount - 1))
    {
      std::cout << "End of Animation: " << m_name << " reached" << std::endl;
      return true;
    }





    return false;
}

const std::string & Animation::getName() const { return m_name; }

const Vec2 &Animation::getSize() const { return m_size; }

sf::Sprite &Animation::getSprite() { return m_sprite; }

void Animation::setScale(float x, float y)
{
  m_size.x *= x;
  m_size.y *= y;
  //m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
  //m_sprite.setTextureRect(sf::IntRect(std::floor(m_currentFrame) * m_size.x, 0,
                                      //m_size.x, m_size.y));
}
