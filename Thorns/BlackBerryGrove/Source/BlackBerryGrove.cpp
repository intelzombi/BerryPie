// BlackBerryGrove.cpp 
// Hugh Smith 
// copyright 2015
// all rights reserved
#define breakPoint //_crtBreakAlloc = 702;

#include <tchar.h>

#include "BlackBerryGrove.h"

void LoadIniFile(LPWSTR lpCmdLine)
{
    //------Config File Initialization--------------------
        // grab vars from config file
    int iReadVal = 0;
	float fReadVal = 0.0f;
    float farray[4] = {1.0f, 1.0f, 1.0f ,1.0f};
    LPTSTR pConfigFileItem;
    CINIFileReader CConfigReader;
    int iSizeBuf = 1024;

	pConfigFileItem = new TCHAR[iSizeBuf];
	
    
    _stprintf_s(pConfigFileItem, iSizeBuf, lpCmdLine);
    CConfigReader.SetIniFile(pConfigFileItem);


    iReadVal = CConfigReader.ReadString(L"INITVALS", L"SCENE", pConfigFileItem, iSizeBuf);
	SceneFileLoc = wstring(pConfigFileItem);
	
	iReadVal = CConfigReader.ReadString(L"INITVALS", L"MODEL_LOCATION", pConfigFileItem, iSizeBuf);
	if ((iReadVal == CConfigReader.GetStringErrorValue()) || (wcscmp(pConfigFileItem, DEFAULT_STRING) == 0))
	{//if iReadInt test fails default to csv file 
		TextureFileLocRoot = wstring(L"..\\models\\");
	}
	else {
		TextureFileLocRoot = wstring(ModelsFileLocRoot);
	}

    iReadVal = CConfigReader.ReadString(L"INITVALS", L"TEXTURE_LOCATION", pConfigFileItem, iSizeBuf);
    if(( iReadVal == CConfigReader.GetStringErrorValue()) || (wcscmp(pConfigFileItem, DEFAULT_STRING) == 0))
    {//if iReadInt test fails default to csv file 
		TextureFileLocRoot = wstring(L"..\\Media\\");
	}
	else {
		TextureFileLocRoot = wstring(pConfigFileItem);
	}

    iReadVal = CConfigReader.ReadString(L"INITVALS", L"EFFECTS_LOCATION", pConfigFileItem, iSizeBuf);
	if ((iReadVal == CConfigReader.GetStringErrorValue()) || (wcscmp(pConfigFileItem, DEFAULT_STRING) == 0))
    {//if iReadInt test fails default to csv file 
		FxFileLocRoot = wstring(L"..\\FX\\");
	}
	else {
		FxFileLocRoot = wstring(pConfigFileItem);
	}


    fReadVal = CConfigReader.ReadBrace4Float(L"INITVALS", L"BACKCOLOR", g_pfaBackColor);

    fReadVal = CConfigReader.ReadBrace4Float(L"INITVALS", L"SPEC1", farray);
        g_Spec.x = farray[0];
        g_Spec.y = farray[1];
        g_Spec.z = farray[2];

    fReadVal = CConfigReader.ReadBrace4Float(L"INITVALS", L"EL1", farray);

        g_El1.x = farray[0];
        g_El1.y = farray[1];
        g_El1.z = farray[2];

    fReadVal = CConfigReader.ReadBrace4Float(L"INITVALS", L"EL2", farray);

        g_El2.x = farray[0];
        g_El2.y = farray[1];
        g_El2.z = farray[2];

    fReadVal = CConfigReader.ReadBrace4Float(L"INITVALS", L"EL3", farray);

        g_El3.x = farray[0];
        g_El3.y = farray[1];
        g_El3.z = farray[2];



    fReadVal = CConfigReader.ReadFloat(L"INITVALS", L"POW");

        g_Pow = fReadVal;



    iReadVal = CConfigReader.ReadInt(L"INITVALS", L"RESOLUTIONWIDTH");

    if (iReadVal != CConfigReader.GetIntErrorValue())
    {
        gWindowWidth = iReadVal;
    }

    iReadVal = CConfigReader.ReadInt(L"INITVALS", L"RESOLUTIONHEIGHT");

    if (iReadVal != CConfigReader.GetIntErrorValue())
    {
        gWindowHeight = iReadVal;
    }


    iReadVal = CConfigReader.ReadInt(L"INITVALS", L"WINDOWEDMODE");

    if (iReadVal != CConfigReader.GetIntErrorValue())
    {
        g_WindowedMode = (bool) iReadVal;
    }

    iReadVal = CConfigReader.ReadInt(L"INITVALS", L"LOCKCAMERA");

    if (iReadVal != CConfigReader.GetIntErrorValue())
    {
        uiLockCamera = iReadVal;
    }

    if(pConfigFileItem) delete pConfigFileItem;
    return;
}

/***************************************************************************************************\
Function Name: wWinMain

Description:   

Input: 
    HINSTANCE hInstance:  

	HINSTANCE hPrevInstance:  

	LPWSTR lpCmdLine:  

	int nCmdShow:  

Return: 

\***************************************************************************************************/

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    breakPoint //_crtBreakAlloc = 551;
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    LoadIniFile(lpCmdLine);
	TCHAR pPath[FILENAME_MAX];
	GetCurrentDirectoryW(sizeof(pPath),pPath);
	ProjPath = wstring(pPath);
//------End Config File Initialization--------------------
    if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
        return 0;

    if( FAILED( InitDevice() ) )
    {
        CleanupDevice();
        return 0;
    }

	//  Create Render() thread here:
	DWORD	dwRenderThreadID = 0;
	g_hRenderThreadHandle	=	
		CreateThread( NULL, 0,(LPTHREAD_START_ROUTINE)Render, NULL, 0, &dwRenderThreadID );

    // Main message loop
    MSG msg = {0};

    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }

    }

	// Window received a quit message - terminate the render thread:
	LONG	volatile lvMyDestVal = 0;
	LONG	lvOldValue = g_lvContinueRunningQ;

	while( lvOldValue != InterlockedCompareExchange(  &g_lvContinueRunningQ, lvMyDestVal, (LONG)1 ) )
		;
	
	//  Let the Render() close on its own so that it can cleanup and gracefully exit
	//  then close the thread
	while( g_sbRenderThreadRunning )
		;
	CloseHandle( g_hRenderThreadHandle );
	
	//Clean up and exit
    CleanupDevice();

    return ( int )msg.wParam;
}	// wWinMain( ... )

/***************************************************************************************************\
Function Name: InitWindow

Description:   

Input: 
    HINSTANCE hInstance:  

	int nCmdShow:  

Return: 

\***************************************************************************************************/

HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
	bool fullscreen = false;
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_MYBLACKBERRYGROVE );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	//if (g_WindowedMode)
	//{
		wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
	//}
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"AIX";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_MYBLACKBERRYGROVE );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, gWindowWidth, gWindowHeight };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
	if( g_WindowedMode)
	{
		g_hWnd = CreateWindowEx( NULL, L"AIX", L"BlackBerryGrove", WS_OVERLAPPEDWINDOW,
							   1, 1, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
							   NULL );
	} else {
		g_hWnd = CreateWindowEx( NULL, L"AIX", L"BlackBerryGrove", WS_EX_TOPMOST | WS_POPUP,
							   1, 1, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
							   NULL );
	}
	if( !g_hWnd )
        return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );

    return S_OK;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	POINT mousePos;
	int dx = 0;
	int dy = 0;
	switch(message)
	{
	case WM_LBUTTONDOWN:
		if( wParam & MK_LBUTTON && !uiLockCamera )
		{
			SetCapture(hWnd);

			g_OldMousePos.x = LOWORD(lParam);
			g_OldMousePos.y = HIWORD(lParam);
		}
		return 0;

	case WM_LBUTTONUP:
		ReleaseCapture();
		return 0;

	case WM_MOUSEMOVE:
		if( wParam & MK_LBUTTON && !uiLockCamera )
		{
			mousePos.x = (int)LOWORD(lParam); 
			mousePos.y = (int)HIWORD(lParam); 

			dx = mousePos.x - g_OldMousePos.x;
			dy = mousePos.y - g_OldMousePos.y;

			g_Camera.pitch( dy * 0.0087266f );
			g_Camera.rotateY( dx * 0.0087266f );
			
			g_OldMousePos = mousePos;
		}
		return 0;
    //case WM_PAINT:
    //    hdc = BeginPaint( hWnd, &ps );
    //    EndPaint( hWnd, &ps );
    //    break;
    case WM_GESTURENOTIFY:
	    {
		    // set up our want / block settings
		    DWORD dwPanWant  = GC_PAN_WITH_SINGLE_FINGER_VERTICALLY | GC_PAN_WITH_SINGLE_FINGER_HORIZONTALLY;
		    DWORD dwPanBlock = GC_PAN_WITH_GUTTER | GC_PAN_WITH_INERTIA;

		    // set the settings in the gesture configuration
		    GESTURECONFIG gc[] = {
			    { GID_ZOOM, GC_ZOOM, 0 },
			    { GID_ROTATE, GC_ROTATE, 0},
			    { GID_PAN, dwPanWant , dwPanBlock}                     
		    };    

		    UINT uiGcs = 3;
		    SetGestureConfig(hWnd, 0, uiGcs, gc, sizeof(GESTURECONFIG));  
	    }
	    break;
    case WM_GESTURE:
	    {
		    // Create a structure to populate and retrieve the extra message info.
		    GESTUREINFO gi;  

		    ZeroMemory(&gi, sizeof(GESTUREINFO));

		    gi.cbSize = sizeof(GESTUREINFO);

		    BOOL bResult  = GetGestureInfo((HGESTUREINFO)lParam, &gi);
		    //m_CPUT->CPUTHandleGestureEvent(&gi);
	        switch(gi.dwID)
	        {
	        case GID_ROTATE:
		        if (gi.dwFlags & GF_BEGIN) 
		        {
			        // starting a Pan gesture. See if we want to spin the top
//			        g_TopSpinSpeed = 0;
//			        g_LastAngle = 0.;
		        } else {
			        double newAngle = -GID_ROTATE_ANGLE_FROM_ARGUMENT(gi.ullArguments);
//			        double angleUpdate = newAngle - g_LastAngle;
//			        g_TopSpinSpeed += (float)angleUpdate*10.f;
//			        g_LastAngle = newAngle;
		        }
		        return 0;//CPUT_EVENT_HANDLED;
	        case GID_PAN:
		        if (gi.dwFlags & GF_BEGIN)
		        {
                    POINT local;
                    local.x = gi.ptsLocation.x;
                    local.y = gi.ptsLocation.y;
                    ScreenToClient(hWnd, &local);
                    g_ExpressLastPosition.x = local.x;
                    g_ExpressLastPosition.y = local.y;
                    g_ExpressNewPosition = g_ExpressLastPosition;
		        } else {
                    g_ExpressLastPosition = g_ExpressNewPosition;
                    POINT local;
                    local.x = gi.ptsLocation.x;
                    local.y = gi.ptsLocation.y;
                    ScreenToClient(hWnd, &local);
                    g_ExpressNewPosition.x = local.x;
                    g_ExpressNewPosition.y = local.y;
                    positionUpdated = true;
		        }
		        return 0;//CPUT_EVENT_HANDLED;
		        break;
	        case GID_ZOOM:
		        if (gi.dwFlags & GF_BEGIN)
		        {
			        g_lastZoom = (int)gi.ullArguments;
		        } else {
			        g_newZoom = (int)gi.ullArguments;
                    zoomUpdated = true;
		        }
		        return 0;//CPUT_EVENT_HANDLED;
		        break;
	        }
	        return 1;//CPUT_EVENT_UNHANDLED;

	    }
	    break;
	case WM_MOUSEWHEEL:
        if(wParam && !uiLockCamera ){
            short direction = (short)HIWORD(wParam);
            if(direction < 0){
                g_Camera.walk(-20.0f*0.020f,LH);
            }
            if(direction > 0){
                g_Camera.walk(+20.0f*0.020f,LH);
            }
        }
        return 0;
 	case WM_KEYDOWN:						
		{
            if (wParam == VK_ESCAPE) //If the escape key was pressed
            {
			    PostQuitMessage(0);					
            }
            break;
		}      
    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;
    default:
        return DefWindowProc( hWnd, message, wParam, lParam );
    }
    return 0;
}

