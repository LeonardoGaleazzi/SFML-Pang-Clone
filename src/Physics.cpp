#include "../include/Physics.h"
#include "../include/Vec2.h"

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
  // TODO: return the overlap rectangle size of the bounding boxes of entity a
  // and b
  //std::cout << "Overlap Start" << std::endl;
  auto& aTransform = a->getComponent<CTransform>();
  auto& bTransform = b->getComponent<CTransform>();

  auto& aBoudingBox = a->getComponent<CBounding_box>();
  auto& bBoudingBox = b->getComponent<CBounding_box>();



  Vec2 center_distance(abs(aTransform.position.x - bTransform.position.x), abs(aTransform.position.y - bTransform.position.y));



  Vec2 overlap(aBoudingBox.half_size.x + bBoudingBox.half_size.x - (float) center_distance.x, 
  aBoudingBox.half_size.y + bBoudingBox.half_size.y - (float) center_distance.y);



  Vec2 result(0,0);



  if(overlap.x > 0.0f)
  {

    result.x = overlap.x;

  }

  if(overlap.y > 0.0f)
  {

    result.y = overlap.y;

  }



  return result;

  std::cout << "Overlap End" << std::endl;

}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a,
                                 std::shared_ptr<Entity> b) {
  auto& aTransform = a->getComponent<CTransform>();
  auto& bTransform = b->getComponent<CTransform>();

  auto& aBoudingBox = a->getComponent<CBounding_box>();
  auto& bBoudingBox = b->getComponent<CBounding_box>();



  Vec2 center_distance(abs(aTransform.previous_position.x - bTransform.previous_position.x), abs(aTransform.previous_position.y - bTransform.previous_position.y));



  Vec2 overlap(aBoudingBox.half_size.x + bBoudingBox.half_size.x - (float) center_distance.x, 
  aBoudingBox.half_size.y + bBoudingBox.half_size.y - (float) center_distance.y);



  Vec2 result(0,0);



  if(overlap.x > 0.0f)
  {

    result.x = overlap.x;

  }

  if(overlap.y > 0.0f)
  {

    result.y = overlap.y;

  }



  return result;

}
