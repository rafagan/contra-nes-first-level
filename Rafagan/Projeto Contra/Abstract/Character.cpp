#include "Character.h"

#include "../GameManager.h"

using namespace std;
using namespace math;

unsigned int Character::ID_GEN = 1;
unordered_map<string,unsigned int> Character::charactersTypeByName;

void Character::loadCharacter(
						  string characterName,
						  char* path,
						  Vector2D frameSize,
						  AABB boudingBox,
						  vector<Animacao> animations,
						  vector<char*> sfxNames
						  )
{
	if(charactersTypeByName[characterName])
		return;

	//ID que representará meu personagem
	int id = ID_GEN++;
	charactersTypeByName[characterName] = id;

	//Path do Sprite (carregar sprite)
	char* spriteName = path;

	//Tamanho do quadro do frame (largura e altura)
	int size[] = {
		static_cast<int>(frameSize.x),
		static_cast<int>(frameSize.y)
	};

	//Posição x e y do bouding box no quadro
	//Largura e Altura do bounding box
	int aabb[] ={
		static_cast<int>(boudingBox.getPosX()),
		static_cast<int>(boudingBox.getPosY()),
		static_cast<int>(boudingBox.getWidth()),
		static_cast<int>(boudingBox.getHeight())
	};

	//Animações com:
	// 1- número de quadros
	// 2- tempo por quadro
	// 3- vetores de sequência de quadros
	Animacao anim[ATOR_MAX_ANIMACOES] = {};
	for(unsigned int i=0;i<animations.size();i++)
		anim[i] = animations[i];

	//Rotaciona automaticamente?
	bool autoRotate = false;

	//Audio
	unsigned int numNamesSfx = sfxNames.size();
	char **sn = 0;

	if(numNamesSfx > 0){
		sn = new char*[numNamesSfx];
		int i = 0;
		for(auto v : sfxNames){
			auto t = CA2_CarregaEfeito(v);
			if(t == 0)
				throw new logic_error("Erro ao carregar efeito " + string(v));
			sn[i++] = v;
		}
	} else{
		numNamesSfx = 0;
		sn = nullptr;
	}

	bool atorOk = ATOR_CarregaAtorEstatico(
		id,spriteName,size[0],size[1],
		aabb[0],aabb[1],aabb[2],aabb[3],
		anim,autoRotate,sn, numNamesSfx,
		[](Ator *a, unsigned int mapa){return true;}
	);

	if(!atorOk){
		string str = "Não foi possível carregar o ator: ";
		str+=path;
		throw exception(str.c_str());
	}

	delete sn;
}

Character::Character()
	:myId(0)
{
	keys = C2D2_PegaTeclas();
}

Character::~Character()
{
}

void Character::unloadCharacters()
{
	for(auto id : charactersTypeByName)
		ATOR_DescarregaAtor(id.second);
}


unsigned int Character::getCharacterTypeByName(std::string name)
{
	return Character::charactersTypeByName[name];
}

UINT Character::getTypeId()
{
	return this->typeId;
}

std::string Character::getTypeName()
{
	return this->typeName;
}

math::Vector2D Character::getFrameSize()
{
	return this->frameSize;
}

AABB Character::getBoudingBox()
{
	return this->box;
}

math::Vector2D Character::getPosition()
{
	return this->position;
}

math::Vector2D Character::getRelativePosition()
{
	return this->relativePosition;
}

Level* Character::getCurrentLevel()
{
	return this->currentLevel;
}

Ator* Character::getStruct()
{
	return this->myStruct;
}

unsigned int Character::getDatabaseId()
{
	return this->myId;
}

void Character::setPosition( math::Vector2D position )
{
	this->position = position;
	this->myStruct->x = position.x;
	this->myStruct->y = position.y;
}

void Character::setAnimation(unsigned int number)
{
	ATOR_TrocaAnimacao(myStruct,number);
}

void Character::setDirection( unsigned int angle )
{
	myStruct->olhandoPara = angle;
	myStruct->direcao = angle;
}

void Character::setDatabaseId( unsigned int id )
{
	this->myId = id;
}

void Character::sendMessageToMe( double delay, std::shared_ptr<unsigned int> content )
{
	//Envia Mensagem
	game->getMessageDispatcher()->dispatchMessage(
		delay, //Tempo

		this->myId,
		this->typeName,
		this->typeId,

		this->myId,
		this->typeName,
		Character::getCharacterTypeByName(typeName),
		content
	);
}

void Character::sendMessage( 
	double delay,
	unsigned int receiver,std::string receiverGroupName,unsigned int receiverGroupId, 
	std::shared_ptr<unsigned int> content )
{
	game->getMessageDispatcher()->dispatchMessage(
		delay,

		//Informações do remetente
		this->myId,
		this->typeName,
		this->typeId,

		//Informações do destinatário
		receiver,
		receiverGroupName,
		receiverGroupId,
		content
		);
}

void Character::sendMessageToGroup( double delay, std::string group, std::shared_ptr<unsigned int> content )
{
	game->getMessageDispatcher()->dispatchMessageToGroup(
		delay,this->myId,this->typeName,this->typeId,
		Character::getCharacterTypeByName(group),content
		);
}

void Character::broadcastMessage( double delay, std::shared_ptr<unsigned int> content )
{
	game->getMessageDispatcher()->broadcastMessage(delay,this->myId,this->typeName,this->typeId,content);
}

void Character::setCurrentLevel( Level *level )
{
	this->currentLevel = level;

	try{
		this->startPosition = currentLevel->getMap()->findStartLevelTilemap();
	} catch(logic_error &e){
		cerr << e.what() << endl;
		cerr << "Inicializando a posição do personagem com o valor <0,0>" << endl;
		this->startPosition = Vector2D(0,0);
	}
}