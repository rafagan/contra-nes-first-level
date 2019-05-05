#include "GameLoop.h"
#include "Interfaces/Screen.h"
#include "Levels/LevelOne.h"

#include "definitions.h"
#include "GameManager.h"

#include <iostream>
#include <ctime>
#include <tchar.h>
#include "Screens/Credits.h"
#include "Screens/Menu.h"
#include "Screens/HowToPlay.h"

using namespace std;

GameLoop *GameLoop::myInstance = nullptr;
GameLoop &GameLoop::getInstance()
{
	//Lazy load
	if (myInstance == nullptr)
		myInstance = new GameLoop();

	return *myInstance;
}

GameLoop::GameLoop(void)
	:wasDestroyed(false)
{
}

GameLoop::~GameLoop(void)
{
	if (!wasDestroyed){
		this->destroy();
	}
}

void GameLoop::init()
{
	_tsetlocale(LC_ALL, _T("portuguese_brazil"));
	srand((unsigned int)time(nullptr));

	if (!C2D2_Inicia(SWIDTH, SHEIGHT, SCREEN_MODE, C2D2_DESENHO_OPENGL, GAME_NAME))
		throw exception("Erro ao iniciar Chien2D2.");

	if(C2D2P_Inicia())
		throw exception("Erro ao iniciar Primitivas.");

	if(!CA2_Inicia())
		throw exception("Erro ao iniciar Sistema de Som.");

	if(!C2D2M_Inicia())
		throw exception("Erro ao iniciar Sistema de Mapas.");

	if (!ATOR_Inicia())
		throw exception("Erro ao iniciar Sistema de Atores");

	//Posiciona a janela
	char w[50] = "SDL_VIDEO_WINDOW_POS=";
	strcat_s(w,"0,0");
	_putenv(w);

	this->gameManager = &MANAGER;
}

void GameLoop::run()
{
	C2D2_Botao *keys = C2D2_PegaTeclas();

	Screen *screen = new HowToPlay();
	screen->init(gameManager);

	do{
		gameManager->getTimeManager()->update();

		//Retorna nulo para sair do jogo
		//Retorna this para continuar na tela
		//Retorna nova tela para criar outra tela
		Screen* next = screen->nextScreen();

		if (next == nullptr) 
			break;

		if (next != screen) {
			screen->destroy();
			delete screen;

			screen = next;
			screen->init(gameManager);
		}

		//Atualizar
		gameManager->getMessageDispatcher()->dispatchDelayedMessages();
		screen->update(gameManager->getTimeManager()->getDeltaTime());

		//Desenhar
		gameManager->getRenderer()->clean();
		screen->draw();
		gameManager->getRenderer()->sync(gameManager->getTimeManager()->getFPS());

	} while(!keys[C2D2_ENCERRA].pressionado && !keys[C2D2_ESC].liberado);

	screen->destroy();
	delete screen;
}

void GameLoop::destroy()
{
	Character::unloadCharacters();

	CA2_Encerra();
	C2D2M_Encerra();
	ATOR_Encerra();
	C2D2_Encerra();
	this->wasDestroyed = true;
}
