#include "EnemiesManager.h"

#include "../Interfaces/Enemy.h"
#include "../GameManager.h"

#include <vector>

EnemiesManager::EnemiesManager()
{
}


EnemiesManager::~EnemiesManager(void)
{
}

void EnemiesManager::init(GameManager* game)
{
	this->game = game;
}

void EnemiesManager::addEnemy(std::shared_ptr<Enemy> enemy)
{
	enemies.push_front(enemy);

	//Temporário
	game->getDatabase()->createGroup(enemy->getTypeName(),enemy->getTypeId());
	enemy->init(game);
}

void EnemiesManager::removeEnemy(std::shared_ptr<Enemy> enemy)
{
	enemies.remove(enemy);
	enemy->destroy();
}

void EnemiesManager::processEnemies(float secs)
{
	std::vector<std::shared_ptr<Enemy>> enemiesToRemove;

	for(auto v : enemies){
		v->update(secs);
		if(v->getStruct()->estado.estado == ATOR_ENCERRADO)
			enemiesToRemove.push_back(v);
	}

	for(auto v : enemiesToRemove)
		this->removeEnemy(v);
}

void EnemiesManager::drawEnemies() const
{
	for(auto v : enemies)
		v->draw();
}

void EnemiesManager::clear()
{
	this->enemies.clear();
}
