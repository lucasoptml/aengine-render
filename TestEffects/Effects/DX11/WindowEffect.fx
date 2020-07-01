#include "Global.fx"

//
// Window Effect File
//

// Windows just setup fixed function states for now
// internally they'll alter alpha and coloring states only

SamplerState WindowSampler
{
	Texture   = Diffuse;
	//MipFilter = POINT;
	//MinFilter = LINEAR;
	//MagFilter = LINEAR;
	AddressU  = CLAMP; 
	AddressV  = CLAMP;
};

struct WindowVertexOutput
{

	float4 Position : SV_POSITION;

	float4 Color    : COLOR0;
	float2 TexCoord : TEXCOORD0;
};

struct WindowPixelInput
{

	float4 Position : SV_POSITION;	
	float4 Color    : COLOR0;
	float2 TexCoord : TEXCOORD0;


};

WindowVertexOutput 
WindowVertexShader(
	float3 LocalPosition : POSITION,
	float4 Color         : COLOR0, 
	float2 TexCoord      : TEXCOORD0)
{
	float4 ProjPosition  = GetProjectionPosition(LocalPosition);

	WindowVertexOutput output;
	output.Position = ProjPosition;
	output.TexCoord = TexCoord;
	output.Color    = Color;

	return output;
}


float4 WindowPixelShader( WindowPixelInput input ) : SV_TARGET
{
	float4 texcolor = tex2D(WindowSampler,input.TexCoord);

	return texcolor * input.Color;
}


technique11 PS2
{
	pass P0
	{
		SetVertexShader(  CompileShader( vs_4_0_level_9_1 , WindowVertexShader()) );
		SetGeometryShader( NULL );
		SetPixelShader(   CompileShader( ps_4_0_level_9_1 , WindowPixelShader()) );
	}
}
