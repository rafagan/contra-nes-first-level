#pragma once

template<class EntityType>
class State
{
public:
	virtual ~State(){};

	virtual void enter(EntityType* entity) = 0;
	virtual State* process(float secs) = 0;
	virtual void leave() = 0;
};
