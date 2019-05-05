#pragma once

#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <string>

#pragma region GROUP CLASS HEADER
template<class EntityType>
class Group
{
private:
	static unsigned int ID_ENTITY_GEN;

	unsigned int id;
	std::string name;
	std::unordered_map<unsigned int,EntityType> membersById;
	std::unordered_set<EntityType> entities;
public:
	explicit Group(void);
	virtual ~Group(void);

	void setGroupInformations(unsigned int id,std::string name);
	unsigned int getId();
	std::string getName();

	std::shared_ptr<const std::vector<EntityType>> getMembers();
	EntityType getMember(unsigned int id);

	unsigned int storeMember(EntityType entity);
	void removeMember(unsigned int id);
};
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region DATABASE CLASS HEADER
template<class EntityType>
class Database
{
private:
	std::unordered_map<std::string,Group<EntityType>*> groupsByName;
	std::unordered_map<unsigned int,Group<EntityType>*> groupsById;
public:
	explicit Database(void);
	virtual ~Database(void);

	void createGroup(std::string groupName,unsigned int groupId);

	//Retorna o ID da entidade!
	unsigned int storeEntity(std::string groupName,EntityType entity);
	unsigned int storeEntity(unsigned int groupId,EntityType entity);

	void removeGroup(std::string groupName);
	void removeGroup(unsigned int groupId);

	void removeEntity(std::string groupName,unsigned int entityId);
	void removeEntity(unsigned int groupId,unsigned int entityId);

	std::shared_ptr<const std::vector<EntityType>>  findGroup(std::string groupName);
	std::shared_ptr<const std::vector<EntityType>>  findGroup(unsigned int groupId);

	EntityType findEntity(std::string groupName,unsigned int entityId);
	EntityType findEntity(unsigned int groupId,unsigned int entityId);

	std::shared_ptr<const std::vector<EntityType>> getAllEntities();
};
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region GROUP CLASS IMPLEMENTATION
template<class EntityType>
unsigned int Group<EntityType>::ID_ENTITY_GEN = 1;

template<class EntityType>
Group<EntityType>::Group()
{
}

template<class EntityType>
Group<EntityType>::~Group( void )
{
	this->membersById.clear();
}


template<class EntityType>
void Group<EntityType>::setGroupInformations( unsigned int id,std::string name )
{
	this->id = id;
	this->name = name;
}

template<class EntityType>
unsigned int Group<EntityType>::getId()
{
	return id;
}

template<class EntityType>
std::string Group<EntityType>::getName()
{
	return name;
}

template<class EntityType>
std::shared_ptr<const std::vector<EntityType>> Group<EntityType>::getMembers()
{
	std::shared_ptr<std::vector<EntityType>> 
		tmp = std::shared_ptr<std::vector<EntityType>>(new std::vector<EntityType>());

	for(auto v : membersById)
		tmp->push_back(v.second);

	return tmp;
}

template<class EntityType>
EntityType Group<EntityType>::getMember( unsigned int id )
{
	return this->membersById[id];
}

template<class EntityType>
unsigned int Group<EntityType>::storeMember( EntityType entity )
{
	if(entities.find(entity) != entities.end())
		return 0;

	this->membersById[ID_ENTITY_GEN] = entity;
	entities.insert(entity);
	return ID_ENTITY_GEN++;
}

template<class EntityType>
void Group<EntityType>::removeMember( unsigned int id )
{
	membersById.erase(id);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region DATABASE CLASS IMPLEMENTATION

template<class EntityType>
Database<EntityType>::Database( void )
{
}

template<class EntityType>
Database<EntityType>::~Database( void )
{
	groupsByName.clear();
	groupsById.clear();
}

template<class EntityType>
void Database<EntityType>::createGroup( std::string groupName,unsigned int groupId )
{
	if(groupsByName[groupName])
		return;

	this->groupsByName[groupName] = new Group<EntityType>();
	this->groupsByName[groupName]->setGroupInformations(groupId,groupName);
	this->groupsById[groupId] = this->groupsByName[groupName];
}

template<class EntityType>
unsigned int  Database<EntityType>::storeEntity(std::string groupName,EntityType entity)
{
	return this->groupsByName[groupName]->storeMember(entity);
}

template<class EntityType>
unsigned int Database<EntityType>::storeEntity(unsigned int groupId,EntityType entity)
{
	this->groupsById[groupId]->storeMember(entity);
}

template<class EntityType>
void Database<EntityType>::removeGroup(std::string groupName)
{
	auto tmp = groupsByName[groupName];
	groupsByName.erase(tmp->getName());
	groupsById.erase(tmp->getId());
	delete tmp;
}

template<class EntityType>
void Database<EntityType>::removeGroup(unsigned int groupId)
{
	auto tmp = groupsById[groupId];
	groupsByName.erase(tmp.getName());
	groupsById.erase(tmp.getId());
	delete tmp;
}

template<class EntityType>
void Database<EntityType>::removeEntity(std::string groupName,unsigned int entityId)
{
	this->groupsByName[groupName]->removeMember(entityId);
}

template<class EntityType>
void Database<EntityType>::removeEntity(unsigned int groupId,unsigned int entityId)
{
	this->groupsById[groupId]->removeMember(entityId);
}

template<class EntityType>
std::shared_ptr<const std::vector<EntityType>> 
	Database<EntityType>::findGroup(std::string groupName)
{
	return this->groupsByName[groupName]->getMembers();
}

template<class EntityType>
std::shared_ptr<const std::vector<EntityType>> 
	Database<EntityType>::findGroup(unsigned int groupId)
{
	return this->groupsById[groupId]->getMembers();
}

template<class EntityType>
EntityType Database<EntityType>::findEntity(std::string groupName,unsigned int entityId)
{
	return this->groupsByName[groupName]->getMember(entityId);
}

template<class EntityType>
EntityType Database<EntityType>::findEntity(unsigned int groupId,unsigned int entityId)
{
	return this->groupsById[groupId]->getMember(entityId);
}

template<class EntityType>
std::shared_ptr<const std::vector<EntityType>> Database<EntityType>::getAllEntities()
{
	std::shared_ptr<std::vector<EntityType>> 
		vec = std::shared_ptr<std::vector<EntityType>>(new std::vector<EntityType>());

	for(auto v : groupsById){ //Grupo de entidades
		auto tmp = v.second->getMembers();
		for(auto x : *tmp) //Entidades
			vec->push_back(x);
	}

	return vec;
}
#pragma endregion