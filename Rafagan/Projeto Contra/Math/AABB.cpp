#include "AABB.h"

#include "../definitions.h"

using namespace math;

AABB::AABB()
{
}

AABB::AABB(float x,float y,float w,float h)
	:position(x,y),size(w,h)
{
}

AABB::AABB(math::Vector2D initialPos,math::Vector2D _size)
	:position(initialPos),size(_size)
{
}

void AABB::updatePosition(float x,float y)
{
	position.set(x,y);
}

void AABB::updatePosition(math::Vector2D pos)
{
	position = pos;
}

bool AABB::testColision(AABB box) const
{
	/*Método de teste para ver se a box a ser comparada está fora*/

	//Testa se a região esquerda da box está fora desta box
	if(getPosX() > box.getFinalPosX())
		return false;
	//Testa se a região direita da box está fora desta box
	if(getFinalPosX() < box.getPosX())
		return false;
	//Testa se a região acima da box está fora desta box
	if(getPosY() > box.getFinalPosY())
		return false;
	//Testa se a região abaixo da box está fora desta box
	if(getFinalPosY() < box.getPosY())
		return false;

	//Se houve colisão, então retorna true
	return true;
}

math::Vector2D AABB::getPos() const
{
	return this->position;
}

float AABB::getPosX() const
{
	return this->position.x;
}

float AABB::getPosY() const
{
	return this->position.y;
}

Vector2D AABB::getFinalPos() const
{
	return (this->position + this->size);
}

float AABB::getFinalPosX() const
{
	return (this->position.x + this->size.x);
}

float AABB::getFinalPosY() const
{
	return (this->position.y + this->size.y);
}

AABB::~AABB(void)
{
}

math::Vector2D AABB::getSize() const
{
	return this->size;
}

float AABB::getWidth() const
{
	return this->size.x;
}

float AABB::getHeight() const
{
	return this->size.y;
}

void AABB::draw() const
{
	C2D2P_RetanguloPintado(static_cast<int>(this->position.x),static_cast<int>(this->position.y),
		static_cast<int>(this->getFinalPosX()),static_cast<int>(this->getFinalPosY()),255,0,0);
}
