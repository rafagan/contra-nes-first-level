#include "GameManager.h"

using namespace std;

GameManager* GameManager::myInstance = nullptr;
GameManager& GameManager::getInstance()
{
	//Lazy load
	if (myInstance == nullptr)
		myInstance = new GameManager();
	else
		throw std::logic_error("O Singleton GameManager só pode ser acessado pelo GameLoop");

	return *myInstance;
}

GameManager::GameManager(void)
{
	camera = &CAMERA;
	database = shared_ptr<Database<Character*>>(new Database<Character*>());
	renderer = &RENDERER;
	renderer->setCamera(camera);
	messageDispatcher = &MessageDispatcher<unsigned int>::getInstance();
	messageDispatcher->setGameManager(this);
	timeManager = new TimeManager(C2D2_FPS_PADRAO);
}

GameManager::~GameManager(void)
{
	delete camera;
	delete renderer;
	delete timeManager;
}

Camera2D* GameManager::getCamera()
{
	return this->camera;
}

Renderer* GameManager::getRenderer()
{
	return this->renderer;
}

shared_ptr<Database<Character*>> GameManager::getDatabase()
{
	return this->database;
}
MessageDispatcher<unsigned int>* GameManager::getMessageDispatcher()
{
	return this->messageDispatcher;
}

TimeManager* GameManager::getTimeManager()
{
	return this->timeManager;
}