#pragma once

#include "../Math/Vector2D.h"

//Singleton
#define CAMERA Camera2D::getInstance()

class Camera2D
{
friend std::ostream& operator<<(std::ostream& os, const Camera2D& camera);
private:
	//Vetor com a posição da câmera
	math::Vector2D position;
	//Ponteiro para o construtor
	static Camera2D *myInstance;
	//Construtor privado
	explicit Camera2D();
	//Evita Cópia da Classe
	Camera2D(const Camera2D &);
	Camera2D &operator= (const Camera2D &);
public:
	//Singleton
	static Camera2D &getInstance();

	virtual ~Camera2D(void);

	math::Vector2D getPos();
	void setPos(math::Vector2D pos);
	void setPos(float x,float y);

	float getX();
	float getY();
	void setX(float x);
	void setY(float y);
};

