#include "Camera.h"
//extern Camera              g_Camera;

extern camera::Camera g_Camera;
extern bool isCameraInitialized;

namespace camera {
void initCamera()
{

	g_Camera.Position = GVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.Right    = GVECTOR3(1.0f, 0.0f, 0.0f);
	g_Camera.Up       = GVECTOR3(0.0f, 1.0f, 0.0f);
	//g_Camera.Look     = GVECTOR3(0.0f, 0.0f, -1.0f); // -Z axis projecting from camera RH System
	g_Camera.Look     = GVECTOR3(0.0f, 0.0f, 1.0f); // +Z axis projecting from camera LH System

	GMatrixIdentity(&g_Camera.View);
	GMatrixIdentity(&g_Camera.Proj);
 
	isCameraInitialized = true;
    //return g_Camera;
}

void setLens(float fovY, float aspect, float zn, float zf, bool LH)
{
    if (LH) 
    {
        GMatrixPerspectiveFovLH( &g_Camera.Proj, fovY, aspect, zn, zf);
    }else
    {
        GMatrixPerspectiveFovRH( &g_Camera.Proj, fovY, aspect, zn, zf);
    }
}

void strafe(float d)
{
	g_Camera.Position += d*g_Camera.Right;
}

void walk(float d, bool LH)
{
    if (LH) 
    {
        g_Camera.Position += d*g_Camera.Look;
    }else{
        g_Camera.Position -= d*g_Camera.Look;
    }
}

void rise(float d)
{
	g_Camera.Position += d*g_Camera.Up;
}

void pitch(float angle)
{
	GMATRIX R;
	GMatrixRotationAxis(&R, &g_Camera.Right, angle);

	GVec3TransformNormal(&g_Camera.Up, &g_Camera.Up, &R);
	GVec3TransformNormal(&g_Camera.Look, &g_Camera.Look, &R);
}

void roll(float angle)
{
	GMATRIX R;
	GMatrixRotationAxis(&R, &g_Camera.Look, angle);

	GVec3TransformNormal(&g_Camera.Up, &g_Camera.Up, &R);
	GVec3TransformNormal(&g_Camera.Right, &g_Camera.Right, &R);

}
void rotateY(float angle)//Yaw
{
	GMATRIX R;
	GRotateY(&R, angle);

	GVec3TransformNormal(&g_Camera.Right, &g_Camera.Right, &R);
	GVec3TransformNormal(&g_Camera.Up, &g_Camera.Up, &R);
	GVec3TransformNormal(&g_Camera.Look, &g_Camera.Look, &R);
}

void rebuildView()
{
	if(isCameraInitialized){

		// Keep camera's axes orthogonal to each other and of unit length.
		g_Camera.Look.Normalize();

		//g_Camera.Up = GCrossProduct( g_Camera.Right, g_Camera.Look ); // RH
		g_Camera.Up = GCrossProduct( g_Camera.Look , g_Camera.Right ); // LH
		g_Camera.Up.Normalize();

		//g_Camera.Right = CrossProduct( g_Camera.Look , g_Camera.Up );  //RH
		g_Camera.Right = CrossProduct( g_Camera.Up, g_Camera.Look );  //LH
		g_Camera.Right.Normalize();

		// Fill in the view matrix entries.
		float x = -DotProduct( g_Camera.Position, g_Camera.Right);
		float y = -DotProduct( g_Camera.Position, g_Camera.Up);
		float z = -DotProduct( g_Camera.Position, g_Camera.Look);

		g_Camera.View(0,0) = g_Camera.Right.x; 
		g_Camera.View(1,0) = g_Camera.Right.y; 
		g_Camera.View(2,0) = g_Camera.Right.z; 
		g_Camera.View(3,0) = x;   

		g_Camera.View(0,1) = g_Camera.Up.x;
		g_Camera.View(1,1) = g_Camera.Up.y;
		g_Camera.View(2,1) = g_Camera.Up.z;
		g_Camera.View(3,1) = y;  

		g_Camera.View(0,2) = g_Camera.Look.x; 
		g_Camera.View(1,2) = g_Camera.Look.y; 
		g_Camera.View(2,2) = g_Camera.Look.z; 
		g_Camera.View(3,2) = z;   

		g_Camera.View(0,3) = 0.0f;
		g_Camera.View(1,3) = 0.0f;
		g_Camera.View(2,3) = 0.0f;
		g_Camera.View(3,3) = 1.0f;
	}
}

//Camera& GetCamera()
//{
//	
//	return g_Camera;
//}

}