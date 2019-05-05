#pragma once

#include "../Interfaces/State.h"
#include "../Interfaces/Hero.h"

/************************************************************************/
/* Default Chien States                                                 */
/************************************************************************/

/************************************************************************/
/* Descrição: Esta classe representa o primeiro estado que um ator deve *
 * possuir, ou seja, representa o estado da Chien ATOR_NASCENDO     	*/
/************************************************************************/
namespace HeroSt{
	class Borning : public State<Hero>
	{
	private:
		Hero* actor;
	public:
		explicit Borning(void);
		virtual ~Borning(void);

		virtual void enter(Hero* actor);
		virtual State<Hero>* process(float secs);
		virtual void leave();
	};
}


//Classe que representa o estado ATOR_MORTO da Chien
namespace HeroSt{
	class Dead : public State<Hero>
	{
	private:
		Hero* actor;
		State<Hero>* nextState;
	public:
		explicit Dead(void);
		virtual ~Dead(void);

		virtual void enter( Hero* actor );
		virtual State<Hero>* process(float secs);
		virtual void leave();
	};
}


//Classe que representa o estado ATOR_ENCERRADO da Chien
namespace HeroSt{
	class Finish : public State<Hero>
	{
	private:
		Hero* actor;
	public:
		explicit Finish(void);
		virtual ~Finish(void);

		virtual void enter( Hero* actor );
		virtual State<Hero>* process(float secs);
		virtual void leave();
	};
}


/************************************************************************/
/* Outros estados                                                       */
/************************************************************************/


namespace HeroSt{
	class Starting : public State<Hero>
	{
	public:
		explicit Starting(void);
		virtual ~Starting(void);

		virtual void enter( Hero* actor );
		virtual State<Hero>* process(float secs);
		virtual void leave();
	};
}


namespace HeroSt{
	class Idle : public State<Hero>
	{
	private:
		Hero* actor;
		State<Hero>* nextState;

		State<Hero>* testEventType( Evento &ev, Ator * a);
	public:
		explicit Idle(void);
		virtual ~Idle(void);

		virtual void enter(Hero* actor);
		virtual State<Hero>* process(float secs);
		virtual void leave();
	};
}


namespace HeroSt{
	class Jumping : public State<Hero>
	{
	private:
		Hero* actor;
		State<Hero>* nextState;

		State<Hero>* testEventType( Evento &ev, Ator * a);
	public:
		explicit Jumping(void);
		virtual ~Jumping(void);

		virtual void enter( Hero* actor );
		virtual State<Hero>* process(float secs);
		virtual void leave();
	};
}


namespace HeroSt{
	class Falling : public State<Hero>
	{
	private:
		Hero* actor;
		State<Hero>* nextState;

		State<Hero>* testEventType( Evento &ev, Ator *a);
	public:
		explicit Falling(void);
		virtual ~Falling(void);

		virtual void enter( Hero* actor );
		virtual State<Hero>* process(float secs);
		virtual void leave();
	};
}


namespace HeroSt{
	class Walking : public State<Hero>
	{
	private:
		Hero* actor;
		State<Hero>* nextState;

		State<Hero>* testEventType( Evento &ev, Ator *a);
	public:
		explicit Walking(void);
		virtual ~Walking(void);

		virtual void enter( Hero* actor );
		virtual State<Hero>* process(float secs);
		virtual void leave();
	};
}


namespace HeroSt{
	class Winning : public State<Hero>
	{
	private:
		Hero* actor;
	public:
		explicit Winning(void);
		virtual ~Winning(void);

		virtual void enter( Hero* actor );
		virtual State<Hero>* process(float secs);
		virtual void leave();
	};
}


namespace HeroSt{
	class Dying : public State<Hero>
	{
	private:
		Hero* actor;
		State<Hero>* nextState;
	public:
		explicit Dying(void);
		virtual ~Dying(void);

		virtual void enter( Hero* actor );
		virtual State<Hero>* process(float secs);
		virtual void leave();
	};
}