#pragma once

#include <stdexcept>
#include <memory>
#include <set>
#include "Message.h"
#include "../GameManager.h"

template <typename ContentType>
class MessageDispatcher
{
private:
	static MessageDispatcher* myInstance;

	GameManager* game;

	//a std::set is used as the container for the delayed messages
	//because of the benefit of automatic sorting and avoidance
	//of duplicates. Messages are sorted by their dispatch time.
	std::set<Message<ContentType>> priorityQueue;

	void sendMessage(const Message<ContentType>& message);

	MessageDispatcher();
	MessageDispatcher(const MessageDispatcher&);
	MessageDispatcher& operator=(const MessageDispatcher&);
public:
	static MessageDispatcher &getInstance();

	virtual ~MessageDispatcher(void);

	void setGameManager(GameManager* game);

	//send a message to another agent. Receiving agent is referenced by ID.
	void dispatchMessage(double delay,
		unsigned int sender,std::string senderGroupName,unsigned int senderGroupId,
		unsigned int receiver,std::string receiverGroupName,unsigned int receiverGroupId,
		std::shared_ptr<ContentType> content);

	//send a message to a group
	void dispatchMessageToGroup(double delay,unsigned int sender,
		std::string senderGroupName,unsigned int senderGroupId, unsigned int group, std::shared_ptr<ContentType> content);

	//send a message to all the agents
	void broadcastMessage(
		double delay,
		unsigned int sender,std::string senderGroupName,unsigned int senderGroupId,
		std::shared_ptr<ContentType> content
	);

	//send out any delayed messages.
	void dispatchDelayedMessages();
};

// Lógica para o Singleton
template <typename ContentType>
MessageDispatcher<ContentType> *MessageDispatcher<ContentType>::myInstance = nullptr;
template <typename ContentType>
MessageDispatcher<ContentType> &MessageDispatcher<ContentType>::getInstance()
{
	//Lazy load
	if (myInstance == nullptr)
		myInstance = new MessageDispatcher();
	else
		throw std::logic_error("O Singleton MessageDispatcher só pode ser acessado pelo GameManager");

	return *myInstance;
}

template <typename ContentType>
MessageDispatcher<ContentType>::MessageDispatcher(void)
{
}

template <typename ContentType>
MessageDispatcher<ContentType>::~MessageDispatcher(void)
{
}

template <typename ContentType>
void MessageDispatcher<ContentType>::setGameManager( GameManager* game )
{
	this->game = game;
}

template <typename ContentType>
void MessageDispatcher<ContentType>::dispatchMessage(
	double delay,
	unsigned int sender,std::string senderGroupName,unsigned int senderGroupId,
	unsigned int receiver,std::string receiverGroupName,unsigned int receiverGroupId,
	std::shared_ptr<ContentType> content)
{
	auto currentTime = game->getTimeManager()->getTimeSinceStartup();

	//Cria a mensagem
	auto msg = Message<ContentType>(
		currentTime + static_cast<float>(delay),
		sender,senderGroupName,senderGroupId,
		receiver,receiverGroupName,receiverGroupId,content
	);

	//Se não tiver delay, despacha imediatamente
	if(delay <= 0.0){
		sendMessage(msg);
		return;
	}

	//Caso contrário, insere na fila pra ser enviado depois
	//Observe que estou enviando o próprio objeto do smart pointer
	//Pois não quero perder a sobrecarga do operador <
	this->priorityQueue.insert(msg);
}

template <typename ContentType>
void MessageDispatcher<ContentType>::dispatchMessageToGroup( 
	double delay, unsigned int sender,std::string senderGroupName,unsigned int senderGroupId,
	unsigned int group, std::shared_ptr<ContentType> content )
{
	//Pegar os membros do grupo no banco de dados
	//Enviar a mensagem para cada um deles usando o dispatchMessage

	auto groupMembers = game->getDatabase()->findGroup(group);

	for(auto receiver : *groupMembers)
		this->dispatchMessage(
			delay,sender,senderGroupName,senderGroupId,
			receiver->getDatabaseId(),receiver->getTypeName(),receiver->getTypeId(),
			content
		);
}

template <typename ContentType>
void MessageDispatcher<ContentType>::broadcastMessage( 
	double delay, 
	unsigned int sender,std::string senderGroupName,unsigned int senderGroupId,
	std::shared_ptr<ContentType> content )
{
	auto entities = game->getDatabase()->getAllEntities();

	for(auto receiver : *entities)
		this->dispatchMessage(
			delay,sender,senderGroupName,senderGroupId,
			receiver->getDatabaseId(),receiver->getTypeName(),receiver->getTypeId(),
			content
		);
}

template <typename ContentType>
void MessageDispatcher<ContentType>::dispatchDelayedMessages()
{
	if(priorityQueue.empty())
		return;

	double currentTime = game->getTimeManager()->getTimeSinceStartup();
	const Message<ContentType>& msg = *priorityQueue.begin();
	float deliveryTime = msg.getDeliveryTime();

	while (!(priorityQueue.empty())     &&
		   (deliveryTime < currentTime) &&
		   (deliveryTime > 0)			&&
		   !(msg.isDelivered()))
	{
		const Message<ContentType>& msg = *priorityQueue.begin();
		this->sendMessage(msg);
		priorityQueue.erase(msg);
	}
}

template <typename ContentType>
void MessageDispatcher<ContentType>::sendMessage( const Message<ContentType>& message )
{
	auto database = game->getDatabase();
	auto receiver = database->findEntity(message.getReceiverGroupId(),message.getReceiver());

	//Aqui, envio a mensagem pro recebedor e seto a mensagem como enviada
	auto msg = std::shared_ptr<Message<ContentType>>(new Message<ContentType>(message));
	msg->switchToDelivered();
	receiver->handleMessageReceived(msg);
}