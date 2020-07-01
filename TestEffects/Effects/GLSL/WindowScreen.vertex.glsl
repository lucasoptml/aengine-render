

attribute vec3 Position;
attribute vec4 Color;
attribute vec2 TexCoord;

varying vec4 FragColor;
varying vec2 FragTexCoord;

void main()
{
	//float4 ProjPosition  = GetProjectionPosition(LocalPosition);

    FragColor    = Color;
    FragTexCoord = TexCoord;
    
    gl_Position = vec4(Position,1);
}
