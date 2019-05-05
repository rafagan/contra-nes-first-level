#include "TimeManager.h"
#include <c2d2/chien2d2.h>


TimeManager::TimeManager(unsigned int fps)
	:FPS(fps)
{
	gameSpeed = 1.0f;
	startTime = ((C2D2_TempoSistema()-1) * gameSpeed);
	before = 0.0f;
	currentTime = 0.0f;
}


TimeManager::~TimeManager(void)
{
}

void TimeManager::update()
{
	//Tempo desde o início da aplicação (em milissegundos)
	this->currentTime = (C2D2_TempoSistema() * gameSpeed) - startTime;
	//dt (em segundos)
	this->deltaTime = toSeconds(currentTime - before);
	this->before = currentTime;
}

void TimeManager::setGameSpeed( float speed )
{
	float sp = speed;
	sp = (sp <= 0.001f ? 0.001f : (sp >= 100.0f ? 100.0f : sp));
	this->gameSpeed = sp;
}

float TimeManager::getGameSpeed() const
{
	return gameSpeed;
}