void CreateAXSceneGraph()
{
	g_fxManager.init();
	//find out how many scene items 
	wstring ScenePath = ProjPath + SceneFileLoc;
	g_axScene = new AssetImport::AIScene(ScenePath.c_str());
	int sceneItemCount = g_axScene->objectCount;
	g_SceneGraph = new SG::SceneGraph(sceneItemCount); //+1 );////HUGH_HACK  remove the plus one for ShadowOut 

	//Create View Sets  
	// BackBuffer is passItem[0]
	SG::PassItem * pi = new SG::PassItem();
	pi->viewSet = g_txManager.createViewSet(L"BackBuffer", ViewSetType::BackBuffer, gWindowWidth, gWindowHeight);
	g_SceneGraph->passItems.push_back(pi);

	// ShadowMap is passItem[1]
	pi = new SG::PassItem(); 
	pi->viewSet = g_txManager.createViewSet(L"ShadowMap", ViewSetType::ShadowMap, gWindowWidth, gWindowHeight);
	FxFileLoc = ProjPath + FxFileLocRoot + L"shadowMapLinear.edf";
	// FxFileLoc = ProjPath + FxFileLocRoot + L"%sshadowMap.edf";
	effectPacketItem *epi = g_fxManager.createEffect(FxFileLoc);
	pi->sampleState = epi->samplerState;
	g_SceneGraph->passItems.push_back(pi);
	// end Create view Sets

	// ShadowMap Scene Items
	// This Scene Item has no Geometry.  Setup is the same for all Geometry
	// So in essence we substitue the Geometries (models) Render States with this for 
	// the creation of the Shadow Map.  
	SG::SceneItem shadowSI;
	std::pair<wstring, SG::SceneItem> ssiPair;
	ssiPair.first = L"ShadowMap";
	ssiPair.second.model = NULL;

	SG::RenderItem * renderable = new SG::RenderItem();
	renderable->effectMask = epi->effectMask;
	renderable->depthStencilState = epi->depthStencilState;
	renderable->vtxShader = epi->vertexShader;
	renderable->vtxCBuffer = epi->vsConstantBuffer;
	renderable->pxlShader = epi->pixelShader;
	renderable->pxlCBuffer = epi->psConstantBuffer;
	renderable->rasterState = epi->rasterState;
	renderable->sampleState = epi->samplerState;
	renderable->blendState = epi->blendState;
	ssiPair.second.renderItemList.push_back(renderable);
	g_SceneGraph->mp_sceneGraph.insert(ssiPair);
	delete epi;
	epi = NULL;
	// end Shadow Map Scene Item

	// Start loading Scene File Geometry;
	// the Geometry (Models) were loaded into the AssetExporter Scene Model Map and is extracted here
	// and each one is translated to a Asset Import Model and stored in the Scene Graph. Each model is
	// broken down into Render Items based on Materials used. 
	ID3D11InputLayout * ilp = NULL;
	for (auto axModel : g_axScene->modelsMap)
	{
		MdlMgr::AXModel *model;
		SG::SceneItem si; 
		std::pair<wstring, SG::SceneItem> siPair;
		model = reinterpret_cast< MdlMgr::AXModel *> (axModel.second);
		MdlMgr::ModelItem *modelItem = g_mdlManager.CreateDXModelItem(model);
		siPair.first = modelItem->modelName;
		siPair.second.model = modelItem;
		// Populate Scene Graph with Material, Texture, FX, and Technique Details
		// Get texture file from material by comparing mesh material name with material collection.
		// Model Items is the storage unit for the Model. For each Model there is 1 or more Index buffers 
		// that point to parts of the Vertex Buffer.  There is a index List (not necessarily congruant)
		// for each material associated with the Model. Therefore each index list can also be thought of 
		// as a RenderItem.  
		bool found = false;
		for (auto ri : modelItem->indexLists) {
			ri->AttributeName;
			auto materialItr = g_axScene->materialsMap.find(ri->AttributeName);
			if (materialItr != g_axScene->materialsMap.end())
			{
				found = true;
				SG::RenderItem * renderable = new SG::RenderItem();
				TextureFileLoc = ProjPath + TextureFileLocRoot + materialItr->second->textureName; 
				renderable->texture = g_txManager.createShaderViewFromTextureFile(TextureFileLoc, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
				//populate scenegraphitem
				FxFileLoc = ProjPath + FxFileLocRoot + materialItr->second->FXFile;
				effectPacketItem *epi = g_fxManager.createEffect(FxFileLoc);
				renderable->effectMask = epi->effectMask;
				renderable->blendState = epi->blendState;
				renderable->depthStencilState = epi->depthStencilState;
				renderable->pxlShader = epi->pixelShader;
				renderable->pxlCBuffer = epi->psConstantBuffer;
				renderable->rasterState = epi->rasterState;
				renderable->sampleState = epi->samplerState;
				renderable->vtxShader = epi->vertexShader;
				renderable->vtxCBuffer = epi->vsConstantBuffer;
				renderable->ILO = g_fxManager.InitLayouts((ILOType)modelItem->inputLayoutType, epi);
				renderable->idxBuffer = ri;
				siPair.second.renderItemList.push_back(renderable);
				delete epi;
			}
		}
		g_SceneGraph->mp_sceneGraph.insert(siPair);
	}
	// Scene File Geometer added to Scene Graph

	// Non Scene File Geometry processing
	// This includes Immediate Geometry Definitions and Procedural Geometry
	

	/// Hugh_HACKMAN  display Depth Buffer Scene Item.
/*	
	MdlMgr::AXModel *model = new MdlMgr::AXModel();
	model->numVerts = 4;
	model->triCount = 2;
	model->inputLayoutName = wstring(L"PT" );
	model->objectName = wstring(L"DepthBufferSceneItem" );
	model->MaterialCount = 1;
	model->inputLayoutType = ILOType::PT;

	MdlMgr::TRIANGLE * tri = new MdlMgr::TRIANGLE[2];

	tri[0].vertex[0] = 0;
	tri[0].vertex[1] = 1;
	tri[0].vertex[2] = 2;
	tri[1].vertex[0] = 2;
	tri[1].vertex[1] = 1;
	tri[1].vertex[2] = 3;

	MdlMgr::AXModelIndex * ami = new MdlMgr::AXModelIndex();
	ami->triangles = tri;
	ami->faceCount = 2;
	ami->materialName = wstring(L"ShadowOut" );
	std::pair<wstring, MdlMgr::AXModelIndex *> idxPair;
	idxPair.first = ami->materialName;
	idxPair.second = ami;
	model->indexMap.insert(idxPair);
	
	MdlMgr::PT_VERTEX *ptv = new MdlMgr::PT_VERTEX[4];

	ptv[0].Position = GVECTOR3( -47.500000f, 3.500000f, -30.000000f );
	ptv[1].Position = GVECTOR3( -22.500000f, 3.500000f, -30.000000f );
	ptv[2].Position = GVECTOR3( -47.500000f, 28.500000f, -30.000000f );
	ptv[3].Position = GVECTOR3( -22.500000f, 28.500000f, -30.000000f );
	
	ptv[0].TexCoord = GVECTOR2( 0.000000f, 1.000000f );
	ptv[1].TexCoord = GVECTOR2( 1.000000f, 1.000000f );
	ptv[2].TexCoord = GVECTOR2( 0.000000f, 0.000000f );
	ptv[3].TexCoord = GVECTOR2( 1.000000f, 0.000000f );

	model->ptvertices = ptv;

	MdlMgr::ModelItem* modelItem = g_mdlManager.CreateDXModelItem( model );

	renderable = new SG::RenderItem();
	TextureFileLoc = ProjPath + TextureFileLocRoot + wstring(L".\\ShadowMap");
	renderable->texture = g_txManager.createShaderViewFromTextureFile(TextureFileLoc, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);// DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT);

	FxFileLoc = ProjPath + FxFileLocRoot + wstring( L"ShadowOut.edf");
	epi = g_fxManager.createEffect(FxFileLoc);
	renderable->effectMask			= epi->effectMask       ;
	renderable->blendState          = epi->blendState       ;
	renderable->depthStencilState   = epi->depthStencilState;
	renderable->pxlShader           = epi->pixelShader      ;
	renderable->pxlCBuffer          = epi->psConstantBuffer ;
	renderable->rasterState         = epi->rasterState      ;
	renderable->sampleState         = epi->samplerState     ;
	renderable->vtxShader           = epi->vertexShader     ;
	renderable->vtxCBuffer          = epi->vsConstantBuffer ;
	renderable->vtxCBuffer          = epi->vsConstantBuffer ;
	renderable->ILO                 =   g_fxManager.InitLayouts( ILOType::PT, epi);
	renderable->idxBuffer           = modelItem->indexLists[0];
	
	std::pair<wstring, SG::SceneItem > siPair;
	SG::SceneItem si;
	si.renderItemList.push_back(renderable);
	si.model = modelItem;
	siPair.first = modelItem->modelName;
	siPair.second = si;
	g_SceneGraph->mp_sceneGraph.insert(siPair);

	delete epi;
	*/
	/// END Hugh_HACKMAN
}

void SetUpLevel()
{
    g_Mdl_Plane = 0;
	auto model = g_SceneGraph->mp_sceneGraph.find(L"Plane");
	if(model != g_SceneGraph->mp_sceneGraph.end())
	{ 
		g_Mdl_Plane = new shuttle::Shuttle();
		g_Mdl_Plane->initShuttle();
		g_Mdl_Plane->myModel = model->second.model;
		g_Mdl_Plane->World = g_Mdl_Plane->StartWorld = model->second.model->myWorld;
		g_Mdl_Plane->velocity.x = 0.0f;
		g_Mdl_Plane->velocity.y = 0.0f;
		g_Mdl_Plane->velocity.z = 0.0f;
		g_Mdl_Plane->mdlRotation = g_Mdl_Plane->mdlRotation.Identity;
		g_Mdl_Plane->Position.x = 0.0f;
		g_Mdl_Plane->Position.y = 0.0f;
		g_Mdl_Plane->Position.z = 0.0f;
		g_Mdl_Plane->rebuildShuttle();
	}
}

HRESULT InitDevice()
{
    HRESULT hr = S_OK;;

    RECT rc;
    GetClientRect( g_hWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    hr = CreateSwapChain();
    
    while (! g_pd3dDevice ) {};
	
	// create Scene Graph
	CreateAXSceneGraph();


	// Setup the viewport
    if( SUCCEEDED(hr) )
	{
        hr = SetUpViewPort();
    }
    
    SetViewerTransforms();
    
    SetUpLevel();

    return S_OK;
}

void lockLook(){
    sbLockLook = true;
}

void unlockLook(){
    sbLockLook = false;
}



// UI input
void updateUI(){
        
	if (!uiLockCamera) 
    {
	    if(GetAsyncKeyState('L') & 0x8000)	lockLook();
	    if(GetAsyncKeyState('U') & 0x8000)	unlockLook();
        if(sbLockLook){
	        if(GetAsyncKeyState('A') & 0x8000)	    g_Light1.strafe(-200.0f*0.0020f);
	        if(GetAsyncKeyState('D') & 0x8000)	    g_Light1.strafe(+200.0f*0.0020f);
	        if(GetAsyncKeyState('W') & 0x8000)	    g_Light1.walk(+200.0f*0.0020f);
	        if(GetAsyncKeyState('S') & 0x8000)	    g_Light1.walk(-200.0f*0.0020f);
	        if(GetAsyncKeyState('F') & 0x8000)	    g_Light1.rise(+200.0f*0.0020f);
	        if(GetAsyncKeyState('C') & 0x8000)	    g_Light1.rise(-200.0f*0.0020f);
	        if(GetAsyncKeyState('I') & 0x8000)	    g_Light1.roll(-0.0087266f);
	        if(GetAsyncKeyState('O') & 0x8000)	    g_Light1.roll(0.0087266f);
	        if(GetAsyncKeyState(VK_LEFT) & 0x8000)	g_Light1.rotateY(-0.01f);
	        if(GetAsyncKeyState(VK_RIGHT) & 0x8000)	g_Light1.rotateY(0.01f); 
	        if(GetAsyncKeyState(VK_UP) & 0x8000)	g_Light1.pitch(-0.01f);
	        if(GetAsyncKeyState(VK_DOWN) & 0x8000)	g_Light1.pitch(0.01f);
        }else{
	        if(GetAsyncKeyState('A') & 0x8000)	g_Camera.strafe(-200.0f*0.0020f);
	        if(GetAsyncKeyState('D') & 0x8000)	g_Camera.strafe(+200.0f*0.0020f);
	        if(GetAsyncKeyState('W') & 0x8000)	g_Camera.walk(+200.0f*0.0020f);
	        if(GetAsyncKeyState('S') & 0x8000)	g_Camera.walk(-200.0f*0.0020f);
	        if(GetAsyncKeyState('F') & 0x8000)	g_Camera.rise(+200.0f*0.0020f);
	        if(GetAsyncKeyState('C') & 0x8000)	g_Camera.rise(-200.0f*0.0020f);
	        if(GetAsyncKeyState('I') & 0x8000)	g_Camera.roll(-0.0087266f);
	        if(GetAsyncKeyState('O') & 0x8000)	g_Camera.roll(0.0087266f);
	        if(GetAsyncKeyState('L') & 0x8000)	lockLook();
	        if(GetAsyncKeyState('U') & 0x8000)	unlockLook();
            if( g_Mdl_Plane ) 
            {
	            if(GetAsyncKeyState(VK_LEFT) & 0x8000)	g_Mdl_Plane->radianRoll  += 0.0001125f ;
	            if(GetAsyncKeyState(VK_RIGHT) & 0x8000)	g_Mdl_Plane->radianRoll  -= 0.0001125f ; 
	            if(GetAsyncKeyState(VK_UP) & 0x8000)	g_Mdl_Plane->radianPitch += 0.0000975f   ;
	            if(GetAsyncKeyState(VK_DOWN) & 0x8000)	g_Mdl_Plane->radianPitch -= 0.0000975f   ;
            }
        }
        if( g_Mdl_Plane ) 
        {
	        if(GetAsyncKeyState('H') & 0x8000)	    g_Mdl_Plane->velocity.z  += 0.00010f    ;
	        if(GetAsyncKeyState('B') & 0x8000)	    g_Mdl_Plane->velocity.z  -= 0.00010f    ;
        }
    }
}

// Set All Cameras
void updateCameras(){
        //if(sbLockLook){
        //    g_Camera.Look = g_LookAt - g_Camera.Position;
        //}

        // Track Model
        if( g_Mdl_Plane )
            g_Camera.Look = g_Mdl_Plane->Position - g_Camera.Position;
		g_Camera.pitch( 0.0f );
		g_Camera.rotateY( 0.0f );
        
        g_Camera.Look.Normalize();
        g_Camera.rebuildView();
		//Set Eye Position
		g_EyePos = GVECTOR4(g_Camera.Position.x,g_Camera.Position.y,g_Camera.Position.z,1.0f);

}
	
void setLights(){
		//Set Light Directions
		g_LightDir1 = GVECTOR4(g_Light1.Look.x,g_Light1.Look.y,g_Light1.Look.z,1.0f);
		g_LightDir2 = GVECTOR4(g_Light2.Look.x,g_Light2.Look.y,g_Light2.Look.z,1.0f);
		g_LightDir3 = GVECTOR4(g_Light3.Look.x,g_Light3.Look.y,g_Light3.Look.z,1.0f);
        GVec4Normalize(&g_LightDir1, &g_LightDir1);
        GVec4Normalize(&g_LightDir2, &g_LightDir2);
        GVec4Normalize(&g_LightDir3, &g_LightDir3);

		//setLightColor
		//g_El2 = GVECTOR3(0.5f,0.8f,0.1f);
		//g_El3 = GVECTOR3(0.1f,0.5f,0.8f);
		//g_El2 = GVECTOR3(0.1f,0.1f,0.1f);
		//g_El3 = GVECTOR3(0.1f,0.1f,0.1f);

}

//Hugh_Hackman
void updatePlane()
{
    GVECTOR3 aboutXAxis(1.0f,0.0f,0.0f);
    GVECTOR3 aboutZAxis(0.0f,0.0f,1.0f);
    GMATRIX temp;
    temp = g_MtxToUpdate;

    g_cheeseTime -= 0.01f;
    g_radianPitch += 0.0035f;
    g_radianRoll  += 0.00525f;
    GMatrixRotationAxis( &g_MdlRotation, &aboutZAxis, g_radianRoll );
    g_MtxToUpdate *= g_MdlRotation;
    g_MtxToUpdate._42 = -20.68f;
    GMatrixRotationAxis( &g_MdlRotation, &aboutXAxis, g_radianPitch );
    g_MtxToUpdate *= g_MdlRotation;
    g_MtxToUpdate._42 += 29.0f;
    g_MtxToUpdate = temp;

}

void update_Plane_Shuttle()
{
    //Hugh hackman test touch interface
    if( positionUpdated )
    {
    if( g_ExpressLastPosition.x > g_ExpressNewPosition.x  + 3.0f )
		g_Mdl_Plane->radianRoll  += (g_ExpressLastPosition.x - g_ExpressNewPosition.x) * 0.01125f ; //VK_LEFT 
    if( g_ExpressLastPosition.x < g_ExpressNewPosition.x - 3.0f )
		g_Mdl_Plane->radianRoll  += (g_ExpressLastPosition.x - g_ExpressNewPosition.x) * 0.01125f ; //VK_RIGHT
    if( g_ExpressLastPosition.y > g_ExpressNewPosition.y + 3.0f  )
		g_Mdl_Plane->radianPitch += (g_ExpressLastPosition.y - g_ExpressNewPosition.y) * 0.000975f ; //VK_UP   
    if( g_ExpressLastPosition.y < g_ExpressNewPosition.y - 3.0f )
		g_Mdl_Plane->radianPitch += (g_ExpressLastPosition.y - g_ExpressNewPosition.y) * 0.000975f ; //VK_DOWN 
    //g_ExpressLastPosition = g_ExpressNewPosition;
        positionUpdated = false;
        g_Mdl_Plane->velocity.z = 0.4f;
    }

    if ( zoomUpdated )
    {     
	        g_Camera.walk( (g_newZoom - g_lastZoom ) * 0.020f);//'W'
	        zoomUpdated = false;
    }
    // end Hugh hackman

    g_Mdl_Plane->rotateY( g_Mdl_Plane->radianYaw ); 
    g_Mdl_Plane->roll(  g_Mdl_Plane->radianRoll  );
    g_Mdl_Plane->pitch( g_Mdl_Plane->radianPitch );
    g_Mdl_Plane->walk(  g_Mdl_Plane->velocity.z  );
    g_Mdl_Plane->rebuildShuttle();
    float rx, ry, rz, xzRoot, zxyRoot;
    rx = g_Mdl_Plane->View._41;
    ry = g_Mdl_Plane->View._42;
    rz = g_Mdl_Plane->View._43;
    xzRoot = sqrtf( rx * rx + rz * rz );
    zxyRoot = sqrtf( xzRoot * xzRoot + ry * ry );
    if ( zxyRoot > 245.0f ) 
    {
        g_Mdl_Plane->View._41 = 0.0f;
        g_Mdl_Plane->View._42 = 2.0f;
        g_Mdl_Plane->View._43 = 0.0f;
    }
    if ( ry < 0 )
    {
        g_Mdl_Plane->View._42 = 0.0f;
    }
    g_Mdl_Plane->rebuildShuttle();

    g_Mdl_Plane->myModel->myWorld = g_Mdl_Plane->View;
    g_Mdl_Plane->radianRoll  *= 0.98f;
    g_Mdl_Plane->radianPitch *= 0.98f;
    g_Mdl_Plane->radianYaw   *= 0.999f;

    if ( g_Mdl_Plane->radianRoll > g_maxRoll )
         g_Mdl_Plane->radianRoll = g_maxRoll;
    if ( g_Mdl_Plane->radianRoll < g_minRoll )
         g_Mdl_Plane->radianRoll = g_minRoll;

    if ( g_Mdl_Plane->radianPitch > g_maxPitch )
         g_Mdl_Plane->radianPitch = g_maxPitch;
    if ( g_Mdl_Plane->radianPitch < g_minPitch )
         g_Mdl_Plane->radianPitch = g_minPitch;
        
    if ( g_Mdl_Plane->radianYaw > g_maxYaw )
         g_Mdl_Plane->radianYaw = g_maxYaw;
    if ( g_Mdl_Plane->radianYaw < g_minYaw )
         g_Mdl_Plane->radianYaw = g_minYaw;

    if ( g_Mdl_Plane->radianRoll < g_epsilonRoll && g_Mdl_Plane->radianRoll > -g_epsilonRoll )
         g_Mdl_Plane->radianRoll = 0;

    if ( g_Mdl_Plane->radianPitch < g_epsilonPitch && g_Mdl_Plane->radianPitch > -g_epsilonPitch )
         g_Mdl_Plane->radianPitch = 0;
    
    if ( g_Mdl_Plane->radianYaw < g_epsilonYaw && g_Mdl_Plane->radianYaw > -g_epsilonYaw )
         g_Mdl_Plane->radianYaw = 0;

    if( ( g_Mdl_Plane->Up.y < 0.35f && g_Mdl_Plane->Up.y > -0.35f )  &&
        ( g_Mdl_Plane->Right.y > 0.6f || g_Mdl_Plane->Right.y < -0.6f ) &&
          g_Mdl_Plane->Look.y > 0 )
    {
            g_Mdl_Plane->radianYaw -= g_Mdl_Plane->Right.y * 0.00035f;

    }
}

void update_Shuttles()
{
    if( g_Mdl_Plane )
        update_Plane_Shuttle();
}

void updateFrame()
{
    updateUI();
    updateCameras();
    setLights();
    update_Shuttles();
    //updatePlane(); 
}

void setConstants(PassType pt, wstring sceneitem)
{
	GMATRIX world = g_SceneGraph->mp_sceneGraph.find(sceneitem)->second.model->myWorld;

	g_Light1.rebuildView();
	GMATRIX projLinear = g_Light1.Proj;
	projLinear(2, 2) /= g_LCFar;//1000.0f;
	projLinear(3, 2) /= g_LCFar;//1000.0f;

	GMATRIX lprojview = world * g_Light1.View * projLinear; //g_Light1.Proj;
	GMATRIX projview = world * g_Camera.View * g_Camera.Proj;

	GMATRIX ltpose = lprojview;
	//Transpose(ltpose);
	GMATRIX tpose = projview;
	//Transpose( tpose );

	switch (pt)
	{
	case PassType::SHADOW:
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		g_pd3dImmediateContext->Map(g_fxManager.constantBuffersMap.find(L"LIGHTEQ_EF_VS")->second->pBff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		LIGHTEQ_EF_VS_CONSTANT_BUFFER* pvsConstData = (LIGHTEQ_EF_VS_CONSTANT_BUFFER*)mappedResource.pData;
		pvsConstData->cbViewProj = projview;
		pvsConstData->cbLightViewProj = lprojview;
		//pvsConstData->cbViewProj  = tpose;
		//pvsConstData->cbLightViewProj  = ltpose;
		//pvsConstData->cbLightViewProj  = tpose;
		g_pd3dImmediateContext->Unmap(g_fxManager.constantBuffersMap.find(L"LIGHTEQ_EF_VS")->second->pBff, 0);
	}
	break;
	}
}

void setConstants(wstring sceneitem) {
	GMATRIX world = g_SceneGraph->mp_sceneGraph.find(sceneitem)->second.model->myWorld;
	GMATRIX projview = world *  g_Camera.View * g_Camera.Proj;
	GMATRIX tpose = projview;
	//Transpose( tpose );
	///-----------------
	GMATRIX projLinear = g_Light1.Proj;
	projLinear(2, 2) /= g_LCFar;//1000.0f;
	projLinear(3, 2) /= g_LCFar;//1000.0f;

	GMATRIX lprojview = world * g_Light1.View * projLinear; //g_Light1.Proj

															///----------------

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	g_pd3dImmediateContext->Map(g_fxManager.constantBuffersMap.find(L"LIGHTEQ_EF_VS")->second->pBff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	LIGHTEQ_EF_VS_CONSTANT_BUFFER* pvsConstData = (LIGHTEQ_EF_VS_CONSTANT_BUFFER*)mappedResource.pData;
	pvsConstData->cbViewProj = projview;
	pvsConstData->cbLightViewProj = lprojview;
	pvsConstData->cbEyePosW = g_EyePos;
	g_pd3dImmediateContext->Unmap(g_fxManager.constantBuffersMap.find(L"LIGHTEQ_EF_VS")->second->pBff, 0);

	g_pd3dImmediateContext->Map(g_fxManager.constantBuffersMap.find(L"WORLD_EF_PER_OBJECT")->second->pBff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	PER_ITEM_CONSTANT_BUFFER_B1* pvsoConstData = (PER_ITEM_CONSTANT_BUFFER_B1*)mappedResource.pData;
	pvsoConstData->cbWorld = g_SceneGraph->mp_sceneGraph.find(sceneitem)->second.model->myWorld;
	g_pd3dImmediateContext->Unmap(g_fxManager.constantBuffersMap.find(L"WORLD_EF_PER_OBJECT")->second->pBff, 0);

	g_pd3dImmediateContext->Map(g_fxManager.constantBuffersMap.find(L"LIGHTEQ_EF_PS")->second->pBff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	LIGHTEQ_EF_PS_CONSTANT_BUFFER* ppsConstData = (LIGHTEQ_EF_PS_CONSTANT_BUFFER*)mappedResource.pData;
	ppsConstData->cbLightDir1W = -g_LightDir1;
	ppsConstData->cbLightDir2W = -g_LightDir2;
	ppsConstData->cbLightDir3W = -g_LightDir3;
	ppsConstData->cbSpec = g_Spec;
	ppsConstData->cbEl1 = g_El1;
	ppsConstData->cbEl2 = g_El2;
	ppsConstData->cbEl3 = g_El3;
	ppsConstData->cbmPow = g_Pow;
	g_pd3dImmediateContext->Unmap(g_fxManager.constantBuffersMap.find(L"LIGHTEQ_EF_PS")->second->pBff, 0);
}

/***************************************************************************************************\
Function Name: Render

Description:   

Input: 
    NONE

Return: 

\***************************************************************************************************/

LONG	WINAPI	Render()
{

	g_sbRenderThreadRunning = TRUE;
	UINT stride = sizeof(CUSTOMVERTEX);
    UINT offset = 0; 

	while( g_lvContinueRunningQ > 0 )
	{
		updateFrame();

        HRESULT hr = S_OK;
        
		while (gPass < 4 )
        {
        switch (gPass)
        {
        case 0: //ShadowMapPass  Collect the DepthBuffer Resource
            {                
				g_pd3dImmediateContext->OMSetRenderTargets( 0,
                                    0, 
                                    g_SceneGraph->passItems[1]->viewSet->DepthStencilTargets[0] );
    	        //Clear Only the Depth Stencil View
                g_pd3dImmediateContext->ClearDepthStencilView( g_SceneGraph->passItems[1]->viewSet->DepthStencilTargets[0], D3D11_CLEAR_DEPTH , 1.0f, 0);
                // Set primitive topology
                g_pd3dImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
				SG::RenderItem *smRI = g_SceneGraph->mp_sceneGraph.find(L"ShadowMap")->second.renderItemList[0];
				g_pd3dImmediateContext->RSSetState(smRI->rasterState->rasterState);
				g_pd3dImmediateContext->OMSetBlendState(smRI->blendState->blendState, 0, 0xffffffff);
				g_pd3dImmediateContext->OMSetDepthStencilState(smRI->depthStencilState->depthStencilState, 0);
                ID3D11ShaderResourceView* pNullSRVs[3] = { NULL };
                g_pd3dImmediateContext->PSSetShaderResources(0, 3, pNullSRVs);
				g_pd3dImmediateContext->PSSetSamplers(0, 1, &smRI->sampleState->samplerState);
				g_pd3dImmediateContext->VSSetShader(smRI->vtxShader->vtxShader, NULL, 0 );
                g_pd3dImmediateContext->GSSetShader( NULL, NULL, 0 );
                g_pd3dImmediateContext->PSSetShader(NULL, NULL, 0);
				for (auto si : g_SceneGraph->mp_sceneGraph) {
					if (wcscmp(si.first.c_str(), L"ShadowMap") == 0)  // Shadow Map RenderItem has no Geometry;
						continue;					
					for (auto ri : si.second.renderItemList) {
						setConstants(PassType::SHADOW, si.first);
						ID3D11Buffer* pCBs[2] = { ri->vtxCBuffer->pBff, smRI->vtxCBuffer->pBff };
						g_pd3dImmediateContext->VSSetConstantBuffers(0, 2, pCBs);
						g_pd3dImmediateContext->PSSetConstantBuffers(1, 1, &smRI->pxlCBuffer->pBff);
						g_pd3dImmediateContext->IASetInputLayout(ri->ILO->IPLO);
						g_pd3dImmediateContext->IASetVertexBuffers(0, 1, &si.second.model->vtx,
							&si.second.model->stride,
							&si.second.model->offset);
						g_pd3dImmediateContext->IASetIndexBuffer(ri->idxBuffer->iBuff, DXGI_FORMAT_R32_UINT, 0);
						g_pd3dImmediateContext->DrawIndexed(ri->idxBuffer->faceCount * 3,
							ri->idxBuffer->iBufferStart,
							ri->idxBuffer->vBufferStart);						
					}
				}
                gPass ++;
            }
            break;
        case 1: //  Shadow Pass; Render Depth information to a Texture.
            {
                g_pd3dImmediateContext->OMSetRenderTargets( 1, 
                                    &g_SceneGraph->passItems[0]->viewSet->RenderTargets[0], 
                                    g_SceneGraph->passItems[0]->viewSet->DepthStencilTargets[0] );
    	        g_pd3dImmediateContext->ClearRenderTargetView( g_SceneGraph->passItems[0]->viewSet->RenderTargets[0], g_pfaBackColor );
                g_pd3dImmediateContext->ClearDepthStencilView( g_SceneGraph->passItems[0]->viewSet->DepthStencilTargets[0], D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
                // Set primitive topology
                g_pd3dImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
				for (auto si : g_SceneGraph->mp_sceneGraph) {
					if (wcscmp(si.first.c_str(), L"ShadowMap") == 0)
						continue;
					for (auto ri : si.second.renderItemList) {
						if (ri->effectMask & PassType::SHADOW)
						{
							si.second.model->modelName;
							g_pd3dImmediateContext->RSSetState(ri->rasterState->rasterState);
							g_pd3dImmediateContext->OMSetBlendState(ri->blendState->blendState, 0, 0xffffffff);
							g_pd3dImmediateContext->OMSetDepthStencilState(ri->depthStencilState->depthStencilState, 0);
							setConstants(si.first);
							ID3D11Buffer* pCBs[2] = { ri->vtxCBuffer->pBff, g_fxManager.constantBuffersMap.find(L"WORLD_EF_PER_OBJECT")->second->pBff };
							g_pd3dImmediateContext->VSSetConstantBuffers(0, 2, pCBs);
							g_pd3dImmediateContext->PSSetConstantBuffers(2, 1, &ri->pxlCBuffer->pBff);
							ID3D11ShaderResourceView* pShadowRV = NULL;
							auto rvMapItr = g_txManager.resourceViewsMap.find(L"ShadowMap");
							if (rvMapItr != g_txManager.resourceViewsMap.end())
								pShadowRV = rvMapItr->second->resourceView;
							else
							{
								//error;
							}
							ID3D11ShaderResourceView* pSRVs[2] = { ri->texture->resourceView, pShadowRV };
							g_pd3dImmediateContext->PSSetShaderResources(0, 2, pSRVs);
							ID3D11SamplerState* pSamplerStates[2] = { ri->sampleState->samplerState, g_SceneGraph->passItems[1]->sampleState->samplerState };
							g_pd3dImmediateContext->PSSetSamplers(0, 2, pSamplerStates);
							g_pd3dImmediateContext->IASetInputLayout(ri->ILO->IPLO);
							g_pd3dImmediateContext->IASetVertexBuffers(0, 1, &si.second.model->vtx,
								&si.second.model->stride,
								&si.second.model->offset);
							g_pd3dImmediateContext->IASetIndexBuffer(ri->idxBuffer->iBuff, DXGI_FORMAT_R32_UINT, 0);
							g_pd3dImmediateContext->VSSetShader(ri->vtxShader->vtxShader, NULL, 0);
							g_pd3dImmediateContext->GSSetShader(NULL, NULL, 0);
							g_pd3dImmediateContext->PSSetShader(ri->pxlShader->pxlShader, NULL, 0);
							g_pd3dImmediateContext->DrawIndexed(ri->idxBuffer->faceCount * 3,
								ri->idxBuffer->iBufferStart,
								ri->idxBuffer->vBufferStart);
						}
					}
				}
				gPass ++;
            }
            break;
        case 2: // Solid Pass
            {
                g_pd3dImmediateContext->OMSetRenderTargets( 1, 
                                    &g_SceneGraph->passItems[0]->viewSet->RenderTargets[0], 
                                    g_SceneGraph->passItems[0]->viewSet->DepthStencilTargets[0] );
                // Set primitive topology
                g_pd3dImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
				for (auto si : g_SceneGraph->mp_sceneGraph) {
					if (wcscmp(si.first.c_str(), L"ShadowMap") == 0)
						continue;
					for (auto ri : si.second.renderItemList) {
						if (ri->effectMask & PassType::COLOR_SOLID)
						{
							si.second.model->modelName;
							g_pd3dImmediateContext->RSSetState(ri->rasterState->rasterState);
							g_pd3dImmediateContext->OMSetBlendState(ri->blendState->blendState, 0, 0xffffffff);
							g_pd3dImmediateContext->OMSetDepthStencilState(ri->depthStencilState->depthStencilState, 0);
							setConstants(si.first);
							ID3D11Buffer* pCBs[2] = { ri->vtxCBuffer->pBff, g_fxManager.constantBuffersMap.find(L"WORLD_EF_PER_OBJECT")->second->pBff };
							g_pd3dImmediateContext->VSSetConstantBuffers(0, 2, pCBs);
							g_pd3dImmediateContext->PSSetConstantBuffers(2, 1, &ri->pxlCBuffer->pBff);
							g_pd3dImmediateContext->PSSetShaderResources(0, 1, &ri->texture->resourceView);
							g_pd3dImmediateContext->PSSetSamplers(0, 1, &ri->sampleState->samplerState);
							g_pd3dImmediateContext->IASetInputLayout(ri->ILO->IPLO);
							g_pd3dImmediateContext->IASetVertexBuffers(0, 1, &si.second.model->vtx,
								&si.second.model->stride,
								&si.second.model->offset);
							g_pd3dImmediateContext->IASetIndexBuffer(ri->idxBuffer->iBuff, DXGI_FORMAT_R32_UINT, 0);
							g_pd3dImmediateContext->VSSetShader(ri->vtxShader->vtxShader, NULL, 0);
							g_pd3dImmediateContext->GSSetShader(NULL, NULL, 0);
							g_pd3dImmediateContext->PSSetShader(ri->pxlShader->pxlShader, NULL, 0);
							g_pd3dImmediateContext->DrawIndexed(ri->idxBuffer->faceCount * 3,
								ri->idxBuffer->iBufferStart,
								ri->idxBuffer->vBufferStart);
						}
					}
				}
				gPass ++;
            }
            break;
        case 3: // Transparent Pass
            {
                g_pd3dImmediateContext->OMSetRenderTargets( 1, 
                                    &g_SceneGraph->passItems[0]->viewSet->RenderTargets[0], 
                                    g_SceneGraph->passItems[0]->viewSet->DepthStencilTargets[0] );
                // Set primitive topology
                g_pd3dImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
				for (auto si : g_SceneGraph->mp_sceneGraph) {
					if (wcscmp(si.first.c_str(), L"ShadowMap") == 0)
						continue;
					for (auto ri : si.second.renderItemList) {
						if (ri->effectMask & PassType::COLOR_TRNS)
						{
							si.second.model->modelName;
							g_pd3dImmediateContext->RSSetState(ri->rasterState->rasterState);
							g_pd3dImmediateContext->OMSetBlendState(ri->blendState->blendState, 0, 0xffffffff);
							g_pd3dImmediateContext->OMSetDepthStencilState(ri->depthStencilState->depthStencilState, 0);
							setConstants(si.first);
							ID3D11Buffer* pCBs[2] = { ri->vtxCBuffer->pBff, g_fxManager.constantBuffersMap.find(L"WORLD_EF_PER_OBJECT")->second->pBff };
							g_pd3dImmediateContext->VSSetConstantBuffers(0, 2, pCBs);
							g_pd3dImmediateContext->PSSetConstantBuffers(2, 1, &ri->pxlCBuffer->pBff);
							g_pd3dImmediateContext->PSSetShaderResources(0, 1, &ri->texture->resourceView);
							g_pd3dImmediateContext->PSSetSamplers(0, 1, &ri->sampleState->samplerState);
							g_pd3dImmediateContext->IASetInputLayout(ri->ILO->IPLO);
							g_pd3dImmediateContext->IASetVertexBuffers(0, 1, &si.second.model->vtx,
								&si.second.model->stride,
								&si.second.model->offset);
							g_pd3dImmediateContext->IASetIndexBuffer(ri->idxBuffer->iBuff, DXGI_FORMAT_R32_UINT, 0);
							g_pd3dImmediateContext->VSSetShader(ri->vtxShader->vtxShader, NULL, 0);
							g_pd3dImmediateContext->GSSetShader(NULL, NULL, 0);
							g_pd3dImmediateContext->PSSetShader(ri->pxlShader->pxlShader, NULL, 0);
							g_pd3dImmediateContext->DrawIndexed(ri->idxBuffer->faceCount * 3,
								ri->idxBuffer->iBufferStart,
								ri->idxBuffer->vBufferStart);
						}
					}
				}
				gPass ++;
            }
            break;
        }
        }
        gPass = 0;

    	g_pSwapChain->Present( 0, 0 );

	}   //	while( g_lvContinueRunningQ > 0 )
	g_sbRenderThreadRunning = FALSE;
	return	0;
}

HRESULT CreateSwapChain ()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( g_hWnd, &rc );
    gWindowWidth  = rc.right - rc.left;
    gWindowHeight = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        //D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = sizeof( driverTypes ) / sizeof( driverTypes[0] );

    //Render Target Buffer Description
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width  = gWindowWidth  ;
    sd.BufferDesc.Height = gWindowHeight ;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = g_WindowedMode;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags =0;
    D3D_FEATURE_LEVEL dfl[] = 
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT    numberOfLevels = 1;
    D3D_FEATURE_LEVEL supportedFeature;
    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
      g_driverType = driverTypes[driverTypeIndex];
      hr = D3D11CreateDeviceAndSwapChain( NULL, 
                                          g_driverType, 
                                          NULL, 
                                          NULL,//createDeviceFlags
                                          NULL,//dfl,
                                          0,//numberOfLevels,
                                          D3D11_SDK_VERSION, 
                                          &sd, 
                                          &g_pSwapChain, 
                                          &g_pd3dDevice,
                                          &supportedFeature,
                                          &g_pd3dImmediateContext);
      if( SUCCEEDED( hr ) )
          break;


    }

    return hr;
}

//Create Render View
HRESULT CreateRenderView ()
{
    HRESULT hr = S_OK;
    ID3D11Texture2D* pBackBuffer;

	hr = g_pSwapChain->GetBuffer( 0, 
                                  __uuidof( ID3D11Texture2D ), 
                                  ( LPVOID* )&pBackBuffer );

    
	if( SUCCEEDED( hr ) ) {
		hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, 
                                                   NULL, 
                                                   &g_pRenderTargetView );
		pBackBuffer->Release();
	}
    return hr;
}

// Create Depth Stencil View
HRESULT CreateDepthStencilView()
{
    HRESULT hr = S_OK;
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	
	depthStencilDesc.Width     = gWindowWidth  ;
	depthStencilDesc.Height    = gWindowHeight ;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count   = 1; // multisampling must match
	depthStencilDesc.SampleDesc.Quality = 0; // swap chain values.
	depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0; 
	depthStencilDesc.MiscFlags      = 0;

	if( SUCCEEDED( hr ) ) {
	
        hr = g_pd3dDevice->CreateTexture2D(&depthStencilDesc, 0, &g_pDepthStencilBuffer);
    }
	if( SUCCEEDED( hr ) ) {
       
        hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencilBuffer, 0, &g_pDepthStencilView);
    }
    return hr;
}

// Set up Light
void setupLight(light::Light & lgt) {
	lgt.initLight();
}

// Set up view port
HRESULT SetUpViewPort()
{
    HRESULT hr = S_OK;
    	// Setup the viewport
	
	D3D11_VIEWPORT vp;
    ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width =  gWindowWidth  ;
	vp.Height = gWindowHeight ;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	g_pd3dImmediateContext->RSSetViewports( 1, &vp );
    
    return hr;
}

//View Transforms
void SetViewerTransforms()
{
        g_Camera.initCamera();
		setupLight(g_Light1);
        //g_Light1.setPosition(92.0f, 155.0f, 133.0f );
        //GVECTOR3 lightLookAt( -104.0f, 0.0f, -125.0f );
        //g_Light1.setPosition(-95.95f, 9.08f, -119.32f );  // Ghost Town setting
        g_Light1.setPosition(-72.0f, 30.0f, -160.0f ); // Plane Game Setting
        GVECTOR3 lightLookAt( 0.0f, 0.0f, 0.0f );
        //g_Light1.Look = lightLookAt - g_Light1.Position;
        g_Light1.Look = GVECTOR3( 0.041529290f, -0.39038470f, 0.91971469f );
        g_Light1.Up = GVECTOR3( 0.034566320f, 0.92051911f, 0.38916531f );
        g_Light1.Right = GVECTOR3( 0.99853921f, -0.015629394f, -0.051722672f );
       
		g_Light1.setDirection( g_Light1.Look.x, g_Light1.Look.y, g_Light1.Look.z );
        g_Camera.Position = GVECTOR3(75.0f, 75.0f, 75.0f);
        g_Camera.Look = lightLookAt - g_Camera.Position;;
        //g_Camera.Position = GVECTOR3(-85.0f, 15.0f, -100.0f );
        g_Light1.setLens( 0.25f*G_PI, (float)gWindowWidth/gWindowHeight, g_LCNear, g_LCFar );

        setupLight(g_Light2);
        g_Light2.setDirection(0,-0.5f,-0.1f);


        setupLight(g_Light3);
        g_Light3.setDirection(-0.5f, 0.0f, -0.1f);
        
        GVECTOR3 lookat(0.0f, 0.20f, 1.0f); 
        if (!LH)
        {
            lookat.z = -1.0f;
        }
 
        lookat.Normalize();
        //g_Camera.Look = lookat;
        //g_Camera.Position = GVECTOR3(0.0f, 19.0f, -142.0f);
        //g_Camera.Position = D3DXVECTOR3(0.0f, 0.0f, 6.0f);
        g_Camera.setLens( 0.25f*G_PI, (float)gWindowWidth/gWindowHeight, g_CNear, g_CFar );
}   



/***************************************************************************************************\
Function Name: CleanupDevice

Description:   

Input: 
    NONE

Return: 

\***************************************************************************************************/

void CleanupDevice()
{
    delete g_axScene;
    if( g_Mdl_Plane )
    {
        g_Mdl_Plane->myModel = 0;
        delete g_Mdl_Plane;
    }

	if (g_SceneGraph) 
    {
        g_SceneGraph->cleanup();
        delete g_SceneGraph;
    }

    g_fxManager.Cleanup();
    g_txManager.Cleanup();

    if( g_pd3dImmediateContext ) g_pd3dImmediateContext->ClearState();
    g_pd3dImmediateContext->Flush();

    if( g_pRenderTargetView ) g_pRenderTargetView->Release();
    if( g_WindowedMode == false) {
        g_pSwapChain->SetFullscreenState(FALSE,NULL);
    }
    if( g_pSwapChain ) g_pSwapChain->Release();
    if( g_pd3dDevice ) g_pd3dDevice->Release();
    if( g_pd3dImmediateContext ) g_pd3dImmediateContext->Release();
}

