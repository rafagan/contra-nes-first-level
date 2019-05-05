#include "Camera2D.h"

using namespace math;

Camera2D *Camera2D::myInstance = nullptr;
Camera2D &Camera2D::getInstance()
{
	//Lazy load
	if (myInstance == nullptr)
		myInstance = new Camera2D();
	else
		throw std::logic_error("O Singleton Camera2D só pode ser acessado pelo GameManager");

	return *myInstance;
}

Camera2D::Camera2D()
	:position(Vector2D(0,0))
{
}

Camera2D::~Camera2D(void)
{
}

Vector2D Camera2D::getPos()
{
	return this->position;
}

void Camera2D::setPos(Vector2D pos)
{
	this->position = pos;
}

void Camera2D::setPos(float x,float y)
{
	this->position.set(x,y);
}

float Camera2D::getX()
{
	return this->position.x;
}

float Camera2D::getY()
{
	return this->position.y;
}

void Camera2D::setX(float x)
{
	this->position.x = x;
}

void Camera2D::setY(float y)
{
	this->position.y = y;
}

std::ostream& operator<<(std::ostream& os, const Camera2D& camera)
{
	os << "Camera Position: " << camera.position;

	return os;
}