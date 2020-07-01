//
// AEngineLib.cpp : Defines the exported functions for the DLL application.
//

#include "AEngineLib.h"

#if AENGINE_DX11 == 1
	#include "DX11/AEngineLibDX11.h"
#elif AENGINE_GL == 1
    #include "GL/AEngineLibGL.h"
#endif

namespace AEngineLib
{
	AEngine* AEngine::Engine = nullptr;

	AEngine* AEngine::GetEngine()
	{
		if(!Engine)
		{
#if AENGINE_DX11 == 1
			Engine = new AEngineDX11();
#elif AENGINE_GL == 1
            Engine = new AEngineGL();
#else
			Engine = new AEngine();
#endif
		}

		return Engine;
	}
}
