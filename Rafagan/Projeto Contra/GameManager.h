#pragma once

#include <memory>

#include "Helper/Camera2D.h"
#include "Helper/Database.h"
#include "Abstract/Character.h"
#include "Helper/Renderer.h"
#include "Helper/MessageDispatcher.h"
#include "Helper/TimeManager.h"

#define MANAGER GameManager::getInstance();

class GameManager
{
private:
	static GameManager *myInstance;

	Camera2D* camera;
	std::shared_ptr<Database<Character*>> database;
	Renderer* renderer;
	MessageDispatcher<unsigned int>* messageDispatcher;
	TimeManager* timeManager;

	explicit GameManager(void);
	GameManager(const GameManager &);
	GameManager &operator= (const GameManager &);
public:
	static GameManager &getInstance();

	virtual ~GameManager(void);

	Camera2D* getCamera();
	Renderer* getRenderer();
	std::shared_ptr<Database<Character*>> getDatabase();
	MessageDispatcher<unsigned int>* getMessageDispatcher();
	TimeManager* getTimeManager();
};

