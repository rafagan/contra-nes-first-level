#pragma once

#include <string>

#include "../Math/AffineTransform.h"
#include "Camera2D.h"
#include <SDL_stdinc.h>
#include <c2d2/chien2d2.h>

//Singleton
#define RENDERER Renderer::getInstance()

class Renderer{
private:
	static Renderer *myInstance;

	Camera2D* camera;

	explicit Renderer(){};
	Renderer(const Renderer &);
	Renderer &operator= (const Renderer &);
public:
	//Singleton
	static Renderer &getInstance();

	virtual ~Renderer(void) {};

	void setCamera(Camera2D* camera);

	//Métodos básicos para atualização da tela
	inline void sync(const unsigned int FPS);
	inline void clean();

	//Converte de coordenadas da câmera para coordenadas da tela
	inline math::Vector2D toScreen(const math::Vector2D &position);
	inline math::Vector2D toScreen(float x,float y);

	//Converte de coordenadas da tela para coordenadas da câmera
	inline math::Vector2D toCamera(const math::Vector2D &position);
	inline math::Vector2D toCamera(float x,float y);

	//Desenho sem Matriz
	void drawSprite(unsigned int id,unsigned int index,math::Vector2D position,bool centered = false);
	void drawSpriteHud(unsigned int id,unsigned int index,math::Vector2D position,bool centered = false);

	//Desenha um sprite de acordo com as coordenadas da camera
	void drawSpriteMatrix(const math::Matrix3 &matrix,unsigned int id,unsigned int index,Uint8 r,Uint8 g,Uint8 b,Uint8 a,bool centered = false);
	void drawSpriteMatrix(const math::Matrix3 &matrix,unsigned int id,unsigned int index,bool centered = false);

	//Desenha um sprite sem contar as coordenadas da câmera
	void drawSpriteMatrixHud(const math::Matrix3 &matrix,unsigned int id,unsigned int index,Uint8 r,Uint8 g,Uint8 b,Uint8 a,bool centered = false);
	void drawSpriteMatrixHud(const math::Matrix3 &matrix,unsigned int id,unsigned int index,bool centered = false);

	//Desenho de textos
	void drawText(unsigned int font,math::Vector2D position,const std::string text,unsigned int alignment,bool centered = false);
	void drawTextHud(unsigned int font,math::Vector2D position,const std::string text,unsigned int alignment,bool centered = false);
};

inline void Renderer::sync(const unsigned int FPS)
{
	C2D2_Sincroniza(FPS);
}

inline void Renderer::clean()
{
	C2D2_LimpaTela();
}

inline math::Vector2D Renderer::toScreen(const math::Vector2D &position)
{
	return position + camera->getPos();
}

inline math::Vector2D Renderer::toScreen(float x,float y)
{
	return math::Vector2D(x,y) + camera->getPos();
}

inline math::Vector2D Renderer::toCamera(const math::Vector2D &position)
{
	return position - camera->getPos();
}

inline math::Vector2D Renderer::toCamera(float x,float y)
{
	return math::Vector2D(x,y) - camera->getPos();
}