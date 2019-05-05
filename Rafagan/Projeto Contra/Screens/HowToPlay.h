#pragma once
#include "../Interfaces/Screen.h"
#include <memory>
#include "../definitions.h"

class HowToPlay : public Screen
{
private:
	bool menu, wasDestroyed;
	C2D2_Botao *keys;
	GameManager* game;

	//fundo e botao select
	unsigned int background,select;
	int xSelect, ySelect;

	//botao how to play e suas posicoes
	unsigned int buttonHowToPlay;
	int xButtonHowToPlay, yButtonHowToPlay;

	//Fonte da tela how to play
	unsigned int howFont;

	Screen *newScreen;

public:
	explicit HowToPlay();
	virtual ~HowToPlay();

	virtual Screen* nextScreen();
	virtual void init(GameManager* game);
	virtual void destroy();
	virtual void update( float secs );
	virtual void draw() const;
	
};

