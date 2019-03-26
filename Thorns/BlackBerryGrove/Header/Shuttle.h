// Hugh Smith 
// copyright 2015
// all rights reserved
#pragma once
#include "Vector3.h"
#include "ModelManager.h"

namespace shuttle {

    
class Shuttle {
public:
    //GVECTOR3    _Up;
	GVECTOR3    Position;
	GVECTOR3    Right;
	GVECTOR3    Up;
	GVECTOR3    Look;
    MdlMgr::    ModelItem * myModel;

    GMATRIX     View;
	GMATRIX     StartWorld;
    GMATRIX     World;
    GVECTOR3    velocity;
    bool        isShuttleInitialized;
    GMATRIX     mdlRotation;
    float       radianPitch;
    float       radianRoll;
    float       radianYaw;

    Shuttle(){ isShuttleInitialized = false; };
    void rebuildShuttle();
    void initShuttle();
    void pitch(float angle);
    void roll(float angle);
    void rotateY(float angle);
    void strafe(float d);
    void walk(float d, bool LH= true);
    void rise(float d);
};

}