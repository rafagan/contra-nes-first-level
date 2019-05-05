#include "Menu.h"
#include "../Helper/Renderer.h"
#include "../Helper/Sound.h"

#include "Options.h"
#include "Credits.h"
#include "../Levels/LevelOne.h"

#include "../Helper/Loader.h"

#include "../GameManager.h"

using namespace math;

Menu::Menu(void)
	:move(SWIDTH)
{
}

Menu::~Menu(void)
{
	if (!wasDestroyed)
		this->destroy();
}

Screen* Menu::nextScreen()
{
	return this->newScreen;
}

void Menu::init(GameManager* game)
{
	this->game = game;

	back = Loader::loadSprite("Res/Backgrounds/back.png", 0, 0);
	select = Loader::loadSprite("Res/Buttons/select.png", 0, 0);
	music = Sound::loadMusic("Res/Audio/Mus/Menu.ogg");
	menuFont = Loader::loadFont("Res/Fonts/GameFont32.png", 32);

	xButtonPlay2 = xbuttonOptions = xButtonPlay1 = SWIDTH/4 - 35;
	yButtonPlay1 = SWIDTHM - 50;
	yButtonPlay2 = yButtonPlay1 + 50;
	yButtonOptions = yButtonPlay2 + 50;
	xSelect = xButtonPlay1 - 90;
	ySelect = yButtonPlay1 - 5;
	playedMusic = false;
	this->newScreen = this;

	CA2_PausaMusica();
}

void Menu::destroy()
{
	C2D2_RemoveFonte(menuFont);
	C2D2_RemoveSpriteSet(back);
	C2D2_RemoveSpriteSet(buttonPlay1);
	C2D2_RemoveSpriteSet(buttonPlay2);
	C2D2_RemoveSpriteSet(buttonOptions);
	C2D2_RemoveSpriteSet(select);
	this->wasDestroyed = true;

	game->getCamera()->setPos(0,0);
}

void Menu::update( float secs )
{
	C2D2_Botao *key = C2D2_PegaTeclas();

	if(move > 0){
		move -= 5;
		game->getCamera()->setX(-move);
	}
	else
	{
		if(!playedMusic){
			Sound::playMusic(music, 1);
			playedMusic = true;
		}

		if(key[C2D2_ENTER].pressionado && ySelect == (yButtonPlay1 - 5)){
			Sound::pauseMusic();
			CA2_RemoveMusica(music);
			newScreen = new LevelOne(1);
		}

		if(key[C2D2_ENTER].pressionado && ySelect == (yButtonPlay2 - 5)){
			Sound::pauseMusic();
			CA2_RemoveMusica(music);
			newScreen = new LevelOne(2);
		}

		if(key[C2D2_ENTER].pressionado && ySelect == (yButtonOptions - 5))
			newScreen = new Options();

		if(key[C2D2_RSHIFT].pressionado){
			if(ySelect < yButtonOptions - 5)
				ySelect += 50;
			else ySelect = yButtonPlay1 - 5;
		}
	}
}

void Menu::draw() const
{
	game->getRenderer()->drawSprite(back, 0, Vector2D(0, 0));

	//se o fundo ja estiver em sua posicao certa
	if(move == 0)
	{
		game->getRenderer()->drawSpriteHud(select, 0, 
			Vector2D(static_cast<float>(xSelect),static_cast<float>(ySelect)));
		game->getRenderer()->drawTextHud(
			menuFont, 
			Vector2D(static_cast<float>(xButtonPlay1), static_cast<float>(yButtonPlay1 - 50)),
			"Player Select", C2D2_TEXTO_ESQUERDA,  false);
		game->getRenderer()->drawTextHud(
			menuFont, 
			Vector2D(static_cast<float>(xButtonPlay1), static_cast<float>(yButtonPlay1)),
			"1 Player", C2D2_TEXTO_ESQUERDA,  false);
		game->getRenderer()->drawTextHud(
			menuFont, 
			Vector2D(static_cast<float>(xButtonPlay2), static_cast<float>(yButtonPlay2)), 
			"2 Players", C2D2_TEXTO_ESQUERDA, false);
		game->getRenderer()->drawTextHud(menuFont, 
			Vector2D(static_cast<float>(xbuttonOptions), static_cast<float>(yButtonOptions)), 
			"Options", C2D2_TEXTO_ESQUERDA, false);
	}
}

