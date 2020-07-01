#include "Global.hlsli"


struct PixelInput
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
	float4 Color    : COLOR0;
};

float4 main( PixelInput input ) : SV_TARGET
{
	float4 texcolor = Diffuse.Sample(DiffuseSampler,input.TexCoord);

		
	float4 color = input.Color;
		
	color.r = sin(TimeSeconds);
	color.g = cos(TimeSeconds*1.1);
	color.b = sin(TimeSeconds*1.2);

	//premultiplied alpha
	//	texcolor.rgb *= texcolor.a;

	float4 finalcolor = texcolor * color;

	finalcolor.rgb *= finalcolor.a;

	return finalcolor;
}
