#pragma once

#include "../Interfaces/GameObject.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "../definitions.h"
#include "../Helper/Message.h"
#include "../Math/AABB.h"
#include "../Math/Vector2D.h"

#include "../Interfaces/Level.h"
#include "../Helper/StateMachine.h"

class GameManager;

class Character : public GameObject
{
protected:
	C2D2_Botao *keys;

	GameManager* game;

	Ator* myStruct;
	Level *currentLevel;
	unsigned int typeId;
	unsigned int myId;
	std::string typeName;
	AABB box;
	math::Vector2D frameSize;
	math::Vector2D positionBox,position,startPosition,relativePosition;
private:
	static unsigned int ID_GEN;
	static std::unordered_map<std::string,unsigned int> charactersTypeByName;
public:
	static void loadCharacter(
		std::string characterName,
		char* path,
		math::Vector2D frameSize,
		AABB boudingBox,
		std::vector<Animacao> animations,
		std::vector<char*> sfxNames
	);

	static void unloadCharacters();
	static unsigned int getCharacterTypeByName(std::string name);

	explicit Character();
	virtual ~Character();

	virtual UINT getTypeId();
	virtual std::string getTypeName();

	virtual math::Vector2D getFrameSize();
	virtual AABB getBoudingBox();
	virtual math::Vector2D getPosition();
	virtual math::Vector2D getRelativePosition();
	virtual Level* getCurrentLevel();
	virtual Ator* getStruct();
	virtual unsigned int getDatabaseId();
	
	virtual void setCurrentLevel(Level *level);
	virtual void setPosition(math::Vector2D position);
	virtual void setAnimation(unsigned int number);
	virtual void setDirection(unsigned int angle);
	virtual void setDatabaseId(unsigned int id);

	virtual void sendMessage(
		double delay,
		unsigned int receiver,std::string receiverGroupName,unsigned int receiverGroupId,
		std::shared_ptr<unsigned int> content
	);
	virtual void sendMessageToMe(
		double delay,
		std::shared_ptr<unsigned int> content
	);
	virtual void sendMessageToGroup( double delay, std::string group, std::shared_ptr<unsigned int> content );
	virtual void broadcastMessage(
		double delay,
		std::shared_ptr<unsigned int> content
	);

	virtual void handleMessageReceived(std::shared_ptr<Message<unsigned int>> message) = 0;
};