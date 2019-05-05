#pragma once
#include "../Abstract/Character.h"

template<class T>
class State;

class Hero : public Character
{
public:
	virtual ~Hero(){}

	virtual int getJumpVelocity() = 0;
	virtual int getWalkVelocity() = 0;
	virtual void setWalkVelocity(int velocity) = 0;
	virtual State<Hero>* getCurrentState() = 0;
	virtual StateMachine<Hero,State<Hero>>* getStateMachine() = 0;

	//Retornar nullptr irá gerar o comportamento default do estado
	virtual State<Hero>* getFirstStateLevelOne() = 0;
};

