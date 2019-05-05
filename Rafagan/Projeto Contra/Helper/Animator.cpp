#include "Animator.h"

using namespace std;

#include <c2d2/chien2d2.h>


Animator::Animator(string name,int widthFrames,int heightFrames,float _timePerFrame,int totalFrames)
	: timePerFrame(_timePerFrame),wf(widthFrames),hf(heightFrames)
{
	image = C2D2_CarregaSpriteSet(name.c_str(),widthFrames,heightFrames);
	
	for(int i=0;i<totalFrames;i++)
		frames.push_back(i);
}

Animator::Animator(string name,int numFramesCol,int numFramesRow,int imageWidth, int imageHeight,float _timePerFrame)
    : timePerFrame(_timePerFrame),wf(imageWidth/numFramesCol),hf(imageHeight/numFramesRow)
{
	image = C2D2_CarregaSpriteSet(name.c_str(),wf,hf);

	int totalFrames = (numFramesRow * numFramesCol);

	for(int i=0;i<totalFrames;i++)
		frames.push_back(i);
}

Animator::Animator(string name,int widthFrames,int heightFrames,float _timePerFrame,vector<int> _frames)
	: timePerFrame(_timePerFrame),frames(_frames),wf(widthFrames),hf(heightFrames)
{
	image = C2D2_CarregaSpriteSet(name.c_str(),widthFrames,heightFrames);
}

Animator::Animator(string name,int numFramesCol,int numFramesRow,int imageWidth, int imageHeight,float _timePerFrame,vector<int> _frames)
	: timePerFrame(_timePerFrame),frames(_frames),wf(imageWidth/numFramesCol),hf(imageHeight/numFramesRow)
{
	image = C2D2_CarregaSpriteSet(name.c_str(),wf,hf);
}

const int *Animator::getWqHq()
{
	int v[] = {this->wf,this->hf};
	const int *ptr = v;

	return ptr;
}

int Animator::getNumFrames()
{
	return this->frames.size();
}

void Animator::setQuads(std::vector<int> _frames)
{
	frames = _frames;
	this->frame = 0;
}

void Animator::drawSprite(int _frame,int x,int y)
{
	C2D2_DesenhaSprite(image,_frame%frames.size(),x,y);
}

void Animator::drawSprite(int _frame,math::Matrix3 matrix,bool isHUD)
{
	isHUD ? Renderer::drawSpriteMatrixHud(matrix,image,_frame%frames.size())
		  : Renderer::drawSpriteMatrix(matrix,image,_frame%frames.size());
}

void Animator::drawSprite(int x,int y)
{
	C2D2_DesenhaSprite(image,frames.at(frame),x,y);
}

void Animator::drawSprite(math::Matrix3 matrix,bool isHUD)
{
	isHUD ? Renderer::drawSpriteMatrixHud(matrix,image,frames.at(frame))
		  : Renderer::drawSpriteMatrix(matrix,image,frames.at(frame));
}

void Animator::setTimePerFrame(float secs)
{
	this->timePerFrame = secs;
}

float Animator::getTimePerFrame()
{
	return this->timePerFrame;
}

void Animator::updateFrame(float secs)
{
	int isBackground = 0;

    if(!isBackground){
        this->timePass += secs;

        if(timePass >= timePerFrame){
            frame = (frame + 1) % frames.size();
            timePass -= timePerFrame;
        }
    }
}

Animator::~Animator(void)
{
	C2D2_RemoveSpriteSet(this->image);
	frames.clear();
}
