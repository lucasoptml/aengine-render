
#ifndef GLOBAL_FX
#define GLOBAL_FX

//
// Global Include Header
//

//
// Constant Buffer Types
//

cbuffer GlobalConstants : register(b0)
{
	matrix ViewMx;
	matrix ProjMx;

	// Global Variables
	float TimeSeconds;

	// Camera Related Variables
	float3 CameraWorldForward;
	float3 CameraWorldRight;
	float3 CameraWorldUp;
	float3 CameraWorldPosition;

	float ViewportWidth;
	float ViewportHeight;

	float NearClipping; //used?
	float FarClipping;  //used?
};

cbuffer ObjectConstants : register(b1)
{
	matrix WorldMx;

	float4 DrawColor;
	float  Fatness;
};

cbuffer AnimationConstants : register(b2)
{
	float4x4 SkinPalette[60];
};

cbuffer OptionalConstants : register(b3)
{
	float Factor;
	float Frequency;
};

//
// Vertex Formats
//

struct ActorVertex
{
	float3 LocalPosition : POSITION;
	float3 LocalNormal   : NORMAL;
	float2 TexCoord      : TEXCOORD0;
};

struct CanvasVertex
{
	float3 Position : POSITION;
	float4 Color	: COLOR0;
	float2 TexCoord : TEXCOORD0;
};

struct AnimationVertex
{
	float3 LocalPosition : POSITION;
	float3 LocalNormal   : NORMAL;
	float2 TexCoord      : TEXCOORD0;
	float4 BoneIndices   : COLOR0;
    float4 BoneWeights   : TEXCOORD1;
};

//
// Texture Slots & Samplers
//

// Register 0
Texture2D Diffuse : register(t0);

SamplerState DiffuseSampler : register(s0)
{
	Texture = Diffuse;
	UAddress = Wrap;
	VAddress = Wrap;
};

SamplerState DiffuseClampSampler : register(s0)
{
	Texture = Diffuse;
	UAddress = Clamp;
	VAddress = Clamp;
};

// Register 1
Texture2D Lighting : register(t1);
Texture2D Specular : register(t1);
Texture2D Lookup   : register(t1);

// Register 2
Texture2D ToonLookup   : register(t2);
Texture2D SkyLookup    : register(t2);
Texture2D SpecularMask : register(t2);

// Register 3
Texture2D CloudLookup      : register(t3);
Texture2D ShineTexture     : register(t3);
Texture2D HighlightTexture : register(t3);

//
// Additional Shared Code
//

// Constants
static const float pi     = 3.1415926535897;
static const float halfpi = 1.5707963267948;

//
// Matrix Functions
//

float4x4 GetIdentityMx()
{
	return float4x4(1,0,0,0,
					0,1,0,0,
					0,0,1,0,
					0,0,0,1);
}
	
float4x4 GetTranslationMx(float2 Trans)
{
	float4x4 mx = GetIdentityMx();
	mx[3][0] = Trans.x;
	mx[3][1] = Trans.y;
	return mx;
}

float4x4 GetTranslationMx(float3 Trans)
{
	float4x4 mx = GetIdentityMx();
	mx[3][0] = Trans.x;
	mx[3][1] = Trans.y;
	mx[3][2] = Trans.z;
	return mx;
}

//Rotation about X-axis
float3x3 GetRotationMxX(float angleinrad)
{
	float cosa;
	float sina;
	sincos(angleinrad,sina,cosa);
	
	return float3x3(1,	0,		0,
					0,	cosa,	sina,
					0,	-sina,	cosa);
}

//Rotation about Y-axis
float3x3 GetRotationMxY(float angleinrad)
{
	float cosa;
	float sina;
	sincos(angleinrad,sina,cosa);

	return float3x3(cosa,	0,	-sina,
					0,		1,	0,
					sina,	0,	cosa);
}

//Rotation about Z-axis
float3x3 GetRotationMxZ(float angleinrad)
{
	float cosa;
	float sina;
	sincos(angleinrad,sina,cosa);

	return float3x3(cosa,	sina,	0,
					-sina,	cosa,	0,
					0,		0,		0);
}

//NOTE: Radian/Degree Conversions
//		use degrees() and radians() to convert between

//
// Transform Functions
//

float3 GetWorldPosition(float3 LocalPosition)
{
	return mul(float4(LocalPosition,1), WorldMx).xyz;
}

//local->view
float3 GetViewPosition(float3 LocalPosition, float4x3 WorldViewMx)
{
	return mul(float4(LocalPosition,1), WorldViewMx).xyz;
}

//world->view
float3 GetViewPosition(float3 WorldPosition)
{
	return mul(float4(WorldPosition,1),ViewMx).xyz;
}

//world->projection
float4 WorldToProjectionPosition(float3 WorldPosition)
{
	float4x4 ViewProjMx = mul(ViewMx,ProjMx);
	return mul(float4(WorldPosition,1),ViewProjMx);
}

//local->projection
float4 LocalToProjectionPosition(float3 LocalPosition)
{
	float4x4 WorldViewProjMx = mul(mul(WorldMx,ViewMx),ProjMx);
	return mul(float4(LocalPosition,1),WorldViewProjMx);
}


//local->world
float3 GetWorldNormal(float3 LocalNormal)
{
	return normalize(mul(LocalNormal,(float3x3)WorldMx));
}

float3 GetViewNormal(float3 WorldNormal)
{
	return normalize(mul(WorldNormal,(float3x3)ViewMx));
}

//view->proj
float4 GetProjectionPosition(float3 ViewPosition)
{
	return mul(float4(ViewPosition, 1), ProjMx);
}

//
// Special Matrix Functions
//

float ProjToViewZ(float ProjZ)
{
	float nearz = NearClipping;
	float farz = FarClipping;
	
	float invdifference = 1.0 / (nearz - farz);
	float translation = (nearz*farz) * invdifference;
	float scaling = farz * invdifference;
	
	//return (ProjZ - translation)/scaling;
	return ProjZ/(-1.0 + scaling);
}

//convert view z to typical projection z
float ViewToProjZ(float ViewZ)
{
	float nearz = NearClipping;
	float farz = FarClipping;
	
	float invdifference = 1.0 / (nearz - farz);
	float translation = (nearz*farz) * invdifference;
	float scaling = farz * invdifference;
	
	//return (ViewZ * scaling) + translation;
	return -ViewZ + (ViewZ * scaling) / translation;// + translation;
}

//
// Sprite Functions
//

float GetSpriteScaling(float3 ViewPosition)
{
	//actual sprite scaling equation:
	//viewportheight*projmx_22/(projmx_43*z)
 	//NOTE: we use yy value because ratio is not encoded onto it.
 	float yy = ProjMx._22;
 	float wz = ProjMx._43;
 	float distscale = yy/(wz*ViewPosition.z);
 	float scale = distscale*ViewportHeight;
 	return scale;
}

//
// Actor Functions
//

void ApplyFatness(inout float3 LocalPosition, float3 LocalNormal)
{
	LocalPosition += LocalNormal * Fatness;
}

//
// Texturing Functions
//

//
// Lighting Functions : most of these must be included externally?
//

//
// Math Functions
//

float fastsqrt(float x)
{
	return x*rsqrt(x);
}

//
// Macros
//

#define COLOR(R,G,B,A) float4(R/255.f,G/255.f,B/255.f,A/255.f);

// cull modes

#define NoCullMode None

//viewer specific
#ifdef OPENGINE

	#define SolidCullMode CW
	#define InverseCullMode CCW
#else
	#define SolidCullMode CCW
	#define InverseCullMode CW

#endif


#endif