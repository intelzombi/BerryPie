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


//struct SHADOW_PS_INPUT
//{
//	float4 pos : SV_POSITION;	
//};



//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

float4 ShadowMapVS( VS_INPUT input ) : SV_POSITION
{
	//SHADOW_PS_INPUT output;
	float4 position = mul( float4 ( input.pos, 1.0f ), lightViewProj );		
	//float4 position = mul( lightViewProj, float4 ( input.pos, 1.0f ) );		
    //position.xyz = position.xyz/position.w;
    //position.z = 1.0f - position.z;
	return position;
    //output.pos = mul( lightViewProj, float4 ( input.pos, 1.0f ) );
    ////output.pos = mul( float4 ( input.pos, 1.0f ), lightViewProj );		
    //return output;  
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


