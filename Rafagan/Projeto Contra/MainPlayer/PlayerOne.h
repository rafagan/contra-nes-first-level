#pragma once

#include "../Abstract/Player.h"

class PlayerOne : public Player
{
private:
	static bool characterLoaded;
	bool firstSt;
	unsigned int ballons;
	int bIndex;

	void singleDialogue(std::shared_ptr<unsigned int> content);
	void multiplayerDialogue(std::shared_ptr<unsigned int> content);
protected:
	virtual void lateInit();
	virtual void lateUpdate(float secs);
	virtual void lateDraw() const;
public:
	explicit PlayerOne(Level *level);
	virtual ~PlayerOne(void);

	virtual void handleMessageReceived( std::shared_ptr<Message<unsigned int>> message );

	virtual State<Hero>* getFirstStateLevelOne();
};

