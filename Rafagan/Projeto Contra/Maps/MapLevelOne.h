#pragma once

#include "../Interfaces/Map.h"
#include "../definitions.h"
#include "../Math/Vector2D.h"
#include "../Interfaces/GameObject.h"
#include "../Abstract/Character.h"

#include <vector>
#include <unordered_map>

class MapLevelOne : public Map
{
private:
	bool wasDestroyed;
	C2D2_Botao *keys;
	GameManager* game;

	const unsigned int gravity,maxGravity;
	int numLayers;

	unsigned int id;
	math::Vector2D posMap;
	std::unordered_map<std::string,unsigned int> extendedBlocks;

	void mapPositionDef();
public:
	explicit MapLevelOne(void);
	virtual ~MapLevelOne(void);

	//GameObject
	virtual void init(GameManager* game);
	virtual void destroy();
	virtual void update(float secs);
	virtual void draw() const;

	//MapUtil
	virtual math::Vector2D findStartLevelTilemap() const;
	virtual std::vector<math::Vector2D> findExtendedBlock(std::string blockType);

	virtual unsigned int getId() const;

	virtual bool testCollisionWithEndLevelTilemap(Ator *actor) const;
	virtual bool testCollisionWithSolidTilemap(Ator *actor) const;
	virtual bool testCollisionWithTrapTilemap(Ator *actor) const;
	virtual bool testCollisionWithCheckpoint(Ator *actor) const;
	virtual bool testCollisionWithExtendedBlock(Ator *actor,std::string blockType);
};

