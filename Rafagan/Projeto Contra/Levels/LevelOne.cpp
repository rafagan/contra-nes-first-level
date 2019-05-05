#include "LevelOne.h"

#include "../definitions.h"
#include "../Interfaces/Map.h"
#include "../Abstract/Character.h"
#include "../Maps/MapLevelOne.h"
#include "../MainPlayer/PlayerOne.h"
#include "../MainPlayer/PlayerTwo.h"
//#include "../Enemies/EnemiesManager.h"

#include <iostream>
#include <stdexcept>
#include "../GameManager.h"
#include "../Enemies/Runner.h"

using namespace std;
using namespace math;

LevelOne::LevelOne(unsigned int _numPlayers)
	:numPlayers(_numPlayers)
{
}

LevelOne::~LevelOne(void)
{
	if (!wasDestroyed){
		this->destroy();
	}
}

void LevelOne::init(GameManager* game)
{
	keys = C2D2_PegaTeclas();
	this->game = game;
	game->getCamera()->setPos(0.0f,0.0f);
	enemiesManager.init(game);

  	levelMusic = CA2_CarregaMusica("Res/Audio/Mus/Level.ogg");
	if(levelMusic == 0)
		throw exception("Erro de carregamento em LevelOne");

	map = shared_ptr<Map>(new MapLevelOne());
	map->init(game);

	players.push_back(shared_ptr<Hero>(new PlayerOne(this)));
	if(numPlayers > 1)
		players.push_back(shared_ptr<Hero>(new PlayerTwo(this)));

	//Armazena no banco os heróis
	auto db = game->getDatabase();
	for(auto p : players){
		db->createGroup(p->getTypeName(),p->getTypeId());
		p->init(game);
	}

	//Grupo dos inimigos
	//db->createGroup("Enemies",3000);

	//Grupo dos inimigos corredores
	enemiesManager.addEnemy(shared_ptr<Enemy>(new Runner(this)));

// 
// 	auto rightBall = map->findExtendedBlock("MARCA_BOLA_DIREITA");
// 	auto leftBall = map->findExtendedBlock("MARCA_BOLA_ESQUERDA");
// 
// 	for(auto pos : rightBall)
// 		enemiesManager.addEnemyBall(shared_ptr<Ball>(new Ball(this,darkphoenix,pos,0)));
// 	for(auto pos : leftBall)
// 		enemiesManager.addEnemyBall(shared_ptr<Ball>(new Ball(this,darkphoenix,pos,180)));
// 
	C2D2_TrocaCorLimpezaTela(0, 0, 0);
	CA2_TocaMusica(levelMusic, -1);
	game->getCamera()->setPos(0,0);
}

void LevelOne::destroy()
{
	map->destroy();
	enemiesManager.clear();
	for(auto p : players){
		game->getDatabase()->removeGroup(p->getTypeName());
		p->destroy();
	}

	this->wasDestroyed = true;
}

void LevelOne::update(float secs) 
{
	updateCamera();

 	map->update(secs);
	enemiesManager.processEnemies(secs);
	for(auto p : players)
		p->update(secs);
}

void LevelOne::draw() const
{
 	map->draw();
	enemiesManager.drawEnemies();
	for(auto p : players)
		p->draw();
}

Screen* LevelOne::nextScreen()
{
	return this;
}

shared_ptr<MapUtil> LevelOne::getMap()
{
	return this->map;
}

EnemiesManager* LevelOne::getEnemiesManager()
{
	return &this->enemiesManager;
}

unsigned int LevelOne::getNumberOfPlayers()
{
	return this->numPlayers;
}

//A câmera deve sempre estar centralizada na cena, mas nunca se mover mais que os players
void LevelOne::updateCamera()
{
	auto camera = game->getCamera();
	auto p1x = players[0]->getPosition().x;
	int dx,tmp;
	C2D2M_DimensoesMapa(map->getId(),&dx,&tmp);
	auto cameraLimit = dx + SWIDTH;

	if(p1x >= SWIDTHM)
		camera->setPos(p1x - SWIDTHM,0);

	if(p1x < SWIDTHM)
		camera->setPos(0,0);
 	else if(p1x > cameraLimit)
 		camera->setPos(cameraLimit,0);
}
