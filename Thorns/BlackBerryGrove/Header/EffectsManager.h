// Hugh Smith 
// copyright 2015
// all rights reserved

#pragma once

#include <D3D11.h>
#include <D3DX11async.h>
#include "Global.h"
#include "stdio.h"
#include "vector3.h"
#include <vector>
#include <string>
#include <map>

using std::wstring;
using std::string;
using std::vector;
using std::map;

enum ILOType {
    P,
    PNT,
    PTNT,
    PVSAT,
    PC,
    PT
};

enum PassType {
    COLOR_SOLID     = 0x1,
    COLOR_TRNS      = 0x2,
    DEPTH           = 0x4,
    SHADOW          = 0x8,
    SHADOWMAP       = 0x10,
    HDR             = 0x20,
    SSAO            = 0x40,
    NORMAL          = 0x80,
    MSAA            = 0x100,
	BLUR     		= 0x200,	
};

enum CB_LOOKUP {
    LightEQ,
    LIGHTEQ_EF_VS,
    LIGHTEQ_EF_PS,
    WORLD_EF_PER_OBJECT,
    SHADOW_EF_PER_FRAME,
    SHADOW_EF_PER_OBJECT
};

struct NULL_CONSTANT_BUFFER
{
};

//__declspec(align(16))
struct LIGHTEQ_EF_CONSTANT_BUFFER
{
    GMATRIX      cbViewProj;      
    GVECTOR4     cbEyePosW;
    GVECTOR3     cbSpec;
    GVECTOR3     cbEl1;
    GVECTOR3     cbEl2;
    GVECTOR3     cbEl3;
    float        cbmPow;
    GVECTOR3     fill;
};


struct LIGHTEQ_EF_PS_CONSTANT_BUFFER
{
__declspec(align(16))    GVECTOR4     cbLightDir1W;
__declspec(align(16))    GVECTOR4     cbLightDir2W;
__declspec(align(16))    GVECTOR4     cbLightDir3W;
__declspec(align(16))    GVECTOR3     cbSpec;
__declspec(align(16))    GVECTOR3     cbEl1;
__declspec(align(16))    GVECTOR3     cbEl2;
__declspec(align(16))    GVECTOR3     cbEl3;
                         float        cbmPow;
};

__declspec(align(16))
struct LIGHTEQ_EF_VS_CONSTANT_BUFFER
{
__declspec(align(16))   GMATRIX      cbViewProj;
__declspec(align(16))   GMATRIX      cbLightViewProj; 
__declspec(align(16))   GVECTOR4     cbEyePosW;
__declspec(align(16))   GVECTOR3     cbLightPos;
};

__declspec(align(16))
struct PER_ITEM_CONSTANT_BUFFER_B1
{
__declspec(align(16))    GMATRIX     cbWorld;
};

__declspec(align(16))
struct SHADOW_EF_CONSTANT_BUFFER_B0
{
__declspec(align(16))   GMATRIX      cbViewProj;
__declspec(align(16))   GMATRIX      cbLightViewProj; 
__declspec(align(16))   GVECTOR4     cbEyePosW;
__declspec(align(16))   GVECTOR3     cbLightPos;
};

__declspec(align(16))
struct SHADOW_EF_CONSTANT_BUFFER_B1
{
    GMATRIX     cbWorld;
};

struct ComputeShaderItem 
{
    ID3D11ComputeShader*        cmpShader;
	ID3DBlob*					shaderData;
	wstring						entryName;

};

struct PixelShaderItem
{
    ID3D11PixelShader*          pxlShader;
	ID3DBlob*					shaderData;
	wstring                     entryName;
};

struct VertexShaderItem
{
    ID3D11VertexShader*         vtxShader;
    ID3DBlob*					shaderData;
	wstring                     entryName;
};

struct BlendStateItem
{
    D3D11_BLEND_DESC            blendDesc;
    ID3D11BlendState*           blendState;
	wstring                      stateName;

};

struct RasterStateItem
{
    D3D11_RASTERIZER_DESC       rasterDesc;
    ID3D11RasterizerState*      rasterState;
	wstring                     rasterName;

};

struct DepthStencilStateItem
{
    D3D11_DEPTH_STENCIL_DESC    depthStencilDesc;
    ID3D11DepthStencilState*    depthStencilState;
	wstring                     depthStencilName;
};

struct ConstantBufferItem 
{
	D3D11_BUFFER_DESC           cbDesc;
	ID3D11Buffer*	            pBff;
	int				            size;
	wstring                     constantBufferName;
};

struct SamplerStateItem
{
    D3D11_SAMPLER_DESC          samplerDesc;
    ID3D11SamplerState*         samplerState;
	wstring                     samplerName;

};

struct InputLayoutItem 
{
    ID3D11InputLayout*          IPLO;
	wstring                     inputLayoutName;
};


struct edfFileItem
{
	wstring			path;
	wstring			VSFile;
	wstring			VSFilePath;
	wstring			vsEntryPoint;
	string			cVsEntryPoint;
	wstring			vsConstantBuffer;
	wstring			PSFile;
	wstring			PSFilePath;
	wstring			psEntryPoint;
	string			cPsEntryPoint;
	wstring			psConstantBuffer;
	wstring			blendState;
	wstring			rasterState;
	wstring			depthStencilState;
	wstring			samplerState;
	wstring			ILO;
	unsigned int    EffectMask;
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
    unsigned int            effectMask;
};

class EffectsManager
{
public:
EffectsManager();
~EffectsManager();

bool                    readEDF( wstring path, edfFileItem &edfFileItem );
effectPacketItem*       createEffect (wstring path);
void                    init();
void                    createBlendStates();
void                    createRasterStates();
void                    createSamplerStates();
void                    createDepthStencilStates();
void                    createConstantBuffers();
InputLayoutItem*		InitLayouts( ILOType iloType, effectPacketItem *epi );
void                    Cleanup( void );

map< wstring, VertexShaderItem *>        vertexShadersMap;
map< wstring, PixelShaderItem *>         pixelShadersMap;
map< wstring, BlendStateItem *>          blendStatesMap;
map< wstring, RasterStateItem *>         rasterStatesMap;
map< wstring, DepthStencilStateItem *>   depthStencilStatesMap;
map< wstring, ConstantBufferItem *>      constantBuffersMap;
map< wstring, SamplerStateItem *>        samplerStatesMap;
map< wstring, InputLayoutItem*>          inputLayoutsMap;
};