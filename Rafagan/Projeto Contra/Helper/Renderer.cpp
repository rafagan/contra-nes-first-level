#include "Renderer.h"

using namespace math;
using namespace math::rh;

#include "../definitions.h"

Renderer *Renderer::myInstance = nullptr;
Renderer &Renderer::getInstance()
{
	//Lazy load
	if (myInstance == nullptr)
		myInstance = new Renderer();
	else
		throw std::logic_error("O Singleton Renderer só pode ser acessado pelo GameManager");

	return *myInstance;
}

void Renderer::setCamera(Camera2D* camera)
{
	this->camera = camera;
}

void Renderer::drawSprite(unsigned int id,unsigned int index,math::Vector2D position,bool centered)
{
	math::Vector2D cam = toCamera(position);

	if(centered){
		int dx,dy;
		C2D2_DimensoesSprite(id,&dx,&dy);
		position += Vector2D(dx/2.0f,dy/2.0f);
	}

	C2D2_DesenhaSprite(id,index,static_cast<int>(cam.x),static_cast<int>(cam.y));
}

void Renderer::drawSpriteHud(unsigned int id,unsigned int index,math::Vector2D position,bool centered)
{
	if(centered){
		int dx,dy;
		C2D2_DimensoesSprite(id,&dx,&dy);
		position += Vector2D(dx/2,dy/2);
	}

	C2D2_DesenhaSprite(id,index,static_cast<int>(position.x),static_cast<int>(position.y));
}

void Renderer::drawSpriteMatrix(const math::Matrix3 &matrix,unsigned int id,unsigned int index,Uint8 r,Uint8 g,Uint8 b,Uint8 a,bool centered)
{
	//recebe as dimensões do sprite
	int x,y;
	C2D2_DimensoesSprite(id,&x,&y);
	if(centered){
		x/=2;
		y/=2;
	}
	 
	//Guarda em vetores as extremidades do sprite(os quadrantes da chien funcionam com os sinais --,+-,++,-+)
	Vector2D vetor[4] = {Vector2D(-x,-y),Vector2D(x,-y),Vector2D(x,y),Vector2D(-x,y)};
	 
	//captura os valores das posições e desenha
	int x1[4],y1[4];
	for(int i=0;i<4;i++){
	 	//Por converter para coordenadas da camera, o sprite nao sera HUD
	 	Vector2D vector = transform(matrix,toCamera((vetor[i])));
	 	x1[i] = (int)vector.x;
	 	y1[i] = (int)vector.y;
	}
	C2D2_DesenhaSpriteEfeito(id,index,x1,y1,r,g,b,a);
}

void Renderer::drawSpriteMatrix(const math::Matrix3 &matrix,unsigned int id,unsigned int index,bool centered)
{
	drawSpriteMatrix(matrix,id,index,255,255,255,255,centered);
}

void Renderer::drawSpriteMatrixHud(const math::Matrix3 &matrix,unsigned int id,unsigned int index,Uint8 r,Uint8 g,Uint8 b,Uint8 a,bool centered)
{
	//recebe as dimensões do sprite
	int x,y;
	C2D2_DimensoesSprite(id,&x,&y);
	if(centered){
		x/=2;
		y/=2;
	}
	 
	//Guarda em vetores as extremidades do sprite(os quadrantes da chien funcionam com os sinais --,+-,++,-+)
	Vector2D vetor[4] = {Vector2D(-x,-y),Vector2D(x,-y),Vector2D(x,y),Vector2D(-x,y)};
	 
	//captura os valores das posições e desenha
	int x1[4],y1[4];
	for(int i=0;i<4;i++){
	 	Vector2D vector = transform(matrix, vetor[i]);
	 	x1[i] = (int)vector.x;
	 	y1[i] = (int)vector.y;
	}
	C2D2_DesenhaSpriteEfeito(id,index,x1,y1,r,g,b,a);
}

void Renderer::drawSpriteMatrixHud(const math::Matrix3 &matrix,unsigned int id,unsigned int index,bool centered)
{
	drawSpriteMatrixHud(matrix,id,index,255,255,255,255,centered);
}

void Renderer::drawText(unsigned int font,math::Vector2D position,const std::string text,unsigned int alignment,bool centered)
{
	Vector2D cam = toCamera(position);
	if(centered){
		int dx,dy;
		C2D2_DimensoesTexto(font,text.c_str(),&dx,&dy);
		position += Vector2D(dx/2,dy/2);
	}

	C2D2_DesenhaTexto(font,static_cast<int>(cam.x),static_cast<int>(cam.y),text.c_str(),alignment);
}

void Renderer::drawTextHud(unsigned int font,math::Vector2D position,const std::string text,unsigned int alignment,bool centered)
{
	if(centered){
		int dx,dy;
		C2D2_DimensoesTexto(font,text.c_str(),&dx,&dy);
		position += Vector2D(dx/2,dy/2);
	}

	C2D2_DesenhaTexto(font,static_cast<int>(position.x),static_cast<int>(position.y),text.c_str(),alignment);
}
