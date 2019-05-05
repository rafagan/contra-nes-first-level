#pragma once

/*
 *	Descrição: Uma máquina de estados simples, com suporte a estados globais e retorno
 *	ao estado ligeiramente anterior.
 *	Não possui suporte a enfileiramento ou empilhamento de estados, e nem suporte a goals
 */

template<class EntityType,class StateType>
class StateMachine
{
public:
	explicit StateMachine();
	virtual ~StateMachine();

	void init(
		EntityType* _owner,
		StateType* _currentState,
		StateType* _globalState = nullptr);
	void update(float secs);

	bool isInState(const StateType& state) const;
	EntityType* getOwner() const;
	StateType* getGlobalState() const;
	StateType* getCurrentState() const;
	StateType* getPreviousState() const;
	std::string getCurrentStateName() const;

	void changeState(StateType* nextState);
	void changeStateToPrevious();
private:
	EntityType* owner;
	StateType* globalState;
	StateType* currentState;
	StateType* previousState;
};

template<class EntityType,class StateType>
StateMachine<EntityType, StateType>::StateMachine()
{
}

template<class EntityType,class StateType>
StateMachine<EntityType, StateType>::~StateMachine()
{
	if(globalState) delete globalState;
	if(previousState) delete previousState;
	delete this->currentState;
}


template<class EntityType,class StateType>
void StateMachine<EntityType, StateType>::init( 
	EntityType* _owner, 
	StateType* _currentState, 
	StateType* _globalState)
{
	this->owner = _owner;
	this->currentState = _currentState;
	this->globalState = _globalState;
	this->previousState = nullptr;

 	if(this->owner == nullptr)
 		throw logic_error("owner nulo em StateMachine::init()");
  	if(this->currentState == nullptr){
 		std::string tmp = 
 			"currentState nulo em StateMachine::init() para entidade de nome ";
 		tmp += owner->getTypeName();
  		throw logic_error(tmp.c_str());
 	}
 
 	if(this->globalState)
 		globalState->enter(owner);
 	this->currentState->enter(owner);
}


template<class EntityType,class StateType>
void StateMachine<EntityType, StateType>::update(float secs)
{
 	if(this->globalState)
 		globalState->process(secs);
 
 	StateType* next = currentState->process(secs);
 	if(currentState != next)
 		this->changeState(next);
}


template<class EntityType,class StateType>
bool StateMachine<EntityType, StateType>::isInState(const StateType& state) const
{
	//Compara os tipos (tipo um typeof do C#)
	//Outra abordagem seria return dynamic_cast<NomeDaClasse*>(varEstado) - Funciona como um instanceof do JavaScript
	return typeid(*currentState) == typeid(state);
}

template<class EntityType,class StateType>
EntityType* StateMachine<EntityType, StateType>::getOwner() const
{
	return this->owner;
}

template<class EntityType,class StateType>
StateType* StateMachine<EntityType, StateType>::getGlobalState() const
{
	return this->globalState;
}

template<class EntityType,class StateType>
StateType* StateMachine<EntityType, StateType>::getCurrentState() const
{
	return this->currentState;
}

template<class EntityType,class StateType>
StateType* StateMachine<EntityType, StateType>::getPreviousState() const
{
	return this->previousState;
}

template<class EntityType,class StateType>
void StateMachine<EntityType, StateType>::changeState(StateType* nextState)
{
	currentState->leave();
	if(previousState)
		delete previousState;
	previousState = currentState;
	currentState = nextState;

	if(nextState == nullptr) 
		throw exception(
			std::string("Estado nulo em StateMachine::changeState() para entidade de nome " + 
			owner->getTypeName()).c_str()
		);

	currentState->enter(owner);
}

template<class EntityType,class StateType>
void StateMachine<EntityType, StateType>::changeStateToPrevious()
{
	currentState->leave();
	swap(currentState,previousState);
	currentState->enter();
}

template<class EntityType,class StateType>
std::string StateMachine<EntityType, StateType>::getCurrentStateName() const
{
	std::string s(typeid(*currentState).name());

	//remove the 'class ' part from the front of the string
	if (s.size() > 5)
		s.erase(0, 6);

	return s;
}