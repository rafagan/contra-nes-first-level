#pragma once

#include "../Interfaces/State.h"
#include "../Interfaces/Enemy.h"

namespace RunnerSt {
	class Borning : public State<Enemy>
	{
	private:
		Enemy* actor;
	public:
		explicit Borning(void);
		virtual ~Borning(void);

		virtual void enter(Enemy* actor);
		virtual State<Enemy>* process(float secs);
		virtual void leave();
	};
}

namespace RunnerSt {
	class Dead : public State<Enemy>
	{
	private:
		Enemy* actor;
	public:
		explicit Dead(void);
		virtual ~Dead(void);

		virtual void enter(Enemy* actor);
		virtual State<Enemy>* process(float secs);
		virtual void leave();
	};
}

namespace RunnerSt{
	class Finish : public State<Enemy>
	{
	private:
		Enemy* actor;
	public:
		explicit Finish(void);
		virtual ~Finish(void);

		virtual void enter( Enemy* actor );
		virtual State<Enemy>* process(float secs);
		virtual void leave();
	};
}

namespace RunnerSt{
	class Temporary : public State<Enemy>
	{
	private:
		Enemy* actor;
	public:
		explicit Temporary(void);
		virtual ~Temporary(void);

		virtual void enter( Enemy* actor );
		virtual State<Enemy>* process(float secs);
		virtual void leave();
	};
}