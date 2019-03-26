// Hugh Smith 
// copyright 2015
// all rights reserved
#pragma once

#include "D3D11.h"
#include "D3DX11tex.h"
#include <vector>
#include <map>

using std::map;
using std::wstring;
using std::vector;

extern ID3D11Device*   g_pd3dDevice;

enum ViewSetType 
{
    BackBuffer,
    ShadowMap,
    Other
};

struct ViewSetItem
{
    vector<ID3D11RenderTargetView*>     RenderTargets;
    vector<ID3D11DepthStencilView*>     DepthStencilTargets;
    vector<ID3D11ShaderResourceView*>   ShaderResourceViews;
	wstring                             viewName;
};


//struct RenderViewItem
//{
//    ID3D11RenderTargetView*   renderTargetView;
//    wstring              rtViewName;
//
//};

//struct DepthStencilViewItem
//{
//    ID3D11DepthStencilView*     pDSView;
//    wstring                dsvName;
//};

struct TextureItem
{
    ID3D11Texture2D*			texture;
	wstring						txName;
};

struct ShaderViewItem 
{
    ID3D11ShaderResourceView*   resourceView;
	wstring						rsrcViewName;
};


class ResourceManager
{
public:
ResourceManager();
~ResourceManager();

ShaderViewItem*             createShaderViewFromTextureFile ( wstring path, DXGI_FORMAT format );
ViewSetItem*                createViewSet( wstring RenderTargetName, ViewSetType vst, int width, int height );
ID3D11ShaderResourceView*   getTextureView ( wstring path );
void                        Cleanup( void );

map< wstring, ViewSetItem * >            viewSetMap;
map< wstring, ShaderViewItem * >         resourceViewsMap;
map< wstring, TextureItem * >            textureMap;


};