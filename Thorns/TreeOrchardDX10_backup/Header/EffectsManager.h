#pragma once
#include "d3dx10.h"
#include "Global.h"
#include "stdio.h"
#include "vector3.h"

#include <vector>

enum ILOType{
    P,
    PNT,
    PTNT,
    PVSAT,
    PC
};
struct TEST_CONSTANT_BUFFER
{
    D3DXMATRIX      cbViewProj;      
    D3DXVECTOR4     cbEyePosW;
    D3DXVECTOR4     cbLightDir1W;
    D3DXVECTOR4     cbLightDir2W;
    D3DXVECTOR4     cbLightDir3W;
    D3DXVECTOR3     cbSpec;
    D3DXVECTOR3     cbEl1;
    D3DXVECTOR3     cbEl2;
    D3DXVECTOR3     cbEl3;
    //float           cbmPow;
    //float           fill1;
};

//__declspec(align(16))
struct LIGHTEQ_EF_CONSTANT_BUFFER
{
    D3DXMATRIX      cbViewProj;      
    D3DXVECTOR4     cbEyePosW;
    D3DXVECTOR4     cbLightDir1W;
    D3DXVECTOR4     cbLightDir2W;
    D3DXVECTOR4     cbLightDir3W;
    D3DXVECTOR3     cbSpec;
    D3DXVECTOR3     cbEl1;
    D3DXVECTOR3     cbEl2;
    D3DXVECTOR3     cbEl3;
    float           cbmPow;
    D3DXVECTOR3     fill;
};


struct LIGHTEQ_EF_PS_CONSTANT_BUFFER
{
__declspec(align(16))    D3DXVECTOR3     cbSpec;
__declspec(align(16))    D3DXVECTOR3     cbEl1;
__declspec(align(16))    D3DXVECTOR3     cbEl2;
__declspec(align(16))    D3DXVECTOR3     cbEl3;
    float           cbmPow;
};

__declspec(align(16))
struct LIGHTEQ_EF_VS_CONSTANT_BUFFER
{
    //GMATRIX         cbIDTY;      
    GMATRIX         cbViewProj;      
    D3DXVECTOR4     cbEyePosW;
    D3DXVECTOR4     cbLightDir1W;
    D3DXVECTOR4     cbLightDir2W;
    D3DXVECTOR4     cbLightDir3W;
};




struct PixelShaderItem
{
    ID3D10PixelShader*          pxlShader;
    TCHAR                       entryName[256];
    ID3D10Blob*                 shaderData;
};

struct VertexShaderItem
{
    ID3D10VertexShader*         vtxShader;
    ID3D10Blob*                 shaderData;
    TCHAR                       entryName[256];
};

struct BlendStateItem
{
    D3D10_BLEND_DESC            blendDesc;
    ID3D10BlendState*           blendState;
    TCHAR                       stateName[256];

};

struct RasterStateItem
{
    D3D10_RASTERIZER_DESC       rasterDesc;
    ID3D10RasterizerState*      rasterState;
    TCHAR                       rasterName[256];

};

struct DepthStencilStateItem
{
    D3D10_DEPTH_STENCIL_DESC    DepthStencilDesc;
    ID3D10DepthStencilState*    DepthStencilState;
    TCHAR                       depthStencilName[256];
};

struct ConstantBufferItem 
{
	D3D10_BUFFER_DESC           cbDesc;
	ID3D10Buffer*	            pBff;
	int				            size;
    TCHAR                       constantBufferName[256];
};

struct SamplerStateItem
{
    D3D10_SAMPLER_DESC          samplerDesc;
    ID3D10SamplerState*         samplerState;
    TCHAR                       samplerName[256];

};

struct InputLayoutItem 
{
    ID3D10InputLayout*          IPLO;
    TCHAR                        inputLayoutName[256];
};    

struct effectItem 
{
    ID3D10Effect*               effect;
    TCHAR                       fxName[256];
};

struct edfFileItem
{
    TCHAR path[256];
    TCHAR VSFile[256];
    TCHAR VSFilePath[256];
    TCHAR vsEntryPoint[256];
    char  cVsEntryPoint[256];
    TCHAR vsConstantBuffer[256];
    TCHAR PSFile[256];
    TCHAR PSFilePath[256];
    TCHAR psEntryPoint[256];
    char  cPsEntryPoint[256];
    TCHAR psConstantBuffer[256];
    TCHAR blendState[256];
    TCHAR rasterState[256];
    TCHAR depthStencilState[256];
    TCHAR samplerState[256];
    TCHAR ILO[256];
};

struct effectPacketItem 
{
    VertexShaderItem *      vertexShader;
    PixelShaderItem *       pixelShader;
    BlendStateItem *        blendState;
    RasterStateItem *       rasterState;
    DepthStencilStateItem * depthStencilState;
    ConstantBufferItem *    vsConstantBuffer;
    ConstantBufferItem *    psConstantBuffer;
    SamplerStateItem *      samplerState;
    InputLayoutItem*        inputLayout;

};

class EffectsManager
{
public:
EffectsManager();
~EffectsManager();

bool                    readEDF( TCHAR *path, edfFileItem &edfFileItem );
effectPacketItem*       createEffect (TCHAR *path, int a );
void                    init();
ID3D10Effect*           createEffect (TCHAR *path );
ID3D10Effect*           getEffect( TCHAR *path );
void                    createBlendStates();
void                    createRasterStates();
void                    createSamplerStates();
void                    createDepthStencilStates();
void                    createConstantBuffers();
ID3D10InputLayout*      InitLayouts( ILOType iloType, effectPacketItem *epi );
void                    Cleanup( void );


std::vector< effectItem *>              effectsList;
std::vector< VertexShaderItem *>        vertexShadersList;
std::vector< PixelShaderItem *>         pixelShadersList;
std::vector< BlendStateItem *>          blendStatesList;
std::vector< RasterStateItem *>         rasterStatesList;
std::vector< DepthStencilStateItem *>   depthStencilStatesList;
std::vector< ConstantBufferItem *>      constantBuffersList;
std::vector< SamplerStateItem *>        samplerStatesList;
std::vector< InputLayoutItem*>          inputLayoutsList;

};