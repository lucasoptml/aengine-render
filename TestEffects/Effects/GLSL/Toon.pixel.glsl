
// ------ Include SpaceLightBase Start ------
// ------------------------------------------

//NOTE: works on ps2

//light origin in world space
//manually set


vec3 LightOrigin = vec3(0,-30,-10);

//
// Shared Structs
//


//sampler
SamplerState LightingSampler
{
	Texture = (Lighting);
	AddressU = CLAMP;
	AddressV = CLAMP;
};



//
// PS2+ Lighting Structs
//

struct SpaceLightPS2_Inputs
{
	float2 TexCoord      : TEXCOORD0;
	float3 WorldPosition : TEXCOORD1;
	float3 WorldNormal   : TEXCOORD2;
	float3 LightDir      : TEXCOORD3;
	float3 PointToCam    : TEXCOORD4;
};

struct SpaceLightVS2_Outputs
{
	float2 TexCoord      : TEXCOORD0;
	float3 WorldPosition : TEXCOORD1;
	float3 WorldNormal   : TEXCOORD2;
	float3 LightDir      : TEXCOORD3;
	float3 PointToCam    : TEXCOORD4;
	float4 ProjPosition  : SV_POSITION;
};

SpaceLightVS2_Outputs SpaceLightVS2_Base
(
 float3 LocalPosition,
 float3 LocalNormal,
 float2 TexCoord
 )
{
	//manipulate the world coordinates
	ApplyFatness(LocalPosition,LocalNormal);

	//apply transforms
	float3 WorldNormal   = GetWorldNormal(LocalNormal);
	float3 WorldPosition = GetWorldPosition(LocalPosition);
	float4 ProjPosition  = WorldToProjectionPosition(WorldPosition);

	SpaceLightVS2_Outputs output;
	output.ProjPosition = ProjPosition;
	output.WorldPosition = WorldPosition;
	output.WorldNormal = WorldNormal;
	output.TexCoord = TexCoord;

	output.LightDir   = normalize(WorldPosition - LightOrigin);
	output.PointToCam = normalize(WorldPosition - CameraWorldPosition);

	return output;
}

SpaceLightVS2_Outputs SpaceLightVS2
(
 float3 LocalPosition : POSITION,
 float3 LocalNormal : NORMAL,
 float2 TexCoord : TEXCOORD0
 )
{
	SpaceLightVS2_Outputs output = SpaceLightVS2_Base(LocalPosition,LocalNormal,TexCoord);
	return output;
}


float4 SpaceLightPS2_Base
(
	SpaceLightPS2_Inputs Input
)
{
	float4 Color;

	float4 TexColor = Diffuse.Sample(DiffuseSampler,Input.TexCoord);
	
	float3 WorldNormal = normalize(Input.WorldNormal);
	
	Color = DrawColor;
	Color *= TexColor;

	float2 LookupCoord = 0.5*float2(WorldNormal.x,-WorldNormal.y) + 0.5;

	//NOTE: also know if world normal is back or front facing (from z)
	float3 LookupColor = Lighting.Sample(LightingSampler,LookupCoord).xyz;
	
	Color.rgb *= 2*LookupColor;

	return Color;
}

void SpaceLightPS2(SpaceLightPS2_Inputs Inputs)
{
	vec4 value = SpaceLightPS2_Base(Inputs);

	gl_FragColor = value;
}

// ------------------------------------------
// ------- Include SpaceLightBase End -------

//#ifndef TOONLOOKUP
//	texture toonlookup < string path="Effects/Lookups/toonlookup.png"; >;
//#endif

SamplerState ToonSampler
{
	Texture = (ToonLookup);
	AddressU = CLAMP;
	AddressV = CLAMP;
};



float4 ToonPS2_Base
(
	SpaceLightPS2_Inputs Input
)
{
	float4 color = SpaceLightPS2_Base(Input);

 	float3 PointToCam = Input.PointToCam;

	float  toondot    = dot(PointToCam,Input.WorldNormal);
	
	float4 tooncolor  = ToonLookup.Sample(ToonSampler,-toondot);
	color *= tooncolor;
	
	return color;
}


void main
(
	SpaceLightPS2_Inputs Input
)
{
	vec4 value = ToonPS2_Base(Input);

	gl_FragColor = value;
}

