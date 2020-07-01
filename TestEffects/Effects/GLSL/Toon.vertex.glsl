
// ------ Include SpaceLightBase Start ------
// ------------------------------------------
#include "Global.hlsli"

//NOTE: works on ps2

//light origin in world space
//manually set


static const float3 LightOrigin = float3(0,-30,-10);

//
// Shared Structs
//


//TODO: this should be a manually bound thing (from the level lighting environment)
//texture2D LightEnvironment < string path="effects/lighting/rimonly.png"; >;

//sampler
SamplerState LightingSampler
{
	Texture = (Lighting);
	AddressU = CLAMP;
	AddressV = CLAMP;
};



//
// PS2+ Lighting Structs
//

struct SpaceLightPS2_Inputs
{
	float2 TexCoord      : TEXCOORD0;
	float3 WorldPosition : TEXCOORD1;
	float3 WorldNormal   : TEXCOORD2;
	float3 LightDir      : TEXCOORD3;
	float3 PointToCam    : TEXCOORD4;
};

struct SpaceLightVS2_Outputs
{
	float2 TexCoord      : TEXCOORD0;
	float3 WorldPosition : TEXCOORD1;
	float3 WorldNormal   : TEXCOORD2;
	float3 LightDir      : TEXCOORD3;
	float3 PointToCam    : TEXCOORD4;
	float4 ProjPosition  : SV_POSITION;
};

SpaceLightVS2_Outputs SpaceLightVS2_Base
(
 float3 LocalPosition,
 float3 LocalNormal,
 float2 TexCoord
 )
{
	//manipulate the world coordinates
	ApplyFatness(LocalPosition,LocalNormal);

	//apply transforms
	float3 WorldNormal   = GetWorldNormal(LocalNormal);
	float3 WorldPosition = GetWorldPosition(LocalPosition);
	float4 ProjPosition  = WorldToProjectionPosition(WorldPosition);

	SpaceLightVS2_Outputs output;
	output.ProjPosition = ProjPosition;
	output.WorldPosition = WorldPosition;
	output.WorldNormal = WorldNormal;
	output.TexCoord = TexCoord;

	output.LightDir   = normalize(WorldPosition - LightOrigin);
	output.PointToCam = normalize(WorldPosition - CameraWorldPosition);

	return output;
}

SpaceLightVS2_Outputs SpaceLightVS2
(
 float3 LocalPosition : POSITION,
 float3 LocalNormal : NORMAL,
 float2 TexCoord : TEXCOORD0
 )
{
	SpaceLightVS2_Outputs output = SpaceLightVS2_Base(LocalPosition,LocalNormal,TexCoord);
	return output;
}


float4 SpaceLightPS2_Base
(
	SpaceLightPS2_Inputs Input
)
{
	float4 Color;

	float4 TexColor = Diffuse.Sample(DiffuseSampler,Input.TexCoord);
	
	float3 WorldNormal = normalize(Input.WorldNormal);
	
	Color = DrawColor;
	Color *= TexColor;

	float2 LookupCoord = 0.5*float2(WorldNormal.x,-WorldNormal.y) + 0.5;

	//NOTE: also know if world normal is back or front facing (from z)
	float3 LookupColor = Lighting.Sample(LightingSampler,LookupCoord).xyz;
	
	Color.rgb *= 2*LookupColor;

	return Color;
}

float4 SpaceLightPS2
(
	SpaceLightPS2_Inputs Inputs
)
: COLOR
{
	return SpaceLightPS2_Base(Inputs);
}

// ------------------------------------------
// ------- Include SpaceLightBase End -------


//
// Toon Light Shader
//


SpaceLightVS2_Outputs main
(
	ActorVertex input
)
{
	SpaceLightVS2_Outputs output = SpaceLightVS2_Base(input.LocalPosition,input.LocalNormal,input.TexCoord);
	
	return output;
}
