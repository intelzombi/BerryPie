// Hugh Smith 
// copyright 2015
// all rights reserved
#pragma once

#include "resource.h"
#include <GlobalDecl.h>
#include "Camera.h"
#include <D3D11.h>
#include <string>

HRESULT		InitWindow( HINSTANCE hInstance, int nCmdShow );

HRESULT		InitDevice(void);

void		CleanupDevice();

LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );

LONG	WINAPI	Render();
//void InitAXVertexBuffer( SG::SceneItem &sgi, int modelIdx );
HRESULT CreateSwapChain ();
HRESULT CreateRenderView ();
HRESULT CreateDepthStencilView();
HRESULT SetUpViewPort();
void SetViewerTransforms();
