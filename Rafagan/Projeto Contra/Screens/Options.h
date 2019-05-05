#pragma once
#include "../Interfaces/Screen.h"
#include <memory>
#include "../definitions.h"

class Options : public Screen
{
private:
	bool wasDestroyed;
	C2D2_Botao *keys;
	GameManager* game;

	//fundo e botao select
	int back, select;
	int xSelect, ySelect;

	//fonte para a tela de options
	unsigned int optionsFont;

	//botao how to play e suas posicoes
	unsigned int buttonMenu;
	int xButtonH, yButtonH;

	//botao Credits e suas posicoes
	unsigned int buttonCredits;
	int xButtonM, yButtonM;

	//botao How To Play e suas posicoes
	unsigned int buttonHowToPlay;
	int xButtonC, yButtonC;

	Screen *newScreen;
public:
	explicit Options();
	virtual ~Options();

	virtual Screen* nextScreen();
	virtual void init(GameManager* game);
	virtual void destroy();
	virtual void update( float secs );
	virtual void draw() const;

};

