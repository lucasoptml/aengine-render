


#include "ARenderer.h"

#if AENGINE_DX11 == 1
	#include "DX11/ARendererDX11.h"
#elif AENGINE_GL == 1
    #include "GL/ARendererGL.h"
#endif

	
namespace AEngineLib
{
	ARenderer* ARenderer::Renderer = nullptr;
    
    
	ARenderer* ARenderer::GetRenderer()
	{
		if(Renderer)
		{
			return Renderer;
		}
        
		// Create a new effect given the implementation
#if AENGINE_DX11 == 1
		Renderer = ARendererDX11::CreateRendererDX11();
#elif AENGINE_GL == 1
        Renderer = ARendererGL::CreateRendererGL();
#else
		Renderer = new ARenderer();
#endif
        
		return Renderer;
	}

}