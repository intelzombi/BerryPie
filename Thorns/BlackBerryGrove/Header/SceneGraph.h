// Hugh Smith 
// copyright 2015
// all rights reserved

#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include "Global.h"
#include "EffectsManager.h"
#include "ResourceManager.h"
#include "ModelManager.h"
#include <vector>
#include <map>
#include <string>

namespace SG {

typedef struct renderItem
{														
    ShaderViewItem              *texture;               
	VertexShaderItem            *vtxShader;             
	PixelShaderItem             *pxlShader;				
	BlendStateItem              *blendState;			
	RasterStateItem             *rasterState;			
	DepthStencilStateItem       *depthStencilState;		
	ConstantBufferItem          *vtxCBuffer;			
	ConstantBufferItem          *pxlCBuffer;			
	SamplerStateItem            *sampleState;			
    InputLayoutItem             *ILO;					
    MdlMgr::ModelIndexBuffer    *idxBuffer;				
    unsigned int                effectMask;				
} RenderItem;

typedef struct framePassItem 
{
    ViewSetItem                 *viewSet;               
    SamplerStateItem            *sampleState;
}PassItem;

class SceneItem {
public:
	SceneItem();
	~SceneItem();
    void cleanup();
    std::vector< RenderItem * >    renderItemList;
    MdlMgr::ModelItem *                 model;
};

class SceneGraph {
public:
    int SceneItemCount;
	SceneGraph(int itemCount);
	~SceneGraph();

	map<wstring, SceneItem> mp_sceneGraph;
    std::vector< PassItem * >   passItems;
    RenderItem InitialzeRenderItem();
    void cleanup();
private:
	SceneGraph(){};
};
};