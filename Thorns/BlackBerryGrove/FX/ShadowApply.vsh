#include ShadowApplyVs.cb

//--------------------------------------------------------------------------------------
// Shader Structs
//--------------------------------------------------------------------------------------

struct VS_INPUT
{
	float4 pos : POSITION;	
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float4 wpos : TEXCOORD0;
	float4 lpos : TEXCOORD1;		//vertex with regard to light view
	float3 normal : NORMAL;  
};




//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	PS_INPUT output;
    output.pos = mul( input.pos, mul( world, viewProj ) );	
	output.wpos = input.pos;
	output.lpos = mul( input.pos, mul( world, lightViewProj ) );
	output.normal = input.normal;
    return output;  
}

