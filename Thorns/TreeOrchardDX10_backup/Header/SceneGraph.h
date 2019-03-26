#include <d3d10.h>
#include <d3dx10.h>
//#include <d3dcommon.h>

namespace SG {
typedef struct _BufferInfo {
	D3D10_BUFFER_DESC* bd;
	ID3D10Buffer*	pBff;
	int				Count;
	int				BffStart;
}* BufferInfo;

typedef struct _Material {
    int     materialIdx;
    char    materialName[256];
    char    textureFile[256];
}* Material;

typedef struct _Effect {
    //for using D3D10EffectManager
    ID3D10Effect*               effect;
    char                        effectFile[256];
    ID3D10EffectTechnique*      techniqueT;
    char                        techniqueName[256];
    //General State
    D3D10_BLEND_DESC            blendDesc;
    ID3D10BlendState*           blendState;
    D3D10_RASTERIZER_DESC       rasterDesc;
    ID3D10RasterizerState*      rasterState;
    D3D10_DEPTH_STENCIL_DESC    depthStencilDesc;
    ID3D10DepthStencilState*    depthStencilState;

    //vertexShader
    ID3D10VertexShader*         vtxShader;
    D3D10_SAMPLER_DESC          samplerDesc;
    ID3D10SamplerState*         samplerState;
    int                         iLOIdx;
    int                         resourceViewIdx;
    //geometryShader
    //pixelShader    
    ID3D10PixelShader*          pxlShader;
    //D3D10_SAMPLER_DESC          samplerDesc;
    //ID3D10SamplerState*         samplerState;
}* Effect;


class SceneItem {
public:
	SceneItem();
	~SceneItem();
    void cleanup();
	BufferInfo siVBuffers;
	BufferInfo siIBuffers;
    BufferInfo siCBuffers;
	ID3D10InputLayout* pILO;
	ID3D10EffectTechnique* pET;
	ID3D10ShaderResourceView* pRV;
	Effect effect;
	Material material;

};

class SceneGraph {
public:
    int SceneItemCount;
	SceneGraph(int itemCount);
	~SceneGraph();
	SceneItem *m_sceneGraph;
    void cleanup();
private:
	SceneGraph(){};
};
};