//------
// File:  LightEQ.vsh
//
//--------------

//----- Constants----------
#include "LightEQvs.cb"

//	float4x4 gViewProj;		//: packoffset(c1);
//	float4 gEyePosW	  ;	//: packoffset(c5);     
//	float4 gLightDir1W;		//: packoffset(c6);     
//	float4 gLightDir2W;		//: packoffset(c7);     
//	float4 gLightDir3W;		//: packoffset(c8);     



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
    float3 lt1Dir : TEXTURE3;
    float3 lt2Dir : TEXTURE4;
    float3 lt3Dir : TEXTURE5;
    float4 color : COLOR0;
};

//-------------------
// VS
//-------------------
PSPointIn lightEQVS( VSPointIn input)
{
	PSPointIn vOut;
	float4x4 WVP = gViewProj;
	vOut.pos = mul( float4(input.pos,1.0f), WVP );
	//vOut.pos = mul( WVP, float4(input.pos,1.0f) );
	vOut.vPos = vOut.pos.xyz;
	vOut.tex   = input.tex;
	vOut.lt1Dir = gLightDir1W.xyz;
	vOut.lt2Dir = gLightDir2W.xyz;
	vOut.lt3Dir = gLightDir3W.xyz;
	float4 LightColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	
	// simple lambert
	vOut.norm = input.norm;
	vOut.color = LightColor;
	vOut.ePos  = mul(gEyePosW,WVP).xyz;
	return vOut;
}
