#pragma once

#include "GameObject.h"
#include "MapUtil.h"

class Map : public GameObject,public MapUtil
{
public:
	virtual ~Map(){};
};
