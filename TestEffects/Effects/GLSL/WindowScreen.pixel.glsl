
precision mediump float;


varying vec4 FragColor;
varying vec2 FragTexCoord;


void main()
{
	//float4 texcolor = Diffuse.Sample(WindowSampler,input.TexCoord);

    gl_FragColor = vec4(1,1,1,1);
    //FragColor = Color;
    
	//return texcolor * input.Color;
    //return FragColor;
}
