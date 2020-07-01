
precision mediump float;

attribute float3 Position;
attribute float4 Color;
attribute float2 TexCoord;

varying float4 FragPosition;
varying float4 FragColor;
varying float2 FragTexCoord;


void main()
{
	//float4 texcolor = Diffuse.Sample(WindowSampler,input.TexCoord);

	//return texcolor * input.Color;
    gl_FragColor = FragColor;
}
