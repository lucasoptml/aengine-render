
precision mediump float;

varying vec2 FragTexCoord;
varying	vec4 FragColor;

uniform float TimeSeconds;

void main()
{
	//float4 texcolor = Diffuse.Sample(DiffuseSampler,input.TexCoord);

    vec4 texcolor = vec4(1,1,1,1);
		
	vec4 color = FragColor;
		
	color.r = sin(TimeSeconds);
	color.g = cos(TimeSeconds*1.1);
	color.b = sin(TimeSeconds*1.2);

	//premultiplied alpha
	//	texcolor.rgb *= texcolor.a;

	vec4 finalcolor = texcolor * color;

	finalcolor.rgb *= finalcolor.a;

    gl_FragColor = finalcolor;
}
