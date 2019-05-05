#pragma once

#include "../Interfaces/Hero.h"
#include "../Interfaces/Level.h"
#include "../definitions.h"
#include "../MainPlayer/PlayerEventPool.h"
#include "../Interfaces/State.h"

#include "../Helper/StateMachine.h"

class Player : public Hero
{
private:
	void updatePosition();
	void updateRelativePosition();
protected:
	virtual void lateInit() = 0;
	virtual void lateUpdate(float secs) = 0;
	virtual void lateDraw() const = 0;
protected:
	std::unique_ptr<PlayerEventPool> eventPoll;
	int jumpVelocity,velocity;
	bool processInput;

	State<Hero>* currentState;
	StateMachine<Hero,State<Hero>> stateMachine;
public:
	static void loadHero(std::string name,std::string path,AABB box,math::Vector2D frameSize);

	explicit Player(Level *startLevel);

	//GameObject
	virtual void init(GameManager* game);
	virtual void destroy();
	virtual void update( float secs );
	virtual void draw() const;

	//Exclusivos Hero
	virtual int getJumpVelocity();
	virtual int getWalkVelocity();
	virtual State<Hero>* getCurrentState();
	virtual void setWalkVelocity( int velocity );
	virtual StateMachine<Hero,State<Hero>>* getStateMachine();
};

