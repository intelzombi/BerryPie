// Hugh Smith 
// copyright 2015
// all rights reserved
#include "Shuttle.h"

namespace shuttle {
void Shuttle::initShuttle()
{

	Position = GVECTOR3(0.0f, 0.0f, 0.0f);
	Right    = GVECTOR3(1.0f, 0.0f, 0.0f);
	Up       = GVECTOR3(0.0f, 1.0f, 0.0f);
    //_Up      = GVECTOR3(0.0f, 1.0f, 0.0f);
	Look     = GVECTOR3(0.0f, 0.0f, -1.0f); // -Z axis projecting from camera RH System
	//Look     = GVECTOR3(0.0f, 0.0f, 1.0f); // +Z axis projecting from camera LH System

	GMatrixIdentity(&View);

    velocity = GVECTOR3( 0.0f, 0.0f, 0.0f );
    mdlRotation = mdlRotation.Identity;
    radianPitch = 0.0f;
    radianRoll  = 0.0f;
    radianYaw  = 0.0f;

 
	isShuttleInitialized = true;
    
}

void Shuttle::strafe(float d)
{
	Position += d*Right;
}

void Shuttle::walk(float d, bool LH)
{
    GVECTOR3 pos(0.0f, 0.0f, 0.0f);
    pos.x = View._41;
    pos.y = View._42;
    pos.z = View._43;
    if (LH) 
    {
        pos += d*Look;
        //Position += d*Look;
    }else{
        pos -= d*Look;
        //Position -= d*Look;
    }
    View._41 = pos.x;
    View._42 = pos.y;
    View._43 = pos.z;
    
}

void Shuttle::rise(float d)
{
	Position += d*Up;
    Position.Normalize();
}

void Shuttle::pitch(float angle)
{
	GMATRIX R;
	GMatrixRotationAxis( &R, &Right, angle );
    mdlRotation *= R;
	GVec3TransformNormal( &Look, &Look, &R );
	Look.Normalize();
    GVec3TransformNormal( &Up, &Up, &R );
	//Right = CrossProduct( Up, Look );  //LH
 //   Right.Normalize();
	//Up = GCrossProduct( Look , Right ); // LH
    Up.Normalize();
	//GVec3TransformNormal(&Up, &Up, &R);
}

void Shuttle::roll(float angle)
{
	GMATRIX R;
	GMatrixRotationAxis(&R, &Look, angle);
    mdlRotation *= R;
	GVec3TransformNormal(&Right, &Right, &R);
    Right.Normalize();
	//GVec3TransformNormal(&_Up, &Up, &R);
	GVec3TransformNormal(&Up, &Up, &R);
    Up.Normalize();
}
void Shuttle::rotateY(float angle)//Yaw
{
	GMATRIX R;
	GMatrixRotationAxis(&R, &Up, angle);
    mdlRotation *= R;
	GVec3TransformNormal(&Look, &Look, &R);
    Look.Normalize();
	GVec3TransformNormal(&Right, &Right, &R);
    Right.Normalize();
}

void Shuttle::rebuildShuttle()
{

		// Keep camera's axes orthogonal to each other and of unit length.
		Look.Normalize();

        //Right = GCrossProduct( Look , Up );  //RH
		Right = GCrossProduct( Up, Look );  //LH
		Right.Normalize();

		//Up = GCrossProduct( Right, Look ); // RH
		Up = GCrossProduct( Look , Right ); // LH
		Up.Normalize();


		// Fill in the view matrix entries.
		float x = -DotProduct( Position, Right);
		float y = -DotProduct( Position, Up);
		float z = -DotProduct( Position, Look);

		View(0,0) = mdlRotation(0,0); 
		View(1,0) = mdlRotation(1,0); 
		View(2,0) = mdlRotation(2,0); 
		Position.x = View(3,0);   

		View(0,1) = mdlRotation(0,1);
		View(1,1) = mdlRotation(1,1);
		View(2,1) = mdlRotation(2,1);
		Position.y = View(3,1);  

		View(0,2) = mdlRotation(0,2); 
		View(1,2) = mdlRotation(1,2); 
		View(2,2) = mdlRotation(2,2); 
		Position.z = View(3,2);   

		View(0,3) = 0.0f;
		View(1,3) = 0.0f;
		View(2,3) = 0.0f;
		View(3,3) = 1.0f;
        
	
}


}