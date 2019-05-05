#include "statesRunner.h"

RunnerSt::Borning::Borning( void )
{

}

RunnerSt::Borning::~Borning( void )
{

}

void RunnerSt::Borning::enter( Enemy* actor )
{
	auto a = actor->getStruct();
	a->estado.subestado=ESTADO_RODANDO;
}

State<Enemy>* RunnerSt::Borning::process( float secs )
{
	return new Temporary();
}

void RunnerSt::Borning::leave()
{

}

//////////////////////////////////////////////////////////////////////////

RunnerSt::Dead::Dead( void )
{

}

RunnerSt::Dead::~Dead( void )
{

}

void RunnerSt::Dead::enter( Enemy* actor )
{

}

State<Enemy>* RunnerSt::Dead::process( float secs )
{
	return this;
}

void RunnerSt::Dead::leave()
{

}

//////////////////////////////////////////////////////////////////////////

RunnerSt::Finish::Finish( void )
{

}

RunnerSt::Finish::~Finish( void )
{

}

void RunnerSt::Finish::enter( Enemy* actor )
{

}

State<Enemy>* RunnerSt::Finish::process( float secs )
{
	return this;
}

void RunnerSt::Finish::leave()
{

}

//////////////////////////////////////////////////////////////////////////

RunnerSt::Temporary::Temporary( void )
{

}

RunnerSt::Temporary::~Temporary( void )
{

}

void RunnerSt::Temporary::enter( Enemy* actor )
{
	auto a = actor->getStruct();
	ATOR_TrocaEstado(a, ATOR_PROG_INI, false);
	a->estado.subestado=ESTADO_RODANDO;

	actor->setAnimation(0);
}

State<Enemy>* RunnerSt::Temporary::process( float secs )
{
	return this;
}

void RunnerSt::Temporary::leave()
{

}
