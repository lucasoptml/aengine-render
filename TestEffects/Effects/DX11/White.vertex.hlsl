#include "Global.hlsli"

struct VertexOutput
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
	//float4 Color    : COLOR0;
};

VertexOutput main(ActorVertex vertex)
{
	//float3 WorldPosition = GetWorldPosition(vertex.LocalPosition);
	//float3 ViewPosition  = GetViewPosition(WorldPosition);
	//float4 ProjPosition  = GetProjectionPosition(ViewPosition);

	float4 LocalPosition = float4(vertex.LocalPosition,1);

	float4 WorldPosition = mul(LocalPosition, WorldMx);
	float4 ViewPosition  = mul(WorldPosition, ViewMx);
	float4 ProjPosition  = mul(ViewPosition,  ProjMx);

	VertexOutput output;

	output.Position = ProjPosition;
	output.TexCoord = vertex.TexCoord;

	return output;
}
