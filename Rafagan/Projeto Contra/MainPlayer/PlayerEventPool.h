#pragma once

#include "../Interfaces/Hero.h"

/*
 * Essa classe respeita os eventos de um joystick estilo ao do NES
 * Ordem de direcionais: CIMA, BAIXO, TRAS, FRENTE
 */

class PlayerEventPool
{
private:
	Hero *character;
	const unsigned int BUTTON1, BUTTON2, START_BUTTON, SELECT_BUTTON;
	unsigned int directionals[4];
public:
	explicit PlayerEventPool(Hero *character, 
		const UINT directionals[4],const UINT BUTTON1, const UINT BUTTON2, const UINT START_BUTTON, const UINT SELECT_BUTTON);
	virtual ~PlayerEventPool(void);

	void poolEvents();
};

