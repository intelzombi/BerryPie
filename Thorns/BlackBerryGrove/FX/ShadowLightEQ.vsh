//------
// File:  LightEQ.vsh
//
//--------------

//----- Constants----------
#include "LightEQvs.cb"



struct VSPointIn
{
    float3 pos   : POSITION;
    float3 norm  : NORMAL;
    float2 tex   : TEXCOORD;
};

struct PSPointIn
{
    float4 pos   : SV_Position;
    float3 norm  : NORMAL;    
    float2 tex   : TEXTURE0;
    float3 vPos  : TEXTURE1;
	float3 ePos  : TEXTURE2;
	float4 lpos  : TEXTURE3;		
    float4 color : COLOR0;
};

//-------------------
// VS
//-------------------
PSPointIn ShadowLightEQVS( VSPointIn input)
{
	PSPointIn vOut;
	float4x4 WVP = viewProj;
	vOut.pos  = mul( float4(input.pos,1.0f), WVP );
	vOut.lpos = mul( float4(input.pos,1.0f), lightViewProj );	
	vOut.vPos = vOut.pos.xyz;
	vOut.tex   = input.tex;
	float4 LightColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	// simple lambert
	vOut.norm = mul( input.norm, (float3x3) world );
	vOut.norm = normalize( vOut.norm );
	vOut.ePos  = mul(eyePosW,WVP).xyz;
	return vOut;
}
