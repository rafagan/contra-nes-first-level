#pragma once
#include "../definitions.h"

namespace Sound
{
	//music
	inline int loadMusic(const char *file);
	inline int playMusic(int music, int number);
	inline void pauseMusic();

	//Effects
	inline int loadEffect(const char *file);
	inline int playEffect(int effect);
	inline void pauseEffects();
}

inline int Sound::loadMusic(const char *file)
{
	return CA2_CarregaMusica(file);
}

inline int Sound::loadEffect(const char *file)
{
	return CA2_CarregaEfeito(file);
}
inline int Sound::playMusic(int music, int  number)
{
	return CA2_TocaMusica(music, number);
}

inline int Sound::playEffect(int effect)
{
	return CA2_TocaEfeito(effect, NULL);
}

inline void Sound::pauseMusic()
{
	CA2_PausaMusica();
}

inline void Sound::pauseEffects()
{
	CA2_PausaEfeitos();
}