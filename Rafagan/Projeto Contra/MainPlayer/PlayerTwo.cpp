#include "PlayerTwo.h"

#include "../definitions.h"
#include "../Helper/Camera2D.h"
#include "../MainPlayer/statesMainPlayer.h"
#include "../GameManager.h"
#include "statesMainPlayer.h"
#include "statesPlayerTwo.h"

using namespace std;
using namespace HeroSt;
using namespace math;

bool PlayerTwo::characterLoaded = false;

PlayerTwo::PlayerTwo(Level *level)
	: Player(level)
{
	this->typeName = "PlayerTwo";

	if(!characterLoaded){
		Player::loadHero(typeName,"Res/Sprites/SpritesetHero2.png",box,frameSize);
		characterLoaded = true;
	}

	this->typeId = Character::getCharacterTypeByName(this->typeName);

	this->startPosition.x -= 40;
}


PlayerTwo::~PlayerTwo(void)
{
}

void PlayerTwo::lateInit()
{
	unsigned int d[4] = {C2D2_I,C2D2_K,C2D2_J,C2D2_L};
	this->eventPoll = unique_ptr<PlayerEventPool>(
		new PlayerEventPool(this,d,
		C2D2_U,C2D2_O,C2D2_ENTER,C2D2_RSHIFT)
	);
	this->stateMachine.init(this,new Borning());
	this->firstSt = false;
	this->processInput = false;

	this->ballons = Loader::loadSprite("Res/Ballons/Player2.png",200,150);
	this->bIndex = -1;
}

void PlayerTwo::handleMessageReceived( std::shared_ptr<Message<unsigned int>> message )
{
	enum {D1 = 1,D2,D3,END_TALK};
	auto content = message->getMessageContent();

	switch(*content){
	case D1:
		this->bIndex = -1;
		break;
	case D2:
		this->bIndex = 0;
		this->sendMessageToGroup(3.0,"PlayerOne",shared_ptr<unsigned int>(new unsigned int(3)));
		this->sendMessageToMe(3.01,shared_ptr<unsigned int>(new unsigned int(1)));
		break;
	case D3:
		this->bIndex = 1;
		this->sendMessageToGroup(3.0,"PlayerOne",shared_ptr<unsigned int>(new unsigned int(4)));
		this->sendMessageToMe(3.01,shared_ptr<unsigned int>(new unsigned int(1)));
		break;
	case END_TALK:
		bIndex = -1;
		this->processInput = true;
		stateMachine.changeState(new Idle());
		break;
	}
}

void PlayerTwo::lateUpdate( float secs )
{
	
}

void PlayerTwo::lateDraw() const
{
	if(bIndex == 0)
		game->getRenderer()->drawSprite(ballons,bIndex,position - Vector2D(50,50),true);
	else if(bIndex == 1)
		game->getRenderer()->drawSprite(ballons,bIndex,position - Vector2D(20,70),true);
}

State<Hero>* PlayerTwo::getFirstStateLevelOne()
{
	static bool jumped = false;

	if(stateMachine.isInState(Idle()) && !jumped){
		jumped = true;
		return nullptr;
	}

	if(firstSt)
		return nullptr;
	firstSt = true;
	return new IntroDialogueP2();
}