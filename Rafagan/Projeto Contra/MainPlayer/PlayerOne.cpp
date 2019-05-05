#include "PlayerOne.h"

#include "../definitions.h"
#include "../Helper/Camera2D.h"
#include "../MainPlayer/statesMainPlayer.h"
#include "statesPlayerOne.h"
#include "../GameManager.h"

using namespace std;
using namespace math;
using namespace HeroSt;

bool PlayerOne::characterLoaded = false;

PlayerOne::PlayerOne(Level *level)
	: Player(level)
{
	this->typeName = "PlayerOne";

	if(!characterLoaded){
		Player::loadHero(typeName,"Res/Sprites/SpritesetHero1.png",box,frameSize);
		characterLoaded = true;
	}

	this->typeId = Character::getCharacterTypeByName(this->typeName);
}


PlayerOne::~PlayerOne(void)
{
}

void PlayerOne::lateInit()
{
	unsigned int d[4] = {C2D2_CIMA,C2D2_BAIXO,C2D2_ESQUERDA,C2D2_DIREITA};
	this->eventPoll = unique_ptr<PlayerEventPool>(
		new PlayerEventPool(this,d,
		C2D2_Z,C2D2_X,C2D2_ENTER,C2D2_RSHIFT)
	);
	this->stateMachine.init(this,new Borning());
	this->firstSt = false;
	this->processInput = false;

	this->ballons = Loader::loadSprite("Res/Ballons/Player1.png",200,150);
	this->bIndex = -1;
}

void PlayerOne::handleMessageReceived( std::shared_ptr<Message<unsigned int>> message )
{
	auto content = message->getMessageContent();
	
	if(currentLevel->getNumberOfPlayers() == 1)
		singleDialogue(content);
	else
		multiplayerDialogue(content);

	if(myStruct->direcao==0)
		this->setAnimation(0);
	else
		this->setAnimation(1);
}

void PlayerOne::singleDialogue(std::shared_ptr<unsigned int> content){
	enum {D1 = 1,D2,D3,END_TALK};

	switch(*content){
	case D1:
		this->bIndex = -1;
		break;
	case D2:
		this->bIndex = 0;
		break;
	case D3:
		bIndex = 1;
		break;
	case END_TALK:
		bIndex = -1;
		this->processInput = true;
		stateMachine.changeState(new Idle());
		this->sendMessageToGroup(3.0,"Runner",shared_ptr<unsigned int>(new unsigned int(2)));
		break;
	}
}

void PlayerOne::multiplayerDialogue(std::shared_ptr<unsigned int> content)
{
	enum {D1 = 1,D2,D3,END_TALK};

	switch(*content){
	case D1:
		this->bIndex = -1;
		break;
	case D2:
		this->setDirection(180);
		this->bIndex = 0;
		this->sendMessageToGroup(3.0,"PlayerTwo",shared_ptr<unsigned int>(new unsigned int(2)));
		this->sendMessageToMe(3.01,shared_ptr<unsigned int>(new unsigned int(1)));
		break;
	case D3:
		bIndex = 1;
		this->sendMessageToGroup(3.0,"PlayerTwo",shared_ptr<unsigned int>(new unsigned int(3)));
		this->sendMessageToMe(3.01,shared_ptr<unsigned int>(new unsigned int(1)));
		break;
	case END_TALK:
		this->setDirection(0);
		bIndex = -1;
		this->processInput = true;
		stateMachine.changeState(new Idle());
		this->sendMessageToGroup(0.0,"PlayerTwo",shared_ptr<unsigned int>(new unsigned int(4)));
		this->sendMessageToGroup(3.0,"Runner",shared_ptr<unsigned int>(new unsigned int(2)));
		break;
	}
}

void PlayerOne::lateUpdate( float secs )
{

}

void PlayerOne::lateDraw() const
{
	if(bIndex >= 0)
		game->getRenderer()->drawSprite(ballons,bIndex,position - Vector2D(20,60),true);
}

State<Hero>* PlayerOne::getFirstStateLevelOne()
{
	static bool jumped = false;

	if(stateMachine.isInState(Idle()) && !jumped){
		jumped = true;
		return nullptr;
	}

	if(firstSt)
		return nullptr;
	firstSt = true;
	return new IntroDialogueP1();
}