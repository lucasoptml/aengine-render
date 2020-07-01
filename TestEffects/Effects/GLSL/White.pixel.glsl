
precision mediump float;

uniform vec4  DrawColor;
uniform float TimeSeconds;

//varying vec4 FragPosition;
varying vec2 FragTexCoord;

void main()
{
	//float4 texcolor = Diffuse.Sample(DiffuseClampSampler,input.TexCoord);

    //premultiplied alpha
	//	texcolor.rgb *= texcolor.a;

	vec4 color = DrawColor;
		
	color.r = sin(TimeSeconds);
	color.g = cos(TimeSeconds * 1.1);
	color.b = sin(TimeSeconds * 1.2);

    gl_FragColor = vec4(1,1,1,1);//color;
    
	//return color;// *texcolor;
    
}
