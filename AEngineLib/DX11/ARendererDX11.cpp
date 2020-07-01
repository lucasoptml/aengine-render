


#include "AEngineLibDX11.h"

#ifdef AENGINE_DX11
	#include "DX11/ARendererDX11.h"
#endif

#include <wrl/client.h>
#include <dxgi.h>
#include <d3d11_1.h>

#include <DirectXMath.h>
//#include <memory>
//#include <agile.h>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

namespace AEngineLib
{

	XMFLOAT4X4 orientationTransform3D;
	
	DisplayOrientations Orientation;


	ARendererDX11::ARendererDX11()
	{
	}

	ARendererDX11::~ARendererDX11()
	{
	}

	ARenderer* ARendererDX11::CreateRendererDX11()
	{
		ARendererDX11* renderer = new ARendererDX11();

		// initialize
		renderer->CreateDevice();
		renderer->CreateWindowResources();

		return renderer;
	}

	//
	// Device Related
	//

	void ARendererDX11::CreateDevice()
	{
		// This flag adds support for surfaces with a different color channel ordering
		// than the API default. It is required for compatibility with Direct2D.
		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	#if defined(_DEBUG)
		// If the project is in a debug build, enable debugging via SDK Layers with this flag.
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

		// This array defines the set of DirectX hardware feature levels this app will support.
		// Note the ordering should be preserved.
		// Don't forget to declare your application's minimum required feature level in its
		// description.  All applications are assumed to support 9.1 unless otherwise stated.
		D3D_FEATURE_LEVEL featureLevels[] = 
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		D3D_FEATURE_LEVEL featurelevel;

		// Create the Direct3D 11 API device object and a corresponding context.
		ID3D11Device* device;
		ID3D11DeviceContext* context;
		auto result = 
			D3D11CreateDevice(
				nullptr, // Specify nullptr to use the default adapter.
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				creationFlags, // Set set debug and Direct2D compatibility flags.
				featureLevels, // List of feature levels this app can support.
				ARRAYSIZE(featureLevels),
				D3D11_SDK_VERSION, // Always set this to D3D11_SDK_VERSION for Windows Store apps.
				&device, // Returns the Direct3D device created.
				&featurelevel, // Returns feature level of device created.
				&context // Returns the device immediate context.
				);

		ID3D11Device1* device1;

		device->QueryInterface (__uuidof (ID3D11Device1), (void **) &device1);

		AEngineDX11::SetDevice(device1);

	}

	void ARendererDX11::UpdateWindowSize()
	{
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
	}

	void ARendererDX11::Present()
	{
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
	}

