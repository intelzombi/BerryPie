#include "LightEQVs.cb"

//--------------------------------------------------------------------------------------
// Shader Structs
//--------------------------------------------------------------------------------------

struct VS_INPUT
{
	float3 pos : POSITION;	
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};


struct SHADOW_PS_INPUT
{
	float4 pos : SV_POSITION;
	float d    : depth;	
};



//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

SHADOW_PS_INPUT ShadowMapVSLinear( VS_INPUT input ) 
{
	SHADOW_PS_INPUT output;	
	//float4 position = mul( lightViewProj, float4 ( input.pos, 1.0f ) );
	float4 position = mul( float4 ( input.pos, 1.0f ), lightViewProj );
    //output.d = (position.z + (lightViewProj[3][2]/lightViewProj[2][2])) / ((-lightViewProj[3][2] / (1 - lightViewProj[2][2])) - (lightViewProj[3][2]/lightViewProj[2][2]));
	position.z = position.z * position.w;
	output.pos = position;

    return output;  
}






//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
//technique10 ShadowMapRenderPassFrontFaces
//{
//    pass P0
//    {
//        SetVertexShader( CompileShader( vs_4_0, ShadowMapVS() ) );
//        SetGeometryShader( NULL );
//        SetPixelShader( CompileShader( ps_4_0, ShadowMapPS() ) );
//		SetRasterizerState( backFaceCulling );		
//    }
//}


