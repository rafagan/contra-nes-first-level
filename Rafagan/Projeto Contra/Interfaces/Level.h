#pragma once

#include <memory>
#include "MapUtil.h"
//#include "../Enemies/EnemiesManager.h"

class EnemiesManager;

class Level
{
public:
	virtual ~Level(){};

	virtual std::shared_ptr<MapUtil> getMap() = 0;
	virtual EnemiesManager* getEnemiesManager() = 0;
	virtual unsigned int getNumberOfPlayers() = 0;
};
