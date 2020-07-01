#include "Global.hlsli"

struct WindowVertexOutput
{
	float4 Position : SV_POSITION;
	float4 Color    : COLOR0;
	float2 TexCoord : TEXCOORD0;
};

WindowVertexOutput 

main(
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
