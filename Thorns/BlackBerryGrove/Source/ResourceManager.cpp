// Hugh Smith 
// copyright 2015
// all rights reserved

#include "ResourceManager.h"

extern IDXGISwapChain* g_pSwapChain;

ResourceManager::ResourceManager()
{
  
}

ViewSetItem*  ResourceManager::createViewSet( wstring ViewSetName, ViewSetType vst, int width, int height )
{
    ID3D11Texture2D* DepthStencilBuffer = NULL;
    ID3D11Texture2D* RenderTarget;
    ID3D11RenderTargetView* RenderTargetView = NULL;
    ID3D11DepthStencilView* DepthStencilView = NULL;
    ID3D11ShaderResourceView* ShaderResourceView = NULL;
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	D3D11_TEXTURE2D_DESC depthStencilDesc;
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    HRESULT hr = S_OK;

	auto mapItr = viewSetMap.find(ViewSetName);
	if (mapItr != viewSetMap.end())
	{
		return mapItr->second;
	}

    switch ( vst ) 
    {
        case ViewSetType::BackBuffer: 
                {
	                hr = g_pSwapChain->GetBuffer( 0, 
                                                  __uuidof( ID3D11Texture2D ), 
                                                  ( LPVOID* )&RenderTarget );
                    if( ! SUCCEEDED( hr ) ){
                        MessageBox( NULL,
			                L"No Render Target retrieved from the SwapChain - bailing out! ResourceManager::createTexture", L"Error", MB_OK );
                        return NULL;
                    }
	                if( SUCCEEDED( hr ) ) {
		                hr = g_pd3dDevice->CreateRenderTargetView( RenderTarget, 
                                                                   NULL, 
                                                                   &RenderTargetView );
                    }
	                depthStencilDesc.Width     = width  ;
	                depthStencilDesc.Height    = height ;
	                depthStencilDesc.MipLevels = 1;
	                depthStencilDesc.ArraySize = 1;
	                depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
	                depthStencilDesc.SampleDesc.Count   = 1; // multisampling must match
	                depthStencilDesc.SampleDesc.Quality = 0; // swap chain values.
	                depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
	                depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
	                depthStencilDesc.CPUAccessFlags = 0; 
	                depthStencilDesc.MiscFlags      = 0;
                    hr = g_pd3dDevice->CreateTexture2D( &depthStencilDesc, 0, &DepthStencilBuffer );
	                if( SUCCEEDED( hr ) ) {
       
                        hr = g_pd3dDevice->CreateDepthStencilView( DepthStencilBuffer, 0, &DepthStencilView );
                    }
               }
                break;
        case ViewSetType::ShadowMap:
                {
                    depthStencilDesc.Width     = width;
                    depthStencilDesc.Height    = height;
                    depthStencilDesc.MipLevels = 1;
                    depthStencilDesc.ArraySize = 1;
                    depthStencilDesc.Format    = DXGI_FORMAT_R32_TYPELESS;
                    depthStencilDesc.SampleDesc.Count   = 1;  
                    depthStencilDesc.SampleDesc.Quality = 0;  
                    depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
                    depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
                    depthStencilDesc.CPUAccessFlags = 0; 
                    depthStencilDesc.MiscFlags      = 0;

                    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
                    dsvDesc.Flags = 0;
	                dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	                dsvDesc.Texture2D.MipSlice = 0;
                    dsvDesc.Texture2DArray.FirstArraySlice = 0;
                    dsvDesc.Texture2DArray.ArraySize = 1;

	                srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	                srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	                srvDesc.Texture2D.MostDetailedMip = 0;
	                srvDesc.Texture2D.MipLevels = depthStencilDesc.MipLevels;
                    srvDesc.Texture2DArray.FirstArraySlice = 0;
                    srvDesc.Texture2DArray.ArraySize = 1;
            
                    hr = g_pd3dDevice->CreateTexture2D( &depthStencilDesc, 0, &DepthStencilBuffer );
	                if( SUCCEEDED( hr ) ) {
       
                        hr = g_pd3dDevice->CreateDepthStencilView( DepthStencilBuffer, &dsvDesc, &DepthStencilView );
                    }
                    if( SUCCEEDED( hr ) ) {
       
                        hr = g_pd3dDevice->CreateShaderResourceView( DepthStencilBuffer, &srvDesc, &ShaderResourceView );
                    }
                }
                break;
        default:
            break;
    }

    ViewSetItem *VSI = new ViewSetItem[1];
	VSI->viewName = ViewSetName;
	VSI->RenderTargets.push_back( RenderTargetView );
    VSI->DepthStencilTargets.push_back( DepthStencilView );
    VSI->ShaderResourceViews.push_back( ShaderResourceView );
    
    ShaderViewItem * svi = new ShaderViewItem();
    svi->resourceView = ShaderResourceView;
	svi->rsrcViewName = ViewSetName;
	resourceViewsMap.insert(std::pair<wstring, ShaderViewItem*>(svi->rsrcViewName, svi));
	viewSetMap.insert(std::pair<wstring, ViewSetItem*>(VSI->viewName, VSI));
    return VSI;

    
}



ResourceManager::~ResourceManager()
{
}


