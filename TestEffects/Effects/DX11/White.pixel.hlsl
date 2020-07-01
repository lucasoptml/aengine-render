#include "Global.hlsli"


struct PixelInput
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
	//float4 Color    : COLOR0;
};

float4 main( PixelInput input ) : SV_TARGET
{
	float4 texcolor = Diffuse.Sample(DiffuseClampSampler,input.TexCoord);

		//premultiplied alpha
		texcolor.rgb *= texcolor.a;

	float4 color = DrawColor;
		
	color.r = sin(TimeSeconds);
	color.g = cos(TimeSeconds*1.1);
	color.b = sin(TimeSeconds*1.2);

	return color*texcolor;
}