	void ARendererDX11::CreateWindowResources()
	{
		// Store the window bounds so the next time we get a SizeChanged event we can
		// avoid rebuilding everything if the size is identical.
		CoreWindow^ window = CoreWindow::GetForCurrentThread();

		auto Device = AEngineDX11::GetDevice();

		auto bounds = window->Bounds;

		ScreenWidthDIPS  = bounds.Width;
		ScreenHeightDIPS = bounds.Height;

		auto ConvertDipsToPixels = [](float dips)
		{
			static const float dipsPerInch = 96.0f;
			return floor(dips * DisplayProperties::LogicalDpi / dipsPerInch + 0.5f); // Round to nearest integer.
		};

		// Calculate the necessary swap chain and render target size in pixels.
		float windowWidth  = ConvertDipsToPixels(bounds.Width);
		float windowHeight = ConvertDipsToPixels(bounds.Height);

		ScreenWidth = (int)windowWidth;
		ScreenHeight = (int)windowHeight;

		// The width and height of the swap chain must be based on the window's
		// landscape-oriented width and height. If the window is in a portrait
		// orientation, the dimensions must be reversed.
		Orientation = DisplayProperties::CurrentOrientation;
		
		bool swapDimensions =
			Orientation == DisplayOrientations::Portrait ||
			Orientation == DisplayOrientations::PortraitFlipped;
	
		Size RenderTargetSize;

		RenderTargetSize.Width = swapDimensions ? windowHeight : windowWidth;
		RenderTargetSize.Height = swapDimensions ? windowWidth : windowHeight;

		if(SwapChain != nullptr)
		{
			// If the swap chain already exists, resize it.
	//		DX::ThrowIfFailed(
				SwapChain->ResizeBuffers(
					2, // Double-buffered swap chain.
					static_cast<UINT>(RenderTargetSize.Width),
					static_cast<UINT>(RenderTargetSize.Height),
					DXGI_FORMAT_B8G8R8A8_UNORM,
					0
					);
		//		);
		}
		else
		{
			// Otherwise, create a new one using the same adapter as the existing Direct3D device.
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
			swapChainDesc.Width = static_cast<UINT>(RenderTargetSize.Width); // Match the size of the window.
			swapChainDesc.Height = static_cast<UINT>(RenderTargetSize.Height);
			swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // This is the most common swap chain format.
			swapChainDesc.Stereo = false;
			swapChainDesc.SampleDesc.Count = 1; // Don't use multi-sampling.
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 2; // Use double-buffering to minimize latency.
			swapChainDesc.Scaling = DXGI_SCALING_NONE;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // All Windows Store apps must use this SwapEffect.
			swapChainDesc.Flags = 0;

			IDXGIDevice1*  dxgiDevice;

			//TODO: need to fix this conversion!
			//NOTE: should probably just use these ComPtr's if its the best practice.

			Device->QueryInterface (__uuidof (IDXGIDevice1), (void **) &dxgiDevice);

			//DX::ThrowIfFailed(
			//	m_d3dDevice.As(&dxgiDevice)
			//	);


			ComPtr<IDXGIAdapter> dxgiAdapter;
			//DX::ThrowIfFailed(
				dxgiDevice->GetAdapter(&dxgiAdapter);
			//	);

			ComPtr<IDXGIFactory2> dxgiFactory;
			//DX::ThrowIfFailed(
				dxgiAdapter->GetParent(
					__uuidof(IDXGIFactory2), 
					&dxgiFactory
					);
			//	);



			//Windows::UI::Core::CoreWindow^ window = window.Get();
			//DX::ThrowIfFailed(
				dxgiFactory->CreateSwapChainForCoreWindow(
					Device,
					reinterpret_cast<IUnknown*>(window),
					&swapChainDesc,
					nullptr, // Allow on all displays.
					&SwapChain
					);
			//	);
			
			// Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and
			// ensures that the application will only render after each VSync, minimizing power consumption.
			//DX::ThrowIfFailed(
				dxgiDevice->SetMaximumFrameLatency(1);
			//	);
		}
	
		// Set the proper orientation for the swap chain, and generate the
		// 3D matrix transformation for rendering to the rotated swap chain.
		DXGI_MODE_ROTATION rotation = DXGI_MODE_ROTATION_UNSPECIFIED;
		switch (Orientation)
		{
			case DisplayOrientations::Landscape:
				rotation = DXGI_MODE_ROTATION_IDENTITY;
				orientationTransform3D = XMFLOAT4X4( // 0-degree Z-rotation
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
					);
				break;

			case DisplayOrientations::Portrait:
				rotation = DXGI_MODE_ROTATION_ROTATE270;
				orientationTransform3D = XMFLOAT4X4( // 90-degree Z-rotation
					0.0f, 1.0f, 0.0f, 0.0f,
					-1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
					);
				break;

			case DisplayOrientations::LandscapeFlipped:
				rotation = DXGI_MODE_ROTATION_ROTATE180;
				orientationTransform3D = XMFLOAT4X4( // 180-degree Z-rotation
					-1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, -1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
					);
				break;

			case DisplayOrientations::PortraitFlipped:
				rotation = DXGI_MODE_ROTATION_ROTATE90;
				orientationTransform3D = XMFLOAT4X4( // 270-degree Z-rotation
					0.0f, -1.0f, 0.0f, 0.0f,
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
					);
				break;

			default:
				throw ref new Platform::FailureException();
		}

		//DX::ThrowIfFailed(
			SwapChain->SetRotation(rotation);
		//	);

		// Create a render target view of the swap chain back buffer.
		ComPtr<ID3D11Texture2D> backBuffer;
		//DX::ThrowIfFailed(
			SwapChain->GetBuffer(
				0,
				__uuidof(ID3D11Texture2D),
				&backBuffer
				);
		//	);

		//DX::ThrowIfFailed(
			Device->CreateRenderTargetView(
				backBuffer.Get(),
				nullptr,
				&RenderTargetView
				);
		//	);

		// Create a depth stencil view.
		CD3D11_TEXTURE2D_DESC depthStencilDesc(
			DXGI_FORMAT_D24_UNORM_S8_UINT, 
			static_cast<UINT>(RenderTargetSize.Width),
			static_cast<UINT>(RenderTargetSize.Height),
			1,
			1,
			D3D11_BIND_DEPTH_STENCIL
			);

		ComPtr<ID3D11Texture2D> depthStencil;
		//DX::ThrowIfFailed(
			Device->CreateTexture2D(
				&depthStencilDesc,
				nullptr,
				&depthStencil
				);
		//	);

		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
		//DX::ThrowIfFailed(
			Device->CreateDepthStencilView(
				depthStencil.Get(),
				&depthStencilViewDesc,
				&DepthStencilView
				);
		//	);

		// Set the rendering viewport to target the entire window.
		CD3D11_VIEWPORT viewport(
			0.0f,
			0.0f,
			RenderTargetSize.Width,
			RenderTargetSize.Height
			);

		auto Context = AEngineDX11::GetDeviceContext();

		Context->RSSetViewports(1, &viewport);

		//
		// Setup Alpha Blend State
		//

		CD3D11_BLEND_DESC desc;
		ZeroMemory(&desc,sizeof(desc));
		
		auto& Target = desc.RenderTarget[0];
		
		Target.BlendEnable = true;
		Target.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		Target.SrcBlend  = D3D11_BLEND_ONE;
		Target.BlendOp   = D3D11_BLEND_OP_ADD;
		Target.BlendOpAlpha   = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		Target.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		Target.SrcBlendAlpha  = D3D11_BLEND::D3D11_BLEND_ONE;
		Target.RenderTargetWriteMask = 0xf;

		Device->CreateBlendState(&desc,&AlphaBlendState);


	}

	void ARendererDX11::HandleDeviceLost()
	{
		// Reset these member variables to ensure that UpdateForWindowSizeChange recreates all resources.
		
		Size WindowBounds;

		WindowBounds.Width = 0;
		WindowBounds.Height = 0;
		SwapChain = nullptr;

		CreateDevice();
		UpdateWindowSize();
	}


	//
	// Frame Rendering Related
	//

	void ARendererDX11::BeginFrame()
	{
		auto Context = AEngineDX11::GetDeviceContext();

		EnableDepth();


	}

	void ARendererDX11::EnableDepth()
	{
		auto Context = AEngineDX11::GetDeviceContext();

		Context->OMSetRenderTargets(1,&RenderTargetView,DepthStencilView);
	}

	void ARendererDX11::DisableDepth()
	{
		auto Context = AEngineDX11::GetDeviceContext();

		Context->OMSetRenderTargets(1,&RenderTargetView,nullptr);

	}

	void ARendererDX11::EnableAlpha()
	{
		// Enables for premultiplied alpha blending.
		auto Context = AEngineDX11::GetDeviceContext();


		Context->OMSetBlendState(AlphaBlendState,nullptr,0xffffffff);
	}

	void ARendererDX11::DisableAlpha()
	{
		auto Context = AEngineDX11::GetDeviceContext();

		Context->OMSetBlendState(nullptr,nullptr,0xffffffff);
	}

	void ARendererDX11::Clear(const AColor& color)
	{
		auto Context = AEngineDX11::GetDeviceContext();

		float colors[4] = 
		{ 
			color.r, 
			color.g, 
			color.b, 
			color.a 
		};

		Context->ClearRenderTargetView(
			RenderTargetView,
			colors
			);

		Context->ClearDepthStencilView(
			DepthStencilView,
			D3D11_CLEAR_DEPTH,
			1.0f,
			0
			);
	}



	void ARendererDX11::Draw(AVertexBuffer* vertexbuffer, AIndexBuffer* indexbuffer, AShader* shader)
	{
		auto Context = AEngineLib::AEngineDX11::GetDeviceContext();

		//NOTE: would be cleaner to cast these and pass around context.
		//NOTE: may only do cleanup after its ported to multiple platforms (GL + Stage3d + DX11)
		vertexbuffer->Apply();
		indexbuffer->Apply();
		shader->Apply();

		int indexcount = indexbuffer->GetNumIndices();

		Context->DrawIndexed(indexcount,0,0);
	}

	void ARendererDX11::EndFrame()
	{
		Present();
	}

}