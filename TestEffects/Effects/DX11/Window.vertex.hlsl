#include "Global.hlsli"

struct VertexOutput
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
	float4 Color    : COLOR0;
};

VertexOutput main(CanvasVertex vertex)
{
	//float3 WorldPosition = GetWorldPosition(vertex.LocalPosition);
	//float3 ViewPosition  = GetViewPosition(WorldPosition);
	//float4 ProjPosition  = GetProjectionPosition(ViewPosition);

	float4 LocalPosition = float4(vertex.Position,1);

	float4 ProjPosition  = mul(LocalPosition,  ProjMx);

	VertexOutput output;

	output.Position = ProjPosition;
	output.TexCoord = vertex.TexCoord;
	output.Color = vertex.Color;

	return output;
}
