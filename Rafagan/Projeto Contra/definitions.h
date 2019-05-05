#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#include <c2d2/chien2d2.h>
#include <c2d2/chien2d2primitivas.h>
#include <c2d2/ator.h>
#include <c2d2/chien2d2mapa.h>
#include <c2d2/chienaudio2.h>

#include "Helper/loader.h"
#include "Helper/sound.h"

#define SWIDTH 800
#define SHEIGHT 600
#define SWIDTHM 400
#define SHEIGHTM 300
#define GAME_NAME "Contra Redesign"

#define SCREEN_MODE C2D2_JANELA

//Função que converte um tipo void para o mais apropriado
template <typename T>
inline T* DereferenceToType(void* p)
{
	return (T*)(p);
}

#endif