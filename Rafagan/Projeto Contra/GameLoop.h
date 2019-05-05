#pragma once

//Singleton!!!
#define GAME_LOOP GameLoop::getInstance()

class GameManager;

class GameLoop
{
private:
	bool wasDestroyed;
	GameManager* gameManager;

	explicit GameLoop();
	static GameLoop *myInstance;
	GameLoop(const GameLoop &);
	GameLoop &operator = (const GameLoop &);

public:
	//Singleton
	static GameLoop &getInstance();

	void init();
	void run();
	void destroy();

	virtual ~GameLoop();
};

