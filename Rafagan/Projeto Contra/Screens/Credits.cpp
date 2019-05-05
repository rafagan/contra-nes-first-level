#include "Credits.h"
#include "../Helper/Renderer.h"
#include "Menu.h"
#include "../Helper/Loader.h"
#include "Options.h"

#include "../GameManager.h"
#include "../Helper/sound.h"

using namespace math;

Credits::Credits(void)
{
}

Credits::~Credits(void)
{
	if (!wasDestroyed)
		this->destroy();
}

Screen* Credits::nextScreen()
{
	return this->newScreen;
}

void Credits::init(GameManager* game)
{
	this->game = game;
	back = Loader::loadSprite("Res/Backgrounds/CreditsBack.png", 0, 0);
	select = Loader::loadSprite("Res/Buttons/select.png", 0, 0);
	creditsFont = Loader::loadFont("Res/Fonts/GameFont32.png", 32);
	thankYouFont = Loader::loadFont("Res/Fonts/GameFont64.png", 64);
	music = Sound::loadMusic("Res/Audio/Mus/EnergyZone.ogg");

	xButtonOptions = SWIDTHM - 80;
	xSelect = xButtonOptions - 80;
	yButtonOptions = 520;
	ySelect = yButtonOptions - 5;

	CA2_PausaMusica();
	CA2_TocaMusica(music, 1);

	moveCredits = 255;
	game->getCamera()->setY(-moveCredits);
	keys = C2D2_PegaTeclas();
	this->newScreen = this;
}

void Credits::destroy()
{
	CA2_RemoveMusica(music);
	C2D2_RemoveSpriteSet(back);
	C2D2_RemoveSpriteSet(select);
	this->wasDestroyed = true;
	game->getCamera()->setPos(0,0);
}

void Credits::update(float secs)
{
	game->getCamera()->setY(-moveCredits);

	if(moveCredits > -440)
		moveCredits -= 50 * secs;
	else {
		if (keys[C2D2_ENTER].pressionado)
			newScreen = new Menu();
	}	
}

void Credits::draw() const
{
	game->getRenderer()->drawText(creditsFont, Vector2D(400, 300), "Rafagan - Programmer", 0, true);
	game->getRenderer()->drawText(creditsFont, Vector2D(400, 400), "Lucas - Programmer", 0, true);
	game->getRenderer()->drawText(creditsFont, Vector2D(400, 500), "Ewerton - Artist", 0, true);
	game->getRenderer()->drawText(creditsFont, Vector2D(400, 600), "Keli Cruz - Artist", 0, true);
	game->getRenderer()->drawText(thankYouFont, Vector2D(400, 800), "Thank You For Playing!!!", 0, true);

	C2D2P_RetanguloPintado(0, 0, 800, 280, 0, 0, 0);
	C2D2P_RetanguloPintado(0, 500, 800, 600, 0, 0, 0);
	game->getRenderer()->drawSpriteHud(back, 0, Vector2D(0, 0));
	game->getRenderer()->drawTextHud(creditsFont, Vector2D(400, 230), "Credits", 0, false);
	if(moveCredits <= -440)
		game->getRenderer()->drawSpriteHud(select, 0, Vector2D(static_cast<float>(xSelect), static_cast<float>(ySelect)));
	game->getRenderer()->drawTextHud(
		creditsFont, Vector2D(static_cast<float>(xButtonOptions), static_cast<float>(yButtonOptions)), 
		"Options", C2D2_TEXTO_ESQUERDA, false);
}
