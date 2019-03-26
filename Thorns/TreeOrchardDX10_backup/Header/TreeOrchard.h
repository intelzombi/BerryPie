#pragma once

#include "resource.h"
#include <GlobalDecl.h>
//#include <d3dx10.h>
#include "Camera.h"
#include <D3D10.h>
#include <string>

HRESULT		InitWindow( HINSTANCE hInstance, int nCmdShow );

HRESULT		InitDevice(void);

void		CleanupDevice();

LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );

LONG	WINAPI	Render();
ID3D10ShaderResourceView* CreateTextureFromFile(std::wstring TexFileName);
ID3D10Buffer * InitTriangleVertexBuffer ( unsigned int uiNumTris   );
void InitTreeVertexBuffer    (ID3D10Buffer * *pvBuff, ID3D10Buffer * *piBuff);
void InitVertexBuffer(ID3D10Buffer * *pvBuff, ID3D10Buffer * *piBuff);
void InitAXVertexBuffer( SG::SceneItem &sgi, int modelIdx );
ID3D10Buffer * InitStreamOutVertexBuffer( unsigned int uiNumPoints );
HRESULT CreateSwapChain ();
HRESULT CreateEffect(LPTSTR fxFile);
HRESULT InitLayouts();
ID3D10InputLayout* InitLayouts(LPCSTR ET, ID3D10Effect* fx, ILOType iloType);
HRESULT CreateRenderView ();
HRESULT CreateDepthStencilView();
HRESULT SetUpViewPort();
void SetViewerTransforms();
void BindAppShaderVars(SG::SceneItem &sgi);
