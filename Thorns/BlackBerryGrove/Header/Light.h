// Hugh Smith 
// copyright 2015
// all rights reserved
#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include "camera.h"

namespace light {
enum LightType {Directional, Point, Spot};

class Light : public camera::Camera
{
	public:
		
	GXCOLOR	Color;

	LightType	Type;


	void initLight();
	void setDirection(float x, float y, float z);
	void setPosition(float x, float y, float z);
    void setColor(float r, float g, float b, float a);
};
}