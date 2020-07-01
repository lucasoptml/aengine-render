
#include "SpaceLightBase.hlsli"

//
// Toon Light Shader
//

// description: this uses the typical toon lighting, but
//				also does a stylized dot calculation/lookup
//				this shader may be smooth, unlike the others though

//
// Shared Structures
//

//#ifndef TOONLOOKUP
//	texture toonlookup < string path="Effects/Lookups/toonlookup.png"; >;
//#endif

SamplerState ToonSampler
{
	Texture = (ToonLookup);
	AddressU = CLAMP;
	AddressV = CLAMP;
};



float4 ToonPS2_Base
(
	SpaceLightPS2_Inputs Input
)
{
	float4 color = SpaceLightPS2_Base(Input);

 	float3 PointToCam = Input.PointToCam;

	float  toondot    = dot(PointToCam,Input.WorldNormal);
	
	float4 tooncolor  = ToonLookup.Sample(ToonSampler,-toondot);
	color *= tooncolor;
	
	return color;
}


float4 main
(
	SpaceLightPS2_Inputs Input
)
: SV_TARGET
{
	return ToonPS2_Base(Input);
}

