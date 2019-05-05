#pragma once

#include "../Interfaces/Enemy.h"
#include "../Interfaces/State.h"

class Runner : public Enemy
{
private:
	static bool characterLoaded;

	State<Enemy>* currentState;
	StateMachine<Enemy,State<Enemy>> stateMachine;

	void updatePosition();
	void updateRelativePosition();

	unsigned int ballons;
	int bIndex;
public:
	static void loadRunner(std::string name,std::string path,AABB box,math::Vector2D frameSize);

	explicit Runner(Level *level);
	virtual ~Runner(void);

	virtual void init( GameManager* );
	virtual void destroy();
	virtual void update( float dtSecs );
	virtual void draw() const;

	virtual void handleMessageReceived( std::shared_ptr<Message<unsigned int>> message );
};

