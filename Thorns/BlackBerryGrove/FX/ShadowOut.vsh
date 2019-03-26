#include "LightEQVs.cb"

//--------------------------------------------------------------------------------------
// Shader Structs
//--------------------------------------------------------------------------------------

struct VS_INPUT
{
	float3 pos : POSITION;	
	float2 uv : TEXCOORD;
};


struct SHADOW_PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;	
};



//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

SHADOW_PS_INPUT ShadowOutVS( VS_INPUT input )
{
	SHADOW_PS_INPUT output;
    //output.pos = mul( viewProj , float4 ( input.pos, 1.0f ) );		
    output.pos = mul( float4 ( input.pos, 1.0f ) , viewProj );		
	output.uv  = input.uv;
    return output;  
}



