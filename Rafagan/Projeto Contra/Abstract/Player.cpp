#include "Player.h"


#include "../definitions.h"
#include "../Helper/Camera2D.h"
#include "../MainPlayer/statesMainPlayer.h"
#include "../GameManager.h"

using namespace std;
using namespace math;
using namespace HeroSt;

void Player::loadHero(string name,std::string path,AABB box,Vector2D frameSize)
{
#pragma region ARRAY DAS ANIMAÇÕES DO HERO
	// Ordem: número de quadros, tempo entre os quadros, vetor com a seqüência de quadros
	Animacao tmp[] ={
		// Ordem: número de quadros, tempo entre os quadros, vetor com a seqüência de quadros
		// HERO_PARADO direita: 0
		{1, 0, {0}},
		// HERO_PARADO esquerda: 1
		{1, 0, {8}},
		// HERO_ANDANDO direita: 2
		{5, 10, {11, 12, 13, 14, 10}},
		// HERO_ANDANDO esquerda: 3
		{5, 10, {18, 17, 16, 15, 19}},
		// HERO_PULANDO direita: 4
		{4, 5, {40, 41, 42, 43}},
		// HERO_PULANDO esquerda: 5	
		{4, 5, {47, 46, 45, 44}},
		// HERO_CAINDO direita: 6
		{4, 5, {40, 41, 42, 43}},
		// HERO_CAINDO esquerda: 7
		{4, 5, {47, 46, 45, 44}},
		// HERO_VITORIA: 8
		{5, 10, {11, 12, 13, 14, 10}},
		// HERO_MORRENDO direita: 9
		{3, 2, {48, 49, 50}},
		// HERO_MORRENDO esquerda 10
		{3, 2, {47, 46, 45}},
		// HERO_MORREU direita: 11
		{1, 1, {44}}, 
		// HERO_MORREU esquerda  12
		{1, 1, {43}},
	};

	vector<Animacao> animHero;
	for (int i = 0; i < 11; i++)
		animHero.push_back(tmp[i]);
#pragma endregion

#pragma region ARRAY DOS SFX DO HERO

	vector<char*> sounds;
	// 	sounds.push_back("Res/Audio/SFX/pula.wav");
	// 	sounds.push_back("Res/Audio/SFX/bate.wav");
	// 	sounds.push_back("Res/Audio/SFX/morreu.wav");	

#pragma endregion

	Character::loadCharacter(
		name,
		const_cast<char*>(path.c_str()),
		frameSize,
		box,
		animHero,
		sounds
	);
}


Player::Player(Level *level)
	:processInput(true)
{
	this->setCurrentLevel(level);
	this->box = AABB(21,25,16,50);
	this->frameSize = Vector2D(55,86);
}

void Player::init(GameManager* game)
{
	this->game = game;

	this->velocity = 2;
	this->jumpVelocity = 400;
	this->positionBox = startPosition;

	myStruct = ATOR_CriaAtor(typeId, startPosition.x, startPosition.y, 0);
	if(myStruct == nullptr)
		throw exception("Erro ao tentar criar uma instância de Player em init()");

	//Armazena o player no banco de dados
	this->setDatabaseId(
		game->getDatabase()->storeEntity(this->getTypeName(),this)
	);

	this->lateInit();
}

void Player::destroy()
{
	delete myStruct;
}

void Player::update( float secs )
{
	// a)Desloca o personagem
	// b)Aplica a gravidade
	// c)Testa colisão com o cenário
	// d)Anima o personagem
	// e)Envia eventos de acordo com estas operações
	ATOR_AplicaEstado(myStruct,currentLevel->getMap()->getId(),SWIDTH,SHEIGHT);

	// Chamada à função de atualização passada como ponteiro na criação do personagem
	ATOR_Atualiza(myStruct,currentLevel->getMap()->getId());

	//Processa os inputs
	if(processInput)
		this->eventPoll->poolEvents();

	//Atualiza os estados
	this->stateMachine.update(secs);

	//Atualiza os vetores de posição
	updatePosition();
	updateRelativePosition();

	//Atualiza o bouding box
 	positionBox = (Vector2D(-10,75) + relativePosition);
 	box.updatePosition(positionBox);

	this->lateUpdate(secs);
}

void Player::draw() const
{
	// Desenha o Ator. As coordenadas passadas representam a coordenada de início canto superior esquerdo do mapa na tela
	ATOR_Desenha(myStruct,currentLevel->getMap()->getId(),0,100);
	//box.draw();

	this->lateDraw();
}

int Player::getJumpVelocity()
{
	return this->jumpVelocity;
}

int Player::getWalkVelocity()
{
	return this->velocity;
}

State<Hero>* Player::getCurrentState()
{
	return this->currentState;
}

void Player::setWalkVelocity( int velocity )
{
	this->velocity = velocity;
	this->myStruct->velocidade = velocity;
}

StateMachine<Hero,State<Hero>>* Player::getStateMachine()
{
	return &this->stateMachine;
}

void Player::updatePosition()
{
	position.x = static_cast<float>(myStruct->x);
	position.y = static_cast<float>(myStruct->y);
}

void Player::updateRelativePosition()
{
	int dx,tmp;
	C2D2M_DimensoesMapa(currentLevel->getMap()->getId(),&dx,&tmp);
	if(position.x < dx-SWIDTHM)
		relativePosition = game->getRenderer()->toCamera(position);
	else
		relativePosition.set(position.x - dx + SWIDTH,position.y);
}
