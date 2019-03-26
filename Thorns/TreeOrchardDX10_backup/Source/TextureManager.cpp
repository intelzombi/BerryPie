#include "TextureManager.h"



TextureManager::TextureManager()
{
  
}

void TextureManager::Cleanup( void )
{
    for( int loop = 0; loop < (int)texturesList.size() ; loop++)
    {
        if( texturesList[loop]->textureView )
            texturesList[loop]->textureView->Release();

        delete texturesList[loop];
    }
}

TextureManager::~TextureManager()
{
    texturesList.clear();
}


ID3D10ShaderResourceView* TextureManager::createTextureView (TCHAR *path, DXGI_FORMAT format )
{
    HRESULT hr = S_OK;

    wchar_t* bs = NULL;
    bs = wcsrchr(path, L'\\');

    if ( wcslen(bs) > 255 ) 
    {
 		MessageBox( NULL,
			L"Your shadername is greater than 255 characters - bailing out!", L"Error", MB_OK );
        return NULL;

    }

    for( int loop = 0; loop < (int)texturesList.size(); loop++)
    {
        if ( wcscmp( bs+1, texturesList[loop]->txName ) == 0 )
            return texturesList[loop]->textureView;
    }

    ID3D10Texture2D* pStagingTexture;

	D3DX10_IMAGE_LOAD_INFO loadInfo;

    loadInfo.Width  = D3DX10_FROM_FILE;
    loadInfo.Height = D3DX10_FROM_FILE;
    loadInfo.Depth  = D3DX10_FROM_FILE;
    loadInfo.FirstMipLevel = 0;
    loadInfo.MipLevels = D3DX10_FROM_FILE;
    loadInfo.Usage = D3D10_USAGE_DEFAULT;
    loadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    loadInfo.CpuAccessFlags = 0;
    loadInfo.MiscFlags = 0;
    loadInfo.Format = format;
    loadInfo.Filter = D3DX10_FILTER_NONE;
    loadInfo.MipFilter = D3DX10_FILTER_NONE;
	loadInfo.pSrcInfo  = 0;

    hr = D3DX10CreateTextureFromFile(g_pd3dDevice, path, 
		&loadInfo, 0, (ID3D10Resource**)&pStagingTexture, 0);

    if(hr == D3D10_ERROR_FILE_NOT_FOUND){
        MessageBox( NULL,
			L"Can't find the texture file - bailing out! TextureManager::createTexture", L"Error", MB_OK );
        return NULL;
    }
    if(hr == D3D10_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS){
        MessageBox( NULL,
			L"Too many unique state objects - bailing out! TextureManager::createTexture", L"Error", MB_OK );
        return NULL;
    }
    if(hr == D3DERR_INVALIDCALL){
        MessageBox( NULL,
			L"Invalid call - bailing out! TextureManager::createTexture", L"Error", MB_OK );
        return NULL;
    }
    if(hr == D3DERR_WASSTILLDRAWING){
        MessageBox( NULL,
			L"Was still drawing - bailing out! TextureManager::createTexture", L"Error", MB_OK );
        return NULL;
    }
    if(hr != S_OK){
        MessageBox( NULL,
			L"General Microsoft BS error - bailing out! TextureManager::createTexture", L"Error", MB_OK );
        return NULL;
    }


	//
	// Create a resource view to the texture array.
	//
    D3D10_RESOURCE_DIMENSION type;
    D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;

    pStagingTexture->GetType( &type );

    switch( type )
    {
        case D3D10_RESOURCE_DIMENSION_BUFFER:
        break;

        case D3D10_RESOURCE_DIMENSION_TEXTURE1D:
        break;

        case D3D10_RESOURCE_DIMENSION_TEXTURE2D:
        {
            D3D10_TEXTURE2D_DESC desc;
            pStagingTexture->GetDesc( &desc );
            viewDesc.Format = desc.Format;
            viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
            viewDesc.Texture2D.MipLevels = desc.MipLevels;
            viewDesc.Texture2D.MostDetailedMip = desc.MipLevels - 1;
        }
        break;

        case D3D10_RESOURCE_DIMENSION_TEXTURE3D:
        break;
    }
	
	ID3D10ShaderResourceView* texView = 0;
	hr = g_pd3dDevice->CreateShaderResourceView( pStagingTexture, &viewDesc, &texView );

	//
	// Cleanup--we only need the resource view.
	//

    pStagingTexture->Release();

    if(hr != S_OK){
        MessageBox( NULL,
		    L"CreateShaderResourceView error - bailing out! TextureManager::createTexture", L"Error", MB_OK );
        return NULL;
    }

    textureItem *texture = new textureItem();
    texture->textureView = texView;
 

    if (bs)
    {
        wcscpy(texture->txName, bs+1);
    }else{
        wcscpy(texture->txName, path);
    }
    
    texturesList.push_back(texture);

    return texView;

}


ID3D10ShaderResourceView* TextureManager::getTextureView( TCHAR *path )
{
    return NULL;
}



