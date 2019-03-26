// Hugh Smith 
// copyright 2015
// all rights reserved
#include "Camera.h"
//extern Camera              g_Camera;

//extern camera::Camera g_Camera;


namespace camera {
void Camera::initCamera()
{

	Position = GVECTOR3(0.0f, 0.0f, 0.0f);
	Right    = GVECTOR3(1.0f, 0.0f, 0.0f);
	Up       = GVECTOR3(0.0f, 1.0f, 0.0f);
    _Up      = GVECTOR3(0.0f, 1.0f, 0.0f);
	//Look     = GVECTOR3(0.0f, 0.0f, -1.0f); // -Z axis projecting from camera RH System
	Look     = GVECTOR3(0.0f, 0.0f, 1.0f); // +Z axis projecting from camera LH System

	GMatrixIdentity(&View);
	GMatrixIdentity(&Proj);
 
	isCameraInitialized = true;
    
}

void Camera::setLens(float fovY, float aspect, float zn, float zf, bool LH)
{
    if (LH) 
    {
        GMatrixPerspectiveFovLH( &Proj, fovY, aspect, zn, zf);
    }else
    {
        GMatrixPerspectiveFovRH( &Proj, fovY, aspect, zn, zf);
    }
}

void Camera::strafe(float d)
{
	Position += d*Right;
}

void Camera::walk(float d, bool LH)
{
    if (LH) 
    {
        Position += d*Look;
    }else{
        Position -= d*Look;
    }
}

void Camera::rise(float d)
{
	Position += d*Up;
}

void Camera::pitch(float angle)
{
	GMATRIX R;
	GMatrixRotationAxis( &R, &Right, angle );
	GVec3TransformNormal( &Look, &Look, &R );
	Look.Normalize();
	Right = CrossProduct( _Up, Look );  //LH
    Right.Normalize();
	Up = GCrossProduct( Look , Right ); // LH
    Up.Normalize();
	//GVec3TransformNormal(&Up, &Up, &R);
}

void Camera::roll(float angle)
{
	GMATRIX R;
	GMatrixRotationAxis(&R, &Look, angle);
	GVec3TransformNormal(&Right, &Right, &R);
	GVec3TransformNormal(&_Up, &Up, &R);
	GVec3TransformNormal(&Up, &Up, &R);

}
void Camera::rotateY(float angle)//Yaw
{
	GMATRIX R;
	GMatrixRotationAxis(&R, &Up, angle);
	GVec3TransformNormal(&Look, &Look, &R);
    Look.Normalize();
	GVec3TransformNormal(&Right, &Right, &R);
    Right.Normalize();
}

void Camera::rebuildView()
{
	if(isCameraInitialized){

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

		View(0,0) = Right.x; 
		View(1,0) = Right.y; 
		View(2,0) = Right.z; 
		View(3,0) = x;   

		View(0,1) = Up.x;
		View(1,1) = Up.y;
		View(2,1) = Up.z;
		View(3,1) = y;  

		View(0,2) = Look.x; 
		View(1,2) = Look.y; 
		View(2,2) = Look.z; 
		View(3,2) = z;   

		View(0,3) = 0.0f;
		View(1,3) = 0.0f;
		View(2,3) = 0.0f;
		View(3,3) = 1.0f;
	}
}


}