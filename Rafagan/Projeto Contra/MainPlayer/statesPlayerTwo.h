#pragma once

#include "../Interfaces/State.h"
#include "../Interfaces/Hero.h"



namespace HeroSt{
	class IntroDialogueP2 : public State<Hero>
	{
	private:
		Hero* actor;
	public:
		explicit IntroDialogueP2(void);
		virtual ~IntroDialogueP2(void);

		virtual void enter(Hero* actor);
		virtual State<Hero>* process(float secs);
		virtual void leave();
	};
}