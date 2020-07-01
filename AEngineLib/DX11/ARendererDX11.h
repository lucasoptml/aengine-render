
#ifndef __ARENDERERDX11_H__
#define __ARENDERERDX11_H__

#include "ARenderer.h"

// forward decl
struct IDXGISwapChain1;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11BlendState;

namespace AEngineLib
{

	class ARendererDX11 : public ARenderer
	{
	public:
		ARendererDX11();

		~ARendererDX11();

		static ARenderer* CreateRendererDX11();

		virtual void BeginFrame();

		virtual void Clear(const AColor& color);

		virtual void EnableDepth();
		virtual void DisableDepth();

		virtual void EnableAlpha();
		virtual void DisableAlpha();

		virtual void Draw(AVertexBuffer* vertexbuffer, AIndexBuffer* indexbuffer, AShader* shader);

		virtual void EndFrame();


	private:
		
		void CreateDevice();

		void CreateWindowResources();
		
		void UpdateWindowSize();
		
		void Present();

		void HandleDeviceLost();

		IDXGISwapChain1*        SwapChain;
		ID3D11RenderTargetView* RenderTargetView;
		ID3D11DepthStencilView* DepthStencilView;
		ID3D11BlendState*       AlphaBlendState;

		static ARenderer* Renderer;

		// in DIPS?
		float ScreenWidthDIPS;
		float ScreenHeightDIPS;

		//DisplayOrientations Orientation;
		//Size				  RenderTargetSize;

	};

}


#endif