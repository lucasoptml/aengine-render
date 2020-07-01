
#ifndef __ARENDERERGL_H__
#define __ARENDERERGL_H__

#include "ARenderer.h"



namespace AEngineLib
{

	class ARendererGL : public ARenderer
	{
	public:
		ARendererGL();

		~ARendererGL();

		static ARenderer* CreateRendererGL();

        static void CheckErrorsGL();
        
		virtual void BeginFrame();

		virtual void Clear(const AColor& color);

		virtual void Draw(AVertexBuffer* vertexbuffer, AIndexBuffer* indexbuffer, AShader* shader);

		virtual void EndFrame();

        
        
	private:
		
		void CreateDevice();

		void CreateWindowResources();
		
		void UpdateWindowSize();
		
		void Present();

		void HandleDeviceLost();


		//static ARenderer* Renderer;

		// in DIPS?
		//float ScreenWidthDIPS;
		//float ScreenHeightDIPS;

		//DisplayOrientations Orientation;
		//Size				RenderTargetSize;

	};

}


#endif