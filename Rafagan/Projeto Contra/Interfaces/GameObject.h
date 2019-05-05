#pragma once

class GameManager;

class GameObject
{
public:
	virtual ~GameObject(){}

	virtual void init(GameManager*) = 0;
	virtual void destroy() = 0;
	virtual void update(float dtSecs) = 0;
	virtual void draw() const = 0;
};
