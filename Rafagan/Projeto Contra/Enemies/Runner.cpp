#include "Runner.h"

#include "../GameManager.h"
#include "statesRunner.h"

using namespace std;
using namespace math;
using namespace RunnerSt;

bool Runner::characterLoaded = false;

void Runner::loadRunner(string name,std::string path,AABB box,Vector2D frameSize)
{
#pragma region ARRAY DAS ANIMAÇÕES
	// Ordem: número de quadros, tempo entre os quadros, vetor com a seqüência de quadros
	Animacao tmp[] ={
		//Andando para a direita
		6,7,{6,7,8,9,10,11},
		//Andando para a esquerda
		6,7,{0,1,2,3,4,5}
	};

	vector<Animacao> anim;
	for (int i = 0; i < 11; i++)
		anim.push_back(tmp[i]);
#pragma endregion

#pragma region ARRAY DOS SFX

	vector<char*> sounds;
	// 	sounds.push_back("Res/Audio/SFX/pula.wav");	

#pragma endregion

	Character::loadCharacter(
		name,
		const_cast<char*>(path.c_str()),
		frameSize,
		box,
		anim,
		sounds
	);
}

Runner::Runner(Level *level)
{
	this->typeName = "Runner";
	this->setCurrentLevel(level);
	this->box = AABB(0,0,42,65);
	this->frameSize = Vector2D(42.0f,65);

	if(!characterLoaded){
		Runner::loadRunner(typeName,"Res/Sprites/SpRunner.png",box,frameSize);
		characterLoaded = true;
	}

	this->typeId = Character::getCharacterTypeByName(this->typeName);
}

Runner::~Runner(void)
{
}

void Runner::init(GameManager* g)
{
	this->game = g;

	this->startPosition += Vector2D(400,0);
	myStruct = ATOR_CriaAtor(typeId, startPosition.x, startPosition.y, 0);
	if(myStruct == nullptr)
		throw exception("Erro ao tentar criar uma instância de Runner em init()");

	//Armazena no banco de dados
	this->setDatabaseId(
		game->getDatabase()->storeEntity(this->getTypeName(),this)
	);

	this->stateMachine.init(this,new Borning());

	this->ballons = Loader::loadSprite("Res/Ballons/Boss.png",200,150);
	this->bIndex = -1;
}

void Runner::destroy()
{
	delete myStruct;
}

void Runner::update( float dtSecs )
{
	ATOR_AplicaEstado(myStruct,currentLevel->getMap()->getId(),SWIDTH,SHEIGHT);
	ATOR_Atualiza(myStruct,currentLevel->getMap()->getId());

	this->stateMachine.update(dtSecs);

	//Atualiza os vetores de posição
	updatePosition();
	updateRelativePosition();

	//Atualiza o bouding box
	positionBox = (Vector2D(-10,75) + relativePosition);
	box.updatePosition(positionBox);
}

void Runner::draw() const
{
	ATOR_Desenha(myStruct,currentLevel->getMap()->getId(),0,100);
	
	if(bIndex > 0)
		game->getRenderer()->drawSprite(ballons,bIndex,position - Vector2D(50,50),true);
}

void Runner::handleMessageReceived( std::shared_ptr<Message<unsigned int>> message )
{
	auto content = *message->getMessageContent();

	if(content == 1)
		bIndex = -1;
	else if(content == 2){
		bIndex = 2;
		sendMessageToMe(3.0f,shared_ptr<unsigned int>(new unsigned int(1)));
	}
}

void Runner::updatePosition()
{
	position.x = static_cast<float>(myStruct->x);
	position.y = static_cast<float>(myStruct->y);
}

void Runner::updateRelativePosition()
{
	int dx,tmp;
	C2D2M_DimensoesMapa(currentLevel->getMap()->getId(),&dx,&tmp);
	if(position.x < dx-SWIDTHM)
		relativePosition = game->getRenderer()->toCamera(position);
	else
		relativePosition.set(position.x - dx + SWIDTH,position.y);
}