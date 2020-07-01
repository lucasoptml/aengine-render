

attribute float4 Position;
attribute float4 Color;
attribute float2 TexCoord;

varying float4 FragColor;
varying float2 FragTexCoord;

main()
{
	vec4 ProjPosition  = ProjMx * LocalPosition;//GetProjectionPosition(LocalPosition);

	//WindowVertexOutput output;
	//output.Position = ProjPosition;
	//output.TexCoord = TexCoord;
	//output.Color    = Color;

    gl_Position  = ProjPosition;
    
    FragColor    = Color;
    FragTexCoord = TexCoord;
}
