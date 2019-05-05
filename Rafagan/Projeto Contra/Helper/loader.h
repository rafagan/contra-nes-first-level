#pragma once


namespace Loader
{
	inline unsigned int loadSprite(const char* fileName,int w,int h);
	inline unsigned int loadFont(const char* fileName, int dimension);
}

inline unsigned int Loader::loadSprite(const char* fileName,int w,int h)
{
	return C2D2_CarregaSpriteSet(fileName,w,h);
}

inline unsigned int Loader::loadFont( const char* fileName, int dimension )
{
	return C2D2_CarregaFonte(fileName, dimension);
}