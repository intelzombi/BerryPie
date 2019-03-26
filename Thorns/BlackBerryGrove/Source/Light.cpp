// Hugh Smith 
// copyright 2015
// all rights reserved
#include "Light.h"


namespace light {

void Light::initLight()
{
    initCamera();

	Look    = GVECTOR3(0.0f, 0.0f, 1.0f);
	Color     = GXCOLOR  (1.0f, 1.0f, 1.0f, 1.0f);
	Type      = LightType::Directional; 

}

void Light::setDirection(float x, float y, float z)
{
	Look = GVECTOR3(x,y,z);
    if ( Look != Right )
    {
        rebuildView();
    } else {
        GVECTOR3 r = Right;
        Right.x = r.y;
        Right.y = r.z;
        Right.z = r.x;
        rebuildView();
    }
}

void Light::setPosition(float x, float y, float z)
{
	Position = GVECTOR3(x,y,z);
}

void Light::setColor(float r, float g, float b, float a)
{
	Color = GXCOLOR(r,g,b,a);
}


}