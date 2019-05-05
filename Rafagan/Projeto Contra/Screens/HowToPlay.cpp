#include "HowToPlay.h"
#include "../Helper/Renderer.h"
#include "Menu.h"
#include "../Helper/Loader.h"
#include "Options.h"

#include "../GameManager.h"

using namespace math;

HowToPlay::HowToPlay()
{
}

HowToPlay::~HowToPlay(void)
{
	if (!wasDestroyed)
		this->destroy();
}

void HowToPlay::init(GameManager* game)
{
	this->game = game;
	background = Loader::loadSprite("Res/Backgrounds/HowToPlay.png",0,0);
	select = Loader::loadSprite("Res/Buttons/select.png", 0, 0);
	howFont = Loader::loadFont("Res/Fonts/GameFont32.png", 32); 

	xSelect = SWIDTH/2 - 130;
	ySelect = 505;

	this->newScreen = this;
}

void HowToPlay::destroy()
{
	C2D2_RemoveSpriteSet(background);
	this->wasDestroyed = true;
}

void HowToPlay::update( float secs )
{
	keys = C2D2_PegaTeclas();

	if (keys[C2D2_ENTER].pressionado)
		newScreen = new Menu();
}

void HowToPlay::draw() const
{
	game->getRenderer()->drawSpriteHud(background, 0, Vector2D(0, -50));
	game->getRenderer()->drawSpriteHud(select,0,Vector2D(260, 530));
	game->getRenderer()->drawTextHud(howFont, Vector2D(50, 50), "How To Play", C2D2_TEXTO_CENTRALIZADO, true);
	game->getRenderer()->drawTextHud(howFont, Vector2D(350, 530), "Menu", C2D2_TEXTO_CENTRALIZADO, true);
}

Screen* HowToPlay::nextScreen()
{
	return this->newScreen;
}
