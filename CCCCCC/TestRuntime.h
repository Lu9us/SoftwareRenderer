#pragma once
#include "Window.h"

class TestRT : public Runtime
{
public:
	Window * instance;
	TestRT(Window *inst)
	{
		instance = inst;
	}
	void drawToClear(uint8* color)
	{


		for (int i = 0; i < instance->getBufferHeight() - 1; i++)
		{

			for (int j = 0; j < instance->getBufferWidth() - 1; j++)
			{

				instance->setPixel(j, i, color);
			}

		}
	}
	void RenderPattern(int frameCounter)
	{
		for (int i = 0; i < instance->getBufferHeight(); i++)
		{

			for (int j = 0; j < instance->getBufferWidth(); j++)
			{

				uint8  color[3];


				color[0] = (uint8)j + (uint8)frameCounter;

				color[1] = (uint8)i;

				color[2] = 0;

				instance->setPixel(j, i, color);

			}

		}

	}
	void  Draw(int frameCounter)
	{
		uint8  color[3];
		color[0] = 0;

		color[1] = 255;

		color[2] = 0;
		drawToClear(color);
		RenderPattern(frameCounter);
	}
	void Update(int frameCounter)
	{
	}


};