#pragma once

#include "Common.h"
#include "Animation.h"
#include "Assets.h"

class Component
{
public:
	bool has= false;
};
class CTransform : public Component
{
public: 
	Vec2 position=			{ 0.0, 0.0 };
	Vec2 previous_position=	{ 0.0, 0.0 };
	Vec2 scale=				{ 1.0, 1.0 };
	Vec2 velocity=			{ 0.0, 0.0 };
	float  angle=				0;

	CTransform() {}
	CTransform(const Vec2 &p)
		: position(p) {}
	CTransform(const Vec2 &p, const Vec2 &sp, const Vec2 &sc, float a)
		: position(p), previous_position(p), velocity(sp), scale(sc), angle(a) {}
};

class CLifespan : public Component
{
public:
	int lifespan= 0;
	int frame_created= 0;

	CLifespan() {}
	CLifespan(int duration, int frame)
		: lifespan(duration), frame_created(frame) {}
};

class CInput : public Component
{
public:
	bool up=		false;
	bool down=		false;
	bool left=		false;
	bool right=		false;
	bool shoot=		false;
	bool can_shoot= true;
	bool can_jump=	true;

	CInput(){}
};

class CBounding_box : public Component
{
public:
	Vec2 size;
	Vec2 half_size;

	CBounding_box() {}
	CBounding_box(const Vec2 &s)
		: size(s), half_size(s.x / 2, s.y / 2) {}
};

class CAnimation : public Component
{
public:
	Animation animation;
	bool repeat= false;
	
	CAnimation() {}
	CAnimation(const Animation &animation, bool r)
		: animation(animation), repeat(r) {}
};

class CGravity : public Component
{
public:
	float gravity= 0;

	CGravity() {}
	CGravity(float g) : gravity(g) {}
};

class CState : public Component
{
public:
	std::string state= "jumping";
	std::string direction = "right";
	
	CState() {}
	CState(const std::string &s, const std::string &d) : state(s), direction(d) {}
};