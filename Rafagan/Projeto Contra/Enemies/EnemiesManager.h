#pragma once

#include <forward_list>
#include <unordered_map>
#include <memory>
#include <string>

class Enemy;
class GameManager;

class EnemiesManager
{
private:
	GameManager* game;

 	std::forward_list<std::shared_ptr<Enemy>> enemies;
public:
	explicit EnemiesManager();
	virtual ~EnemiesManager(void);

	void init(GameManager* game);

 	void addEnemy(std::shared_ptr<Enemy> enemy);
 	void removeEnemy(std::shared_ptr<Enemy> enemy);
 	void processEnemies(float secs);
 	void drawEnemies() const;

	void clear();
};

