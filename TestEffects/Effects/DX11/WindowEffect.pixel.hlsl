#include "Global.hlsli"


SamplerState WindowSampler
{
	Texture   = Diffuse;
	AddressU  = CLAMP; 
	AddressV  = CLAMP;
};

struct WindowPixelInput
{
	float4 Position : SV_POSITION;	
	float4 Color    : COLOR0;
	float2 TexCoord : TEXCOORD0;
};

float4 main( WindowPixelInput input ) : SV_TARGET
{
	float4 texcolor = Diffuse.Sample(WindowSampler,input.TexCoord);

	return texcolor * input.Color;
}
