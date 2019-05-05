#include "statesMainPlayer.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////

HeroSt::Borning::Borning(void)
{
}


HeroSt::Borning::~Borning(void)
{
}

void HeroSt::Borning::enter(Hero* _actor)
{
	//Por padrão, já estamos em ATOR_NASCENDO!
	_actor->getStruct()->vidas=3;
	_actor->getStruct()->estado.subestado=ESTADO_RODANDO;
}

State<Hero>* HeroSt::Borning::process( float secs )
{
	return new Starting();
}

void HeroSt::Borning::leave()
{
}

//////////////////////////////////////////////////////////////////////////

HeroSt::Dead::Dead(void)
{
}


HeroSt::Dead::~Dead(void)
{
}

void HeroSt::Dead::enter( Hero* actor )
{
	this->actor = actor;
	auto a = actor->getStruct();

	if(a->direcao == 0)
		actor->setAnimation(11);
	else 
		actor->setAnimation(12);

	a->velocidade=0;
	a->estado.estado = ATOR_MORTO;
	a->y=0;
	a->x=100;
	a->estado.subestado = ESTADO_RODANDO;
	nextState = this;
}

State<Hero>* HeroSt::Dead::process( float secs )
{
	auto a = actor->getStruct();

	if(a->vidas>0)
	{
		a->direcao=0;

		nextState = new Idle();
	}
	else
		nextState = new Finish();

	return nextState;
}

void HeroSt::Dead::leave()
{
	ATOR_LimpaEvento(this->actor->getStruct());
}


//////////////////////////////////////////////////////////////////////////

HeroSt::Finish::Finish(void)
{
}


HeroSt::Finish::~Finish(void)
{
}

void HeroSt::Finish::enter( Hero* actor )
{
	auto a = actor->getStruct();
	Evento ev;

	a->estado.subestado=ESTADO_RODANDO;
	a->velocidade=0;

	ev.tipoEvento = EVT_FIM_FASE;
	ev.subtipo = SUBEVT_FIM_FASE_DERROTA;
	ATOR_EnviaEventoJogo(&ev);
}

State<Hero>* HeroSt::Finish::process( float secs )
{
	return this;
}

void HeroSt::Finish::leave()
{
	actor->getStruct()->estado.estado = ATOR_ENCERRADO;
	ATOR_LimpaEvento(this->actor->getStruct());
}


//////////////////////////////////////////////////////////////////////////

HeroSt::Starting::Starting(void)
{
}


HeroSt::Starting::~Starting(void)
{
}

void HeroSt::Starting::enter( Hero* actor )
{
	auto a = actor->getStruct();
	a->olhandoPara=0;
	a->naTela = true;

	//Identifica que o Ator irá para um estado não nativo da Chien
	ATOR_TrocaEstado(a, ATOR_PROG_INI, false);
}

State<Hero>* HeroSt::Starting::process( float secs )
{
	return new Idle();
}

void HeroSt::Starting::leave()
{

}

//////////////////////////////////////////////////////////////////////////

HeroSt::Idle::Idle(void)
{
}


HeroSt::Idle::~Idle(void)
{
}

void HeroSt::Idle::enter(Hero* _actor)
{
	this->actor = _actor;
	Ator *a = actor->getStruct();

	actor->setWalkVelocity(0);
	a->estado.estado=ATOR_PROG_INI;
	a->estado.subestado=ESTADO_RODANDO;

	if(a->direcao==0)
		actor->setAnimation(0);
	else
		actor->setAnimation(1);

	this->nextState = actor->getFirstStateLevelOne();
	if(nextState == nullptr)
		nextState = this;
}

State<Hero>* HeroSt::Idle::process( float secs )
{
	if(nextState != this)
		return nextState;

	Ator *a = actor->getStruct();
	Evento ev;

	while(ATOR_ProximoEvento(a, &ev))
	{
		nextState = this->testEventType(ev,a);

		if(nextState != this)
			break;
	}

	return nextState;
}

void HeroSt::Idle::leave()
{
}

State<Hero>* HeroSt::Idle::testEventType( Evento &ev, Ator *a )
{
	State<Hero>* st = this;

	switch(ev.tipoEvento)
	{
	case EVT_PRESSIONOU_BOTAO1:
		st = new Jumping();
		break;
	case EVT_PRESSIONOU_ESQ:
		st = new Walking();
		actor->setDirection(180);
		break;
	case EVT_PRESSIONOU_DIR:
		st = new Walking();
		actor->setDirection(0);
		break;
	case EVT_COMECOU_CAIR:
		st = new Falling();
		break;
	case EVT_COLIDIU_ARMADILHA:
		st = new Dying();
		break;
	case EVT_FIM_FASE:
		st = new Winning();
		break;
	case EVT_COLIDIU_PERSONAGEM:
		st = new Dying();
		break;
	}

	return st;
}


