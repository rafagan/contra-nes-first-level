#pragma once

#include "../Abstract/Player.h"

class PlayerTwo : public Player
{
private:
	static bool characterLoaded;
	bool firstSt;
	unsigned int ballons;
	int bIndex;
protected:
	virtual void lateInit();
	virtual void lateUpdate(float secs);
	virtual void lateDraw() const;
public:
	explicit PlayerTwo(Level *level);
	virtual ~PlayerTwo(void);

	virtual void handleMessageReceived( std::shared_ptr<Message<unsigned int>> message );

	virtual State<Hero>* getFirstStateLevelOne();
};

