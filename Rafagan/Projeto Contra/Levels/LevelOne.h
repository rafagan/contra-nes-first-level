#pragma once
#include "../Interfaces/Screen.h"
#include "../Interfaces/Level.h"
#include "../definitions.h"
#include "../Math/Vector2D.h"
#include "../Interfaces/Hero.h"

#include "../Enemies/EnemiesManager.h"

#include <memory>
#include <vector>

class Map;
class Character;
class GameManager;

class LevelOne : public Screen,public Level
{
private:
	bool wasDestroyed;
	C2D2_Botao *keys;
	GameManager* game;

	unsigned int numPlayers;

	unsigned int font;
	unsigned int levelMusic;

 	std::shared_ptr<Map> map;
 	std::vector<std::shared_ptr<Hero>> players;
 	EnemiesManager enemiesManager;

	void updateCamera();
public:
	explicit LevelOne(unsigned int numPlayers);
	virtual ~LevelOne(void);

	//Game Object
	virtual void init(GameManager* game);
	virtual void update( float secs );
	virtual void draw() const;
	virtual void destroy();

	//Screen
	virtual Screen* nextScreen();

	//Level
	virtual std::shared_ptr<MapUtil> getMap();
	virtual EnemiesManager* getEnemiesManager();
	virtual unsigned int getNumberOfPlayers();
};
