#pragma once

/*
 * Esta classe trabalha com tempo em SEGUNDOS
 */

class TimeManager
{
private:
	float startTime,before;
	float currentTime;
	float deltaTime;
	float gameSpeed;
	const unsigned int FPS;
public:
	explicit TimeManager(unsigned int fps);
	virtual ~TimeManager(void);

	void update();
	void setGameSpeed(float speed);
	float getGameSpeed() const;

	inline const unsigned int getFPS() const;
	inline float getDeltaTimeGoal() const;
	inline float getDeltaTime() const;
	inline float getTimeSinceStartup() const;

	inline float toSeconds(float milliseconds) const;
	inline float toMilliseconds(float seconds) const;
};

inline const unsigned int TimeManager::getFPS()const{return this->FPS;}
inline float TimeManager::getDeltaTimeGoal()const{return 1000.0f/FPS;}
inline float TimeManager::getDeltaTime()const{return deltaTime;}
inline float TimeManager::getTimeSinceStartup()const{return toSeconds(currentTime);}
inline float TimeManager::toMilliseconds(float seconds)const{return seconds*1000.0f;}
inline float TimeManager::toSeconds(float millisseconds)const{return millisseconds/1000.0f;}