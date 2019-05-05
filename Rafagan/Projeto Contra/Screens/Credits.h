#pragma once
#include "../definitions.h"
#include "../Interfaces/Screen.h"

class Credits : public Screen
{
private:
	bool menu, wasDestroyed;
	C2D2_Botao *keys;
	GameManager* game;

	Screen* newScreen;

	float moveCredits;

	unsigned int creditsFont;
	unsigned int thankYouFont;

	//fundo e botao select
	int back, select;
	int xSelect, ySelect;

	//botao how to play e suas posicoes
	unsigned int buttonOptions;
	int xButtonOptions, yButtonOptions;

	int music;
	
public:
	Credits(void);
	~Credits(void);

	virtual Screen* nextScreen();

	virtual void init(GameManager* game);
	virtual void destroy();
	virtual void update( float secs );
	virtual void draw() const;
};

