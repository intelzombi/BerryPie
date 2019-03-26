#pragma once

#include "d3dx10.h"
#include <vector>
extern ID3D10Device*   g_pd3dDevice;
struct textureItem 
{
    ID3D10ShaderResourceView*   textureView;
    TCHAR                       txName[256];
};

class TextureManager
{
public:
TextureManager();
~TextureManager();

ID3D10ShaderResourceView*   createTextureView (TCHAR *path, DXGI_FORMAT format );
ID3D10ShaderResourceView*   getTextureView ( TCHAR *path );
void                        Cleanup( void );


std::vector< textureItem *>   texturesList;

};