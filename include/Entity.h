#pragma once

#include "Components.h"
#include <string>
#include <tuple>

class EntityManager;

typedef std::tuple<
    CTransform,
    CLifespan,
    CInput,
    CBounding_box,
    CAnimation,
    CGravity,
    CState,
    CFrameCounter>
    ComponentsTuple;

class Entity {
  friend class EntityManager;

  bool m_active = true;
  std::string m_tag = "default";
  size_t m_id = 0;


  ComponentsTuple m_components;

  // constructor is private se we can never create
  // entities outside the EntityManager which had friend access
  Entity(const size_t& id, const std::string &entityTag);

  public:
  void destroy();
  size_t id() const;
  bool isActive() const;
  const std::string& tag() const;

  template<typename T>
  bool hasComponent() const{
    return getComponent<T>().has;
  }

  template<typename T, typename... TArgs>
  T &addComponent(TArgs &&...mArgs) {
    auto &component = getComponent<T>();
    component = T(std::forward<TArgs>(mArgs)...);
    component.has =true ;
    return component;
  }

  template<typename T>
  T &getComponent() { return std::get<T>(m_components); }


  template<typename T>
	const T &getComponent() const
	{
		return std::get<T>(m_components);
	}

  template<typename T>
  void removeComponent() { 
    auto& component = getComponent<T>();
    component.has = false;  }
};
