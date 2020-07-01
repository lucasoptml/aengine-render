

// Uniform Inputs
uniform mat4 ViewMx;
uniform mat4 ProjMx;

uniform float TimeSeconds;

uniform	mat4 WorldMx;
uniform vec4 DrawColor;

uniform float  Fatness;

// Vertex Attributes (Vertex Inputs)
attribute vec3 LocalPosition;
attribute vec3 LocalNormal;
attribute vec2 TexCoord;

// Vertex Output
//varying vec4 FragPosition;
varying vec2 FragTexCoord;


void main()
{
	vec4 LocalPosition4 = vec4(LocalPosition,1);

    vec4 WorldPosition = WorldMx * LocalPosition4;
    vec4 ViewPosition  = ViewMx  * WorldPosition;
    vec4 ProjPosition  = ProjMx  * ViewPosition;

	gl_Position  = ProjPosition;
	FragTexCoord = TexCoord;
}


