#pragma once

#include "GameObject.h"

class Screen : public GameObject
{
	public:
		virtual ~Screen() {};
		virtual Screen* nextScreen() = 0;
};