// Hugh Smith 
// copyright 2015
// all rights reserved
#pragma once
#include "Vector3.h"

namespace camera {

class Camera {
public:
    GVECTOR3 _Up;
	GVECTOR3 Position;
	GVECTOR3 Right;
	GVECTOR3 Up;
	GVECTOR3 Look;

	GMATRIX View;
	GMATRIX Proj;

    bool isCameraInitialized;

    Camera(){ isCameraInitialized = false; };
    void initCamera();
    void setLens(float fovY, float aspect, float zn, float zf, bool LH = true);
    void rebuildView();
    void pitch(float angle);
    void roll(float angle);
    void rotateY(float angle);
    void strafe(float d);
    void walk(float d, bool LH= true);
    void rise(float d);
};

}


