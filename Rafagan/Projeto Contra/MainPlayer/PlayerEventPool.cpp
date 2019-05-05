#include "PlayerEventPool.h"

#include <memory>

using namespace std;

PlayerEventPool::PlayerEventPool(Hero *_character, const UINT _directionals[4],const UINT _BUTTON1, const UINT _BUTTON2, const UINT _START_BUTTON, const UINT _SELECT_BUTTON)
	:character(_character),BUTTON1(_BUTTON1),BUTTON2(_BUTTON2),START_BUTTON(_START_BUTTON),SELECT_BUTTON(_SELECT_BUTTON)
{
	for (int i=0;i<4;i++)
		directionals[i] = _directionals[i];
}


PlayerEventPool::~PlayerEventPool(void)
{
	this->character = nullptr;
}

//Realiza o envio de eventos para a fila de eventos do ator
//O tratamento desses eventos será realizado chamando ATOR_ProximoEvento()
void PlayerEventPool::poolEvents()
{
	C2D2_Botao *keys = C2D2_PegaTeclas();
	Ator *a = this->character->getStruct();
	shared_ptr<MapUtil> map = character->getCurrentLevel()->getMap();
	Evento ev;

	//Botão CIMA
	if(keys[directionals[0]].pressionado)
	{
		ev.tipoEvento = EVT_PRESSIONOU_CIMA;
		ATOR_EnviaEvento(a, &ev);
	}
	if(keys[directionals[0]].liberado)
	{
		ev.tipoEvento = EVT_LIBEROU_CIMA;
		ATOR_EnviaEvento(a, &ev);
	}

	//Botão BAIXO
	if(keys[directionals[1]].pressionado)
	{
		ev.tipoEvento = EVT_PRESSIONOU_BAIXO;
		ATOR_EnviaEvento(a, &ev);
	}
	if(keys[directionals[1]].liberado)
	{
		ev.tipoEvento = EVT_LIBEROU_BAIXO;
		ATOR_EnviaEvento(a, &ev);
	}

	//Botão ESQUERDA
	if(keys[directionals[2]].pressionado)
	{
		ev.tipoEvento = EVT_PRESSIONOU_ESQ;
		ATOR_EnviaEvento(a, &ev);
	}
	if(keys[directionals[2]].liberado)
	{
		ev.tipoEvento = EVT_LIBEROU_ESQ;
		ATOR_EnviaEvento(a, &ev);
	}

	//Botão DIREITA
	if(keys[directionals[3]].pressionado)
	{
		ev.tipoEvento = EVT_PRESSIONOU_DIR;
		ATOR_EnviaEvento(a, &ev);
	}
	if(keys[directionals[3]].liberado)
	{
		ev.tipoEvento = EVT_LIBEROU_DIR;
		ATOR_EnviaEvento(a, &ev);
	}

	//Botão 01
	if(keys[BUTTON1].pressionado)
	{
		ev.tipoEvento = EVT_PRESSIONOU_BOTAO1;
		ATOR_EnviaEvento(a, &ev);
	}
	if(keys[BUTTON1].liberado)
	{
		ev.tipoEvento = EVT_LIBEROU_BOTAO1;
		ATOR_EnviaEvento(a, &ev);
	}

	//Botão 02
	if(keys[BUTTON2].pressionado)
	{
		ev.tipoEvento = EVT_PRESSIONOU_BOTAO2;
		ATOR_EnviaEvento(a, &ev);
	}
	if(keys[BUTTON2].liberado)
	{
		ev.tipoEvento = EVT_LIBEROU_BOTAO2;
		ATOR_EnviaEvento(a, &ev);
	}

	//Botão Start
	if(keys[START_BUTTON].pressionado)
	{
		ev.tipoEvento = EVT_PRESSIONOU_PAUSA;
		ATOR_EnviaEvento(a, &ev);
	}
	if(keys[START_BUTTON].liberado)
	{
		ev.tipoEvento = EVT_LIBEROU_PAUSA;
		ATOR_EnviaEvento(a, &ev);
	}

	//Botão Select
	if(keys[SELECT_BUTTON].pressionado)
	{
		ev.tipoEvento = EVT_PRESSIONOU_BOTAO3;
		ATOR_EnviaEvento(a, &ev);
	}
	if(keys[SELECT_BUTTON].pressionado)
	{
		ev.tipoEvento = EVT_LIBEROU_BOTAO3;
		ATOR_EnviaEvento(a, &ev);
	}

	//Evento de colisão com armadilha do cenário
	if(map->testCollisionWithTrapTilemap(a))
	{
		ev.tipoEvento = EVT_COLIDIU_ARMADILHA;
		ATOR_EnviaEvento(a, &ev);
	}

	// Evento de colisão com o bloco de final de fase
	if(map->testCollisionWithEndLevelTilemap(a))
	{
		ev.tipoEvento = EVT_FIM_FASE;
		ATOR_EnviaEvento(a, &ev);
	}

	// Evento de colisão com o bloco de final de fase
	if(map->testCollisionWithSolidTilemap(a))
	{
		ev.tipoEvento = EVT_COLIDIU_PAREDE;
		ATOR_EnviaEvento(a, &ev);
	}

	if(map->testCollisionWithCheckpoint(a))
	{
		ev.tipoEvento = EVT_CHECKPOINT;
		ATOR_EnviaEvento(a, &ev);
	}
}
