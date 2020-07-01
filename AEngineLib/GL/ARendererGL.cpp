


#include "AEngineLibGL.h"



namespace AEngineLib
{


	ARendererGL::ARendererGL()
	{
	}

	ARendererGL::~ARendererGL()
	{
	}

	ARenderer* ARendererGL::CreateRendererGL()
	{
		ARendererGL* renderer = new ARendererGL();

		// initialize
		renderer->CreateDevice();
		renderer->CreateWindowResources();

		return renderer;
	}
    
    
    void ARendererGL::CheckErrorsGL()
    {
        auto error = glGetError();
        
        if(error != 0)
        {
            printf("GL Error: 0x%x\n", error);
            //ALog::Log("GL Error");
        }
    }

	//
	// Device Related
	//

	void ARendererGL::CreateDevice()
	{

	}

	void ARendererGL::UpdateWindowSize()
	{
        /*
         
		auto window = CoreWindow::GetForCurrentThread();

		if (window->Bounds.Width  != ScreenWidthDIPS ||
			window->Bounds.Height != ScreenHeightDIPS ||
			Orientation != DisplayProperties::CurrentOrientation)
		{
			auto Context = AEngineDX11::GetDeviceContext();

			ID3D11RenderTargetView* nullViews[] = {nullptr};
			Context->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
			RenderTargetView = nullptr;
			DepthStencilView = nullptr;
			Context->Flush();

			CreateWindowResources();
		}
         
        */
	}

	void ARendererGL::Present()
	{
        
        
        /*
        
		// The application may optionally specify "dirty" or "scroll"
		// rects to improve efficiency in certain scenarios.
		DXGI_PRESENT_PARAMETERS parameters = {0};
		parameters.DirtyRectsCount = 0;
		parameters.pDirtyRects = nullptr;
		parameters.pScrollRect = nullptr;
		parameters.pScrollOffset = nullptr;
	
		// The first argument instructs DXGI to block until VSync, putting the application
		// to sleep until the next VSync. This ensures we don't waste any cycles rendering
		// frames that will never be displayed to the screen.
		HRESULT hr = SwapChain->Present1(1, 0, &parameters);

		auto Context = AEngineDX11::GetDeviceContext();

		// Discard the contents of the render target.
		// This is a valid operation only when the existing contents will be entirely
		// overwritten. If dirty or scroll rects are used, this call should be removed.
		Context->DiscardView(RenderTargetView);

		// Discard the contents of the depth stencil.
		Context->DiscardView(DepthStencilView);

		// If the device was removed either by a disconnect or a driver upgrade, we 
		// must recreate all device resources.
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			HandleDeviceLost();
		}
		else
		{
			//DX::ThrowIfFailed(hr);
		}
         
        */
        
	}

	void ARendererGL::CreateWindowResources()
	{
        //GLint m_viewport[4];
        
        //glGetIntegerv( GL_VIEWPORT, m_viewport );

		//ScreenWidth  = (int)m_viewport[2];
		//ScreenHeight = (int)m_viewport[3];


	}

	void ARendererGL::HandleDeviceLost()
	{
		// Reset these member variables to ensure that UpdateForWindowSizeChange recreates all resources.
		
		//Size WindowBounds;

		//WindowBounds.Width = 0;
		//WindowBounds.Height = 0;
		//SwapChain = nullptr;

		CreateDevice();
		UpdateWindowSize();
	}


	//
	// Frame Rendering Related
	//

	void ARendererGL::BeginFrame()
	{
        glFrontFace(GL_CW);
        //glDisable(GL_CULL_FACE);
	}

	void ARendererGL::Clear(const AColor& color)
	{
        GLCALL(glClearColor,( color.r, color.g, color.b, color.a ));

        GLCALL(glClear,(GL_COLOR_BUFFER_BIT));

	}

	void ARendererGL::Draw(AVertexBuffer* vertexbuffer, AIndexBuffer* indexbuffer, AShader* shader)
	{
        AIndexBufferGL* indexgl = static_cast<AIndexBufferGL*>(indexbuffer);
        
		//auto Context = AEngineLib::AEngineDX11::GetDeviceContext();

		//NOTE: would be cleaner to cast these and pass around context.
		//NOTE: may only do cleanup after its ported to multiple platforms (GL + Stage3d + DX11)
		vertexbuffer->Apply();
		indexbuffer->Apply();
        
		//NOTE: this applies the vertex format
        shader->Apply();

		int indexcount     = indexbuffer->GetNumIndices();
        int primitivecount = indexbuffer->GetNumPrimitives();
        
        
        GLCALL(glDrawElements,(GL_TRIANGLES, primitivecount, GL_UNSIGNED_SHORT, 0));



	}

	void ARendererGL::EndFrame()
	{
    
	}

}