#pragma once

#include <string>
#include <vector>
#include "Renderer.h"

class Animator
{
private:
	std::string name;
	int wf,hf;
	int frame;
	float timePerFrame,timePass;
	std::vector<int> frames;

	unsigned int image;
public:
	//Construtor de passagem da altura e largura do quadro
	explicit Animator(std::string name,int widthFrames,int heightFrames,float timePerFrame,int totalFrames);
	//Construtor de passagem de quadros
	explicit Animator(std::string name,int numFramesCol,int numFramesRow,int imageWidth, int imageHeight,float timePerFrame);
	//Construtor de passagem da altura e largura e sequencia de quadros
	explicit Animator(std::string name,int widthFrames,int heightFrames,float timePerFrame,std::vector<int> frames);
	//Construtor de passagem de quadros e sequencia de quadros
	explicit Animator(std::string name,int numFramesCol,int numFramesRow,int imageWidth, int imageHeight,float timePerFrame,std::vector<int> frames);

	//Retorna um vetor com a largura e altura do quadro
	const int *getWqHq();
	//Retorna o número total de quadros
	int getNumFrames();

	//Seta uma nova animação automática
	void setQuads(std::vector<int> frames);

	//Desenha a animação MANUALMENTE (É responsabilidade do desenvolvedor tratar o tempo por quadro)
	void drawSprite(int frame,int x,int y);
	void drawSprite(int frame,math::Matrix3 matrix, bool isHUD);
	//Desenha a animação com quads ja definidos. Caso o método setQuads ou o terceiro construtor não tenham sido chamados, por padrão este método desenha os quadros sequenciais da imagem
	void drawSprite(int x,int y);
	void drawSprite(math::Matrix3 matrix,bool isHUD);

	//Seta e captura o tempo por frame
	void setTimePerFrame(float secs);
	float getTimePerFrame();

	//Atualiza o quadro
	void updateFrame(float secs);

	~Animator(void);
};