//////////////////////////////////////////////////////////////////////////

HeroSt::Jumping::Jumping(void)
{
}


HeroSt::Jumping::~Jumping(void)
{
}

void HeroSt::Jumping::enter(Hero* _actor)
{
	this->actor = _actor;
	auto a = actor->getStruct();

	ATOR_Impulsiona(a, actor->getJumpVelocity());

	a->estado.subestado=ESTADO_RODANDO;
	if(a->direcao==0)
		actor->setAnimation(4);
	else
		actor->setAnimation(5);

	nextState = this;
}

State<Hero>* HeroSt::Jumping::process( float secs )
{
	auto a = actor->getStruct();
	Evento ev;

	while(ATOR_ProximoEvento(a, &ev))
	{
		this->nextState = testEventType(ev,a);

		if(nextState != this)
			break;
	}

	return nextState;
}

void HeroSt::Jumping::leave()
{
}

State<Hero>* HeroSt::Jumping::testEventType( Evento &ev, Ator *a )
{
	State<Hero>* st = this;

	switch(ev.tipoEvento)
	{
	case EVT_TOPO_PULO:
		st = new Falling();
		break;
	case EVT_COLIDIU_PAREDE:
		if(ev.subtipo == SUBEVT_COLIDIU_PAREDE_CIMA)
			st = new Falling();
		break;
	case EVT_PRESSIONOU_ESQ:
		if(a->velocidade==0)
		{
			actor->setWalkVelocity(2);
			actor->setDirection(180);
			actor->setAnimation(3);
		}
		break;
	case EVT_PRESSIONOU_DIR:
		if(a->velocidade==0)
		{
			actor->setWalkVelocity(2);
			actor->setDirection(0);
			actor->setAnimation(2);
		}
		break;
	case EVT_LIBEROU_ESQ:
		if(a->velocidade!=0 && a->direcao == 180)
			actor->setWalkVelocity(0);
		break;
	case EVT_LIBEROU_DIR:
		if(a->velocidade!=0 && a->direcao == 0)
			actor->setWalkVelocity(0);
		break;
	case EVT_SAIU_FORA_MAPA:
		if(ev.subtipo != SUBEVT_SAIU_FORA_MAPA_CIMA)
			st = new Dying();
		break;
	case EVT_COLIDIU_ARMADILHA:
		st = new Dying();
		break;
	case EVT_FIM_FASE:
		st = new Winning();
		break;
	case EVT_COLIDIU_PERSONAGEM:
		st = new Dying();
		break;
	}

	return st;
}

//////////////////////////////////////////////////////////////////////////

HeroSt::Falling::Falling(void)
{
}


HeroSt::Falling::~Falling(void)
{
}

void HeroSt::Falling::enter( Hero* _actor )
{
	this->actor = _actor;
	Ator *a = actor->getStruct();

	a->estado.subestado=ESTADO_RODANDO;
	if(a->direcao==0)
		ATOR_TrocaAnimacao(a, 6);
	else
		ATOR_TrocaAnimacao(a, 7);

	nextState = this;
}

State<Hero>* HeroSt::Falling::process( float secs )
{
	Ator *a = actor->getStruct();
	Evento ev;

	while(ATOR_ProximoEvento(a, &ev))
	{
		nextState = this->testEventType(ev,a);

		if(nextState != this)
			break;
	}

	return nextState;
}

void HeroSt::Falling::leave()
{

}

State<Hero>* HeroSt::Falling::testEventType( Evento &ev, Ator *a )
{
	State<Hero>* st = this;

	switch(ev.tipoEvento)
	{
	case EVT_SAIU_FORA_MAPA:
		if(ev.subtipo != SUBEVT_SAIU_FORA_MAPA_CIMA)
			st = new Dying();
		break;
	case EVT_COLIDIU_PAREDE:
		if(ev.subtipo == SUBEVT_COLIDIU_PAREDE_BAIXO)
			if(a->velocidade == 0)
				st = new Idle();
			else
				st = new Walking();
		break;
	case EVT_PRESSIONOU_ESQ:
		if(a->velocidade==0)
		{
			a->velocidade = actor->getWalkVelocity();
			a->direcao = 180;
			ATOR_TrocaAnimacao(a, 3);
		}
		break;
	case EVT_PRESSIONOU_DIR:
		if(a->velocidade==0)
		{
			a->velocidade = actor->getWalkVelocity();
			a->direcao = 0;
			ATOR_TrocaAnimacao(a, 2);
		}
		break;
	case EVT_LIBEROU_ESQ:
		if(a->velocidade!=0 && a->direcao == 180)
			a->velocidade = 0;
		break;
	case EVT_LIBEROU_DIR:
		if(a->velocidade!=0 && a->direcao == 0)
			a->velocidade = 0;
		break;
	case EVT_COLIDIU_ARMADILHA:
		st = new Dying();
		break;
	case EVT_FIM_FASE:
		st = new Winning();
		break;
	case EVT_COLIDIU_PERSONAGEM:
		st = new Dying();
		break;
	}

	return st;
}

