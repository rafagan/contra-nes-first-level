#pragma once

#include "Vector2D.h"

class AABB
{
private:
	math::Vector2D position, size;
public:
	explicit AABB();
	explicit AABB(float initialX,float initialY,float width,float height);
	explicit AABB(math::Vector2D initialPos,math::Vector2D size);

	void updatePosition(float x,float y);
	void updatePosition(math::Vector2D pos);

	bool testColision(AABB box) const;

	math::Vector2D getPos() const;
	float getPosX() const;
	float getPosY() const;

	math::Vector2D getFinalPos() const;
	float getFinalPosX() const;
	float getFinalPosY() const;

	math::Vector2D getSize() const;
	float getWidth() const;
	float getHeight() const;

	void draw() const;

	~AABB(void);
};

