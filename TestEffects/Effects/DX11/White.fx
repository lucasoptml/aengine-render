
matrix World;
matrix View;
matrix Projection;

struct PS_INPUT
{
    float4 Pos   : SV_POSITION;
	float4 Color : COLOR0;
};

PS_INPUT VS( float3 Position : POSITION, float3 Color : COLOR0 )
{
    PS_INPUT psInput;

	float4 Pos = float4(Position,1);

    Pos = mul( Pos, World );
    Pos = mul( Pos, View );
	Pos = mul( Pos, Projection );

    psInput.Pos   = Pos;
    psInput.Color = float4(Color,1);

    return psInput;
}

float4 PS( PS_INPUT psInput ) : SV_Target
{
    return psInput.Color;
}

technique11 PS2
{
	pass P0
	{
		SetVertexShader(  CompileShader( vs_4_0_level_9_3 , VS()) );
		SetPixelShader(   CompileShader( ps_4_0_level_9_3 , PS()) );
	}
}