//////////////////////////////////////////////////////////////////////////

HeroSt::Walking::Walking(void)
{
}


HeroSt::Walking::~Walking(void)
{
}

void HeroSt::Walking::enter( Hero* _actor )
{
	this->actor = _actor;
	Ator *a = actor->getStruct();

	actor->setWalkVelocity(2);
	a->estado.subestado=ESTADO_RODANDO;

	if(a->direcao==0)
		actor->setAnimation(2);
	else
		actor->setAnimation(3);

	this->nextState = this;
}

State<Hero>* HeroSt::Walking::process( float secs )
{
	Ator *a = actor->getStruct();
	Evento ev;

	while(ATOR_ProximoEvento(a, &ev))
	{
		nextState = this->testEventType(ev,a);

		if(nextState != this)
			break;
	}

	return nextState;
}


void HeroSt::Walking::leave()
{
}

State<Hero>* HeroSt::Walking::testEventType( Evento &ev, Ator *a )
{
	State<Hero>* st = this;

	switch(ev.tipoEvento)
	{
	case EVT_PRESSIONOU_BOTAO1:
		st = new Jumping();
		break;
	case EVT_LIBEROU_ESQ:
		if(a->direcao==180)
			st = new Idle();
		break;
	case EVT_LIBEROU_DIR:
		if(a->direcao==0)
			st = new Idle();
		break;
	case EVT_COLIDIU_PAREDE:
		if(ev.subtipo==SUBEVT_COLIDIU_PAREDE_CIMA);
		//ATOR_TocaEfeitoTela(a, 1, mapId);
		break;
	case EVT_SAIU_FORA_MAPA:
		if(ev.subtipo != SUBEVT_SAIU_FORA_MAPA_CIMA)
			st = new Dying();
		break;
	case EVT_COLIDIU_ARMADILHA:
		st = new Dying();
		break;
	case EVT_FIM_FASE:
		st = new Winning();
		break;
	case EVT_COLIDIU_PERSONAGEM:
		st = new Dying();
		break;
	}

	return st;
}


//////////////////////////////////////////////////////////////////////////

HeroSt::Winning::Winning(void)
{
}


HeroSt::Winning::~Winning(void)
{
}

void HeroSt::Winning::enter( Hero* _actor )
{
	this->actor = _actor;
	Ator *a = actor->getStruct();

	a->estado.subestado=ESTADO_RODANDO;
	ATOR_TrocaAnimacao(a, 8);
	a->temporizadores[0]=240;
	a->velocidade=0;
	//this->actor->playWinMusic();
}

State<Hero>* HeroSt::Winning::process( float secs )
{
	Ator *a = actor->getStruct();
	Evento ev;

	while(ATOR_ProximoEvento(a, &ev))
	{
		if(ev.tipoEvento==EVT_TEMPO) //Revisar esse disparador de eventos de jogo aqui
		{
			ev.tipoEvento = EVT_FIM_FASE;
			ev.subtipo = SUBEVT_FIM_FASE_VITORIA;
			ATOR_EnviaEventoJogo(&ev);
		}
	}

	return this;
}

void HeroSt::Winning::leave()
{

}


//////////////////////////////////////////////////////////////////////////

HeroSt::Dying::Dying(void)
{
}


HeroSt::Dying::~Dying(void)
{
}

void HeroSt::Dying::enter( Hero* _actor )
{
	this->actor = _actor;
	Ator *a = actor->getStruct();

	ATOR_Impulsiona(a, actor->getJumpVelocity()*0.9);

	if(a->direcao == 0)
		ATOR_TrocaAnimacao(a, 9);
	else
		ATOR_TrocaAnimacao(a, 10);

	//ATOR_TocaEfeitoTela(a, 2, actor->getCurrentLevel()->getMap()->getId());

	a->temporizadores[0] = 120;
	a->estado.subestado = ESTADO_RODANDO;
	a->velocidade=0;
	a->vidas--;

	nextState = this;
}

State<Hero>* HeroSt::Dying::process( float secs )
{
	Ator *a = actor->getStruct();
	Evento ev;

	while(ATOR_ProximoEvento(a, &ev)){
		switch(ev.tipoEvento)
		{
		case EVT_COLIDIU_PAREDE:
			if(ev.subtipo == SUBEVT_COLIDIU_PAREDE_BAIXO)
				nextState = new Dead();
			break;
		case EVT_TEMPO:
			if(ev.subtipo==0)
				nextState = new Dead();
			break;
		}

		if(nextState != this)
			break;
	}

	return nextState;
}

void HeroSt::Dying::leave()
{

}