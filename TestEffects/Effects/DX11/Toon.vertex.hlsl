
#include "SpaceLightBase.hlsli"

//
// Toon Light Shader
//



SpaceLightVS2_Outputs main
(
	ActorVertex input
)
{
	SpaceLightVS2_Outputs output = SpaceLightVS2_Base(input.LocalPosition,input.LocalNormal,input.TexCoord);
	
	return output;
}



