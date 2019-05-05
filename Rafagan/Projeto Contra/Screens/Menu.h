#pragma once
#include "../Interfaces/Screen.h"
#include "../definitions.h"
#include "../Abstract/Character.h"

#include <memory>

class Menu : public Screen
{
private:
	bool wasDestroyed, playedMusic;
	C2D2_Botao *keys;
	GameManager* game;

	//fundo e botao select
	int back, select;
	int xSelect, ySelect;

	//fonte para desenhar as opcoes
	unsigned int menuFont;

	//botao how to play e suas posicoes
	unsigned int buttonPlay1;
	int xButtonPlay1, yButtonPlay1;

	//responsavel pela rolagem da tela
	int move;

	//botao how to play e suas posicoes
	unsigned int buttonPlay2;
	int xButtonPlay2, yButtonPlay2;

	//botao Credits e suas posicoes
	unsigned int buttonCredits;
	int xButtonCredits, yButtonCredits;

	//botao de sair e suas posicioes
	unsigned int buttonOptions;
	int xbuttonOptions, yButtonOptions;

	int music;

	Screen* newScreen;
public:
	explicit Menu();
	virtual ~Menu(void);
	virtual void init(GameManager* game);
	virtual void destroy();

	//Game Object
	virtual Screen* nextScreen();
	virtual void update( float secs );
	virtual void draw() const;
};

