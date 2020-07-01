#include "Global.hlsli"



struct LogoVS1_Outputs
{
	float2 TexCoord     : TEXCOORD0;
	float2 TexCoord1    : TEXCOORD1;
	float2 TexCoord2	: TEXCOORD2;	
	float4 ProjPosition : SV_POSITION;

};

LogoVS1_Outputs main
(
	ActorVertex input
)
{

	float3 WorldPosition = GetWorldPosition(input.LocalPosition);

	//apply transforms
	float4 ProjPosition = WorldToProjectionPosition(WorldPosition);
	
	LogoVS1_Outputs output;
	
	output.ProjPosition = ProjPosition;
	output.TexCoord     = float2(WorldPosition.x - TimeSeconds*0.3,WorldPosition.y)*0.25;
	output.TexCoord1    = input.TexCoord;
	output.TexCoord2	= input.TexCoord;
	
	return output;
}
