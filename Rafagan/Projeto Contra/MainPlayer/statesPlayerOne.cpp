#include "statesPlayerOne.h"
#include "statesMainPlayer.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////

HeroSt::IntroDialogueP1::IntroDialogueP1(void)
{
}


HeroSt::IntroDialogueP1::~IntroDialogueP1(void)
{
}

void HeroSt::IntroDialogueP1::enter(Hero* _actor)
{
	this->actor = _actor;

	if(actor->getCurrentLevel()->getNumberOfPlayers() == 1){
		//actor->sendMessageToMe(0.0,shared_ptr<unsigned int>(new unsigned int(4)));
  		actor->sendMessageToMe(0.0,shared_ptr<unsigned int>(new unsigned int(1)));
  		actor->sendMessageToMe(0.3,shared_ptr<unsigned int>(new unsigned int(2)));
  		actor->sendMessageToMe(3.0,shared_ptr<unsigned int>(new unsigned int(3)));
  		actor->sendMessageToMe(6.0,shared_ptr<unsigned int>(new unsigned int(4)));
	}
	else{
		//Prepara o terreno para o gatilho das falas
		actor->sendMessageToMe(0.0,shared_ptr<unsigned int>(new unsigned int(1)));
		actor->sendMessageToMe(0.3,shared_ptr<unsigned int>(new unsigned int(2)));
	}
}

State<Hero>* HeroSt::IntroDialogueP1::process( float secs )
{
	return this;
}

void HeroSt::IntroDialogueP1::leave()
{
	ATOR_LimpaEvento(actor->getStruct());
}

//////////////////////////////////////////////////////////////////////////