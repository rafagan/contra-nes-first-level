#include "Options.h"
#include "../Helper/Renderer.h"
#include "Menu.h"
#include "../Helper/Loader.h"
#include "Credits.h"
#include "HowToPlay.h"

#include "../GameManager.h"

using namespace math;

Options::Options()
{
}

Options::~Options(void)
{
	if (!wasDestroyed)
		this->destroy();
}

void Options::init(GameManager* game)
{
	this->game = game;
	back = Loader::loadSprite("Res/Backgrounds/back.png", 0, 0);
	select = Loader::loadSprite("Res/Buttons/select.png", 0, 0);
	optionsFont = Loader::loadFont("Res/Fonts/GameFont32.png", 32);

	xButtonH = xButtonM = xButtonC = SWIDTH/2 - 220;
	xSelect = xButtonH - 90;
	yButtonH = 350;
	ySelect =  yButtonH - 5;
	yButtonC = yButtonH + 50;
	yButtonM = yButtonC + 50;

	this->newScreen = this;
}

void Options::destroy()
{
	C2D2_RemoveFonte(optionsFont);
	C2D2_RemoveSpriteSet(back);
	C2D2_RemoveSpriteSet(select);
	this->wasDestroyed = true;
}

void Options::update( float secs )
{
	keys = C2D2_PegaTeclas();

	if(keys[C2D2_ENTER].pressionado && ySelect == (yButtonH - 5))
		newScreen = new HowToPlay();

	if(keys[C2D2_ENTER].pressionado && ySelect == (yButtonC - 5))
		newScreen = new Credits();

	if(keys[C2D2_ENTER].pressionado && ySelect == (yButtonM -5 ))
		newScreen = new Menu();

	if(keys[C2D2_RSHIFT].pressionado){
		if(ySelect < yButtonM - 5)
			ySelect += 50;
		else ySelect = yButtonH - 5;
	}
}

void Options::draw() const
{
	game->getRenderer()->drawSpriteHud(back, 0, Vector2D(0, 0));
	game->getRenderer()->drawSpriteHud(select, 0, 
		Vector2D(static_cast<float>(xSelect), static_cast<float>(ySelect)));
	game->getRenderer()->drawTextHud(optionsFont, 
		Vector2D(static_cast<float>(xButtonH), static_cast<float>(yButtonH)), "How to Play", C2D2_TEXTO_ESQUERDA,false);
	game->getRenderer()->drawTextHud(optionsFont, 
		Vector2D(static_cast<float>(xButtonC), static_cast<float>(yButtonC)), "Credits", C2D2_TEXTO_ESQUERDA,false);
	game->getRenderer()->drawTextHud(optionsFont, 
		Vector2D(static_cast<float>(xButtonM), static_cast<float>(yButtonM)), "Main Menu", C2D2_TEXTO_ESQUERDA,false);
}

Screen* Options::nextScreen()
{
	return newScreen;
}
