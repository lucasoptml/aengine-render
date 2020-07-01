
#include "Global.hlsli"


SamplerState BorderSampler : register(s2)
{
	Texture = (SpecularMask);
};

SamplerState LookupSampler : register(s1)
{
	Texture = (Lookup);
};


float4 main
(
    float2 TexCoord  : TEXCOORD0,
	float2 TexCoord1 : TEXCOORD1,
	float2 TexCoord2 : TEXCOORD2
)
: SV_TARGET
{
	//NOTE: probably need a lookup
	//		how does it work?
	//		1. lookup gives you a value between 0 & 1
	//		2. lerp between DrawColor and 1 using it?
	
	float4 lookup = Lookup      .Sample( LookupSampler, TexCoord  );

	//return lookup;

	float4 border = SpecularMask.Sample( BorderSampler, TexCoord1 );
	float4 middle = Diffuse     .Sample( DiffuseSampler,TexCoord2 );

	//return middle;

	float4 color = float4(lookup.rgb,border.a);
	color.rgb    = lerp(color,DrawColor,middle.a);

	return color;
}

