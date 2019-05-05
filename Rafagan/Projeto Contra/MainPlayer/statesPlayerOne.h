#pragma once

#include "../Interfaces/State.h"
#include "../Interfaces/Hero.h"



namespace HeroSt{
	class IntroDialogueP1 : public State<Hero>
	{
	private:
		Hero* actor;
	public:
		explicit IntroDialogueP1(void);
		virtual ~IntroDialogueP1(void);

		virtual void enter(Hero* actor);
		virtual State<Hero>* process(float secs);
		virtual void leave();
	};
}