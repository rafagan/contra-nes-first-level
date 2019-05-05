#pragma once

#include "../Math/Vector2D.h"
#include "../definitions.h"
#include <vector>

class MapUtil
{
public:
	virtual ~MapUtil(){}

	virtual unsigned int getId() const = 0;

	//C2D2M_INICIO
	virtual math::Vector2D findStartLevelTilemap() const = 0;
	//C2D2M_PROG
	virtual std::vector<math::Vector2D> findExtendedBlock(std::string blockType) = 0;
	virtual bool testCollisionWithExtendedBlock(Ator *actor,std::string blockType) = 0;
	//C2D2M_FIM
	virtual bool testCollisionWithEndLevelTilemap(Ator *actor) const = 0;
	//C2D2M_SOLIDO
	virtual bool testCollisionWithSolidTilemap(Ator *actor) const = 0;
	//C2D2M_MORTE
	virtual bool testCollisionWithTrapTilemap(Ator *actor) const = 0;
	//C2D2M_CHECK
	virtual bool testCollisionWithCheckpoint(Ator *actor) const = 0;
};
