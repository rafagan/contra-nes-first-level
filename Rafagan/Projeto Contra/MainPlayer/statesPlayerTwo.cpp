#include "statesPlayerTwo.h"
#include "statesMainPlayer.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////

HeroSt::IntroDialogueP2::IntroDialogueP2(void)
{
}


HeroSt::IntroDialogueP2::~IntroDialogueP2(void)
{
}

void HeroSt::IntroDialogueP2::enter(Hero* _actor)
{
	this->actor = _actor;

	//Prepara o terreno
	actor->sendMessageToMe(0.0,shared_ptr<unsigned int>(new unsigned int(1)));
}

State<Hero>* HeroSt::IntroDialogueP2::process( float secs )
{
	return this;
}

void HeroSt::IntroDialogueP2::leave()
{
	ATOR_LimpaEvento(actor->getStruct());
}

//////////////////////////////////////////////////////////////////////////