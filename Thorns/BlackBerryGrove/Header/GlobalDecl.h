// Hugh Smith 
// copyright 2015
// all rights reserved
#ifndef	__GLOBAlDECL_H__
#define __GLOBALDECL_H__

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include "resource.h"
#include "Camera.h"
#include "Shuttle.h"
#include "Light.h"
#include "SceneGraph.h"
#include "AssetImport.h"
#include "EffectsManager.h"
#include "ResourceManager.h"
#include "ModelManager.h"
#include <string>


#pragma warning( disable : 4996 )
#include <strsafe.h>
#pragma warning( default : 4996 )

#include <WinBase.h>

#define _USE_MATH_DEFINES
#include <math.h> 

//#include	<HighPerformanceCounter.h>

#include	<iostream>
#include    <INIFileReader.h>

using std::wstring;

bool           LH = true;
unsigned int   uiLockCamera = 0;
bool           g_WindowedMode = true;

wstring FxFileLoc;
wstring FxFileLocRoot;
wstring SgFileLoc;
wstring SceneFileLoc;
wstring TextureFileLoc;
wstring TextureFileLocRoot;
wstring Tex2FileLoc;
wstring ModelsFileLocRoot;
wstring ProjPath;

static  bool	volatile    g_sbRenderThreadRunning = false;
static  LONG	volatile	g_lvContinueRunningQ = 1;
HANDLE						g_hRenderThreadHandle = 0;

UINT gPass              = 0;
UINT gWindowWidth       = 640;
UINT gWindowHeight      = 480;
HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
ID3D11Device*           g_pd3dDevice = NULL;
ID3D11DeviceContext *   g_pd3dImmediateContext = NULL;
IDXGISwapChain*         g_pSwapChain = NULL;
ID3D11Texture2D*        g_pDepthStencilBuffer;
ID3D11RenderTargetView* g_pRenderTargetView = NULL;
ID3D11DepthStencilView* g_pDepthStencilView;

EffectsManager          g_fxManager;
ResourceManager         g_txManager;
MdlMgr::ModelManager    g_mdlManager;

GVECTOR4                 g_EyePos;
GVECTOR4                 g_LightDir1;
GVECTOR4                 g_LightDir2;
GVECTOR4                 g_LightDir3;
GVECTOR3					g_Spec;
GVECTOR3					g_El1;
GVECTOR3					g_El2;
GVECTOR3					g_El3;
float                       g_Pow;
float g_pfaBackColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};


struct CUSTOMVERTEX
{
	GVECTOR3 Position;
    GVECTOR3 Normal;
    GVECTOR2 TexCoord;
};

float                               g_LCFar = 400.0f;
float                               g_LCNear = 0.5f;
float                               g_CFar = 1000.0f;
float                               g_CNear = 0.5f;
// Lights
light::Light                        g_Light1;
light::Light                        g_Light2;
light::Light                        g_Light3;

// Camera and movement
camera::Camera                      g_Camera;

GVECTOR3                            g_CameraPos     ;
GVECTOR3                            g_CameraRight   ;
GVECTOR3                            g_CameraUp      ;
GVECTOR3                            g_CameraLook    ;
POINT                               g_OldMousePos;
GVECTOR3                            g_LookAt(0.0f,3.0f,0.0f);
static bool sbLockLook =false;
bool    positionUpdated = false;
bool    zoomUpdated = false;
POINTS g_ExpressLastPosition, g_ExpressNewPosition;
int     g_lastZoom = 0;
int     g_newZoom  = 0;
shuttle::Shuttle                   *g_Mdl_Plane;
GMATRIX                             g_MtxToUpdate;
GVECTOR3                            g_MdlPosition;
GVECTOR3                            g_MdlVelocity;
GMATRIX                             g_MdlRotation;
float                               g_radianPitch   = 0.0f;
float                               g_radianRoll    = 0.0f;
float                               g_radianYaw     = 0.0f;
float                               g_maxRoll       = 0.0075f;
float                               g_minRoll       = -0.0075f;
float                               g_maxPitch      = 0.0250f;
float                               g_minPitch      = -0.0250f;
float                               g_maxYaw        = 0.0075f;
float                               g_minYaw        = -0.0075f;
float                               g_epsilonYaw    = 0.00090f;
float                               g_epsilonRoll   = 0.0000260f;
float                               g_epsilonPitch  = 0.0000090f;
float                               g_cheeseTime    = 0.0f;
GMATRIX                             g_Identity( 1.0f, 0.0f, 0.0f, 0.0f,
                                                0.0f, 1.0f, 0.0f, 0.0f,
                                                0.0f, 0.0f, 1.0f, 0.0f,
                                                0.0f, 0.0f, 0.0f, 1.0f );

SG::SceneGraph * g_SceneGraph;
AssetImport::AIScene * g_axScene;
#endif		// __GLOBALDECL_H__
