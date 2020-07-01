
attribute vec3 Position;
attribute vec2 TexCoord;
attribute vec4 Color;

varying vec2 FragTexCoord;
varying vec4 FragColor;

uniform mat4 ProjMx;

void main()
{
	//float3 WorldPosition = GetWorldPosition(vertex.LocalPosition);
	//float3 ViewPosition  = GetViewPosition(WorldPosition);
	//float4 ProjPosition  = GetProjectionPosition(ViewPosition);

	vec4 LocalPosition = vec4(Position,1);

	vec4 ProjPosition  = LocalPosition * ProjMx;

	gl_Position  = ProjPosition;
	FragTexCoord = TexCoord;
	FragColor    = Color;
    
}