ShaderViewItem* ResourceManager::createShaderViewFromTextureFile (wstring path, DXGI_FORMAT format )
{
    HRESULT hr = S_OK;

    wstring tFile = path.substr(path.rfind(L'\\') +1);

    if ( path.length() > 255 ) 
    {
 		MessageBox( NULL,
			L"Your shadername is greater than 255 characters - bailing out!", L"Error", MB_OK );
        return NULL;

    }

	auto rvMapItr = resourceViewsMap.find(tFile);
	if (rvMapItr != resourceViewsMap.end())
		return rvMapItr->second;

    ID3D11Texture2D* pStagingTexture;

	D3DX11_IMAGE_LOAD_INFO loadInfo;

    loadInfo.Width  = D3DX11_FROM_FILE;
    loadInfo.Height = D3DX11_FROM_FILE;
    loadInfo.Depth  = D3DX11_FROM_FILE;
    loadInfo.FirstMipLevel = 0;
    loadInfo.MipLevels = D3DX11_FROM_FILE;
    loadInfo.Usage = D3D11_USAGE_DEFAULT;
    loadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    loadInfo.CpuAccessFlags = 0;
    loadInfo.MiscFlags = 0;
    loadInfo.Format = format;
    loadInfo.Filter = D3DX11_FILTER_NONE;
    loadInfo.MipFilter = D3DX11_FILTER_NONE;
	loadInfo.pSrcInfo  = 0;

    hr = D3DX11CreateTextureFromFile(g_pd3dDevice, path.c_str(), 
		&loadInfo, 0, (ID3D11Resource**)&pStagingTexture, 0);

    if(hr == D3D11_ERROR_FILE_NOT_FOUND){
        MessageBox( NULL,
			L"Can't find the texture file - bailing out! ResourceManager::createTexture", L"Error", MB_OK );
        return NULL;
    }
    if(hr == D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS){
        MessageBox( NULL,
			L"Too many unique state objects - bailing out! ResourceManager::createTexture", L"Error", MB_OK );
        return NULL;
    }
    if(hr == D3DERR_INVALIDCALL){
        MessageBox( NULL,
			L"Invalid call - bailing out! ResourceManager::createTexture", L"Error", MB_OK );
        return NULL;
    }
    if(hr == D3DERR_WASSTILLDRAWING){
        MessageBox( NULL,
			L"Was still drawing - bailing out! ResourceManager::createTexture", L"Error", MB_OK );
        return NULL;
    }
    if(hr != S_OK){
        MessageBox( NULL,
			L"General Microsoft BS error - bailing out! ResourceManager::createTexture", L"Error", MB_OK );
        return NULL;
    }


	//
	// Create a resource view to the texture array.
	//
    D3D11_RESOURCE_DIMENSION type;
    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;

    pStagingTexture->GetType( &type );

    switch( type )
    {
        case D3D11_RESOURCE_DIMENSION_BUFFER:
        break;

        case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
        break;

        case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
        {
            D3D11_TEXTURE2D_DESC desc;
            pStagingTexture->GetDesc( &desc );
            viewDesc.Format = desc.Format;
            viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            viewDesc.Texture2D.MipLevels = desc.MipLevels;
            viewDesc.Texture2D.MostDetailedMip = desc.MipLevels - 1;
        }
        break;

        case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
        break;
    }
	
	ID3D11ShaderResourceView* texView = 0;
	hr = g_pd3dDevice->CreateShaderResourceView( pStagingTexture, &viewDesc, &texView );

	//
	// Cleanup--we only need the resource view.
	//

    TextureItem* ti = new TextureItem();
    ti->texture = pStagingTexture;
	textureMap.insert(std::pair<wstring, TextureItem*>(tFile, ti));

    if(hr != S_OK){
        MessageBox( NULL,
		    L"CreateShaderResourceView error - bailing out! ResourceManager::createTexture", L"Error", MB_OK );
        return NULL;
    }

    ShaderViewItem *view = new ShaderViewItem();
    view->resourceView = texView;
 
	view->rsrcViewName = tFile;
    
    //resourceViewsList.push_back(view);
	resourceViewsMap.insert(std::pair<wstring, ShaderViewItem*>(view->rsrcViewName, view));

    return view;

}

ID3D11ShaderResourceView* ResourceManager::getTextureView( wstring path )
{
    return NULL;
}

void ResourceManager::Cleanup ( void )
{
	for (map<wstring, ShaderViewItem*>::iterator mapSvItr = resourceViewsMap.begin(); mapSvItr != resourceViewsMap.end(); mapSvItr++)
	{
		if(mapSvItr->second->resourceView)
			mapSvItr->second->resourceView->Release();
		delete mapSvItr->second;
	}
	resourceViewsMap.clear();

   ViewSetItem * vsi = NULL;
	for (auto vsi : viewSetMap)
	{
		
		for (int dsLoop = 0; dsLoop < (int)vsi.second->DepthStencilTargets.size(); dsLoop++)
		{
			if (vsi.second->DepthStencilTargets[dsLoop])
				vsi.second->DepthStencilTargets[dsLoop]->Release();
			vsi.second->DepthStencilTargets[dsLoop] = NULL;
		}
		vsi.second->DepthStencilTargets.clear();
		for (int rtLoop = 0; rtLoop < (int)vsi.second->RenderTargets.size(); rtLoop++)
		{
			if (vsi.second->RenderTargets[rtLoop])
				vsi.second->RenderTargets[rtLoop]->Release();
			vsi.second->RenderTargets[rtLoop] = NULL;
		}
		vsi.second->RenderTargets.clear();

		for (int svLoop = 0; svLoop < (int)vsi.second->ShaderResourceViews.size(); svLoop++)
		{
			if (vsi.second->ShaderResourceViews[svLoop])
				vsi.second->ShaderResourceViews[svLoop]->Release();
			vsi.second->ShaderResourceViews[svLoop] = NULL;
		}
		vsi.second->ShaderResourceViews.clear();

		if (vsi.second)
			delete[]vsi.second;
		vsi.second = NULL;
	}
	viewSetMap.clear();

	for (auto texture : textureMap)
	{
		if (texture.second->texture)
		{
			texture.second->texture->Release();
			delete texture.second;
		}
	}
	textureMap.clear();
}


