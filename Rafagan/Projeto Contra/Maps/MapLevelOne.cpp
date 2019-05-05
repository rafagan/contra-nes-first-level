#include "MapLevelOne.h"

#include <stdexcept>
#include "../GameManager.h"

using namespace std;
using namespace math;

MapLevelOne::MapLevelOne(void)
	:gravity(25),maxGravity(400)
{
}


MapLevelOne::~MapLevelOne(void)
{
	if (!wasDestroyed){
		this->destroy();
	}
}

void MapLevelOne::init(GameManager* game)
{
	keys = C2D2_PegaTeclas();
	this->game = game;

	id = C2D2M_CarregaMapaMappy("Res/Maps/TilemapLevelOne.fmp", "Res/Maps/TilesetContra.png");
	if(id == 0)
		throw exception("Erro de carregamento do Mapa");

	// Velocidade das camadas
	numLayers = 4;
	for(int i=0;i<numLayers;i++)
		C2D2M_VelocidadeCamadaMapa(id,i,1);

	C2D2M_CamadaMarcas(id, numLayers, 78);

	//Marcas especiais
 	unsigned int mark = C2D2M_PROG;
 	this->extendedBlocks["BOSS_CANNON"] = mark++;
 	this->extendedBlocks["360_CANNON"] = mark++;
	this->extendedBlocks["180_CANNON"] = mark++;
	this->extendedBlocks["SNIPER"] = mark++;

	// GRAVIDADE = A gravidade do mapa
	// MAXGRAVIDADE = é o máximo que o ator pode acelerar na queda, ou seja, simula a resistência do ar
	C2D2M_GravidadeMapa(id, gravity, maxGravity);

	posMap.set(0,0);
	C2D2M_PosicionaMapa(id,static_cast<int>(posMap.x),static_cast<int>(posMap.y));
}

void MapLevelOne::destroy()
{
	C2D2M_RemoveMapa(id);
	this->wasDestroyed = true;
}

void MapLevelOne::update(float secs)
{
	this->mapPositionDef();
}

void MapLevelOne::draw() const
{
	for (int i = 0; i < numLayers; i++)
		C2D2M_DesenhaCamadaMapa(id, i, 0, 100, SWIDTH, 496);
	//C2D2M_AnimaMapa(id);
}

void MapLevelOne::mapPositionDef()
{
	int dx,dy;
	C2D2M_DimensoesMapa(id,&dx,&dy);

 	posMap.x = static_cast<float>(game->getCamera()->getX());
 	posMap.y = 0.0f;

	//Código para evitar o bug do desaparecimento do cenário
 	if(posMap.x < 0) //No início do level
 		posMap.x = 0.0f;
 	else if(posMap.x > dx-SWIDTH) //No fim do level
 		posMap.x = static_cast<float>(dx-SWIDTH);

	C2D2M_PosicionaMapa(id,static_cast<int>(posMap.x),static_cast<int>(posMap.y));
}

Vector2D MapLevelOne::findStartLevelTilemap() const
{
	int x,y;
	if(!C2D2M_PrimeiroBlocoMarca(id, C2D2M_INICIO, &x, &y))
		throw logic_error("Não foi encontrada a posição de partida em findExtendedBlock da classe MapLevelOne");

	return Vector2D(static_cast<float>(x),static_cast<float>(y));
}

vector<Vector2D> MapLevelOne::findExtendedBlock(string blockType)
{
	if(!extendedBlocks[blockType]){
		string st = "Bloco de tipo " + blockType + " não encontrado em findExtendedBlock da classe MapLevelOne";
		throw logic_error(st.c_str());
	}

	int x,y;
	vector<Vector2D> vec;
	if(!C2D2M_PrimeiroBlocoMarca(this->id,extendedBlocks[blockType],&x,&y))
		throw logic_error("Não foi encontrada a posição do bloco no mapa MapLevelOne em findExtendedBlock");
	else {
		vec.push_back(Vector2D(static_cast<float>(x),static_cast<float>(y)));
		while(C2D2M_ProximoBlocoMarca(this->id, &x, &y))
			vec.push_back(Vector2D(static_cast<float>(x),static_cast<float>(y)));
	}

	return vec;
}

unsigned int MapLevelOne::getId() const
{
	return this->id;
}

bool MapLevelOne::testCollisionWithEndLevelTilemap(Ator *actor) const
{
	return ATOR_ColidiuBlocoCenario(actor, id, C2D2M_FIM);
}

bool MapLevelOne::testCollisionWithSolidTilemap(Ator *actor) const
{
	return ATOR_ColidiuBlocoCenario(actor, id, C2D2M_SOLIDO);
}

bool MapLevelOne::testCollisionWithTrapTilemap(Ator *actor) const
{
	return ATOR_ColidiuBlocoCenario(actor, id, C2D2M_MORTE);
}

bool MapLevelOne::testCollisionWithCheckpoint(Ator *actor) const
{
	return ATOR_ColidiuBlocoCenario(actor, id, C2D2M_CHECK);
}

bool MapLevelOne::testCollisionWithExtendedBlock(Ator *actor,std::string blockType)
{
	return ATOR_ColidiuBlocoCenario(actor, id, this->extendedBlocks[blockType]);
}
