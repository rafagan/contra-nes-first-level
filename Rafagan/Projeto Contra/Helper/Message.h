#pragma once

#include <iostream>

template <typename T>
class Message
{
private:
	unsigned int sender,receiver;
	std::string senderGroupName,receiverGroupName;
	unsigned int senderGroupId,receiverGroupId;
	bool delivered;
	std::shared_ptr<T> messageContent;
	float deliveryTime;
public:
	explicit Message();
	explicit Message(float deliveryTime,
		unsigned int sender,std::string senderGroupName,unsigned int senderGroupId,
		unsigned int receiver,std::string receiverGroupName,unsigned int receiverGroupId,
		std::shared_ptr<T> messageContent);
	virtual ~Message();

	//these messages will be stored in a priority queue. Therefore the >
	//operator needs to be overloaded so that the priority queue can sort the telegrams
	//by time priority.
	inline bool operator==(const Message<T>& other);
	inline bool operator<(const Message& other);
	
	unsigned int getSender() const;
	unsigned int getReceiver() const;

	float getDeliveryTime() const;
	unsigned int getSenderGroupId() const;
	unsigned int getReceiverGroupId() const;
	std::string getSenderGroupName() const;
	std::string getReceiverGroupName() const;

	void switchToDelivered();
	bool isDelivered() const;

	std::shared_ptr<T> getMessageContent() const;
};

template <typename T>
Message<T>::Message()
	:deliveryTime(0.0f)sender(0),receiver(0),delivered(false),messageContent(nullptr),
	senderGroupId(0),receiverGroupId(0)
{
}

template <typename T>
Message<T>::Message(float _deliveryTime,
					unsigned int _sender,std::string _senderGroupName,unsigned int _senderGroupId,
					unsigned int _receiver,std::string _receiverGroupName,unsigned int _receiverGroupId,
					std::shared_ptr<T> _messageContent)
	:deliveryTime(_deliveryTime),sender(_sender),receiver(_receiver),delivered(false),messageContent(_messageContent),
	senderGroupId(_senderGroupId),receiverGroupId(_receiverGroupId),
	senderGroupName(_senderGroupName),receiverGroupName(_receiverGroupName)
{
}

template <typename T>
inline bool Message<T>::operator==(const Message<T>& other)
{
	//Para fins de compatibilidade de tempo, consideramos iguais mensagens com uma diferença
	//menor do que 0.25 segundos
	const double smallestDelay = 0.25;
	return ( fabs(this->deliveryTime-other.deliveryTime) < smallestDelay) &&
		(this->sender == other.sender)        &&
		(this->receiver == other.receiver)    &&
		(*this->messageContent == *other.messageContent);
}

template <typename T>
inline bool Message<T>::operator<(const Message<T>& other)
{
	//Se a mensagem é a mesma, provavelmente devem ter sido criados em tempos diferentes
	if (*this == other)
		return false;
	return (this->deliveryTime < other.deliveryTime);
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Message<T>& msg)
{
	os << "time: " << msg.getDeliveryTime() << "  Sender: " << msg.getSender()
		<< "   Receiver: " << msg.getReceiver() << "   Msg: " << *msg.getMessageContent();

	return os;
}

template <typename T>
inline bool operator==(const Message<T>& t1, const Message<T>& t2)
{
	const double smallestDelay = 0.25;

	return (
		fabs(t1.getDeliveryTime()-t2.getDeliveryTime()) < smallestDelay) &&
		(t1.getSender() == t2.getSender())        &&
		(t1.getReceiver() == t2.getReceiver())    &&
		(t1.getMessageContent() == t2.getMessageContent()
	);
}

template <typename T>
inline bool operator<(const Message<T>& t1, const Message<T>& t2)
{
	if (t1 == t2)
		return false;
	return (t1.getDeliveryTime() < t2.getDeliveryTime());
}

template <typename T>
float Message<T>::getDeliveryTime() const
{
	return this->deliveryTime;
}

template <typename T>
std::shared_ptr<T> Message<T>::getMessageContent() const
{
	return this->messageContent;
}

template <typename T>
bool Message<T>::isDelivered() const
{
	return this->delivered;
}

template <typename T>
std::string Message<T>::getReceiverGroupName() const
{
	return this->receiverGroupName;
}

template <typename T>
std::string Message<T>::getSenderGroupName() const
{
	return this->senderGroupName;
}

template <typename T>
unsigned int Message<T>::getReceiverGroupId() const
{
	return this->receiverGroupId;
}

template <typename T>
unsigned int Message<T>::getSenderGroupId() const
{
	return this->senderGroupId;
}

template <typename T>
void Message<T>::switchToDelivered()
{
	this->delivered = true;
}

template <typename T>
unsigned int Message<T>::getReceiver() const
{
	return this->receiver;
}

template <typename T>
unsigned int Message<T>::getSender() const
{
	return this->sender;
}

template <typename T>
Message<T>::~Message()
{
	this->messageContent = nullptr;
}
