//
// AEngineLib.cpp : Defines the exported functions for the DLL application.
//

#include "AEngineLibDX11.h"


namespace AEngineLib
{
	
	ID3D11Device1*        AEngineDX11::Device  = nullptr;
	ID3D11DeviceContext1* AEngineDX11::Context = nullptr;
}