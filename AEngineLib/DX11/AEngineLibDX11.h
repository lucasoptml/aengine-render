

#ifndef __AENGINELIBWIN8_H__
#define __AENGINELIBWIN8_H__

#include "AEngineLib.h"
#include "AShaderDX11.h"
#include "ATextureDX11.h"
#include "AVertexBufferDX11.h"
#include "AIndexBufferDX11.h"
#include "ARendererDX11.h"

#include <d3d11_1.h>
#include <wrl/client.h>
#include <agile.h>

namespace AEngineLib
{
	using Windows::UI::Core::CoreWindow;
	using Microsoft::WRL::ComPtr;

	class AEngineDX11 : public AEngine
	{
	public:

		static void SetDevice(ID3D11Device1* device)
		{
			Device = device;

			Device->GetImmediateContext1(&Context);
		}

		static ID3D11Device* GetDevice()
		{
			return Device;
		}

		static ID3D11DeviceContext1* GetDeviceContext()
		{
			return Context;
		}

		//static void SetCoreWindow(ComPtr<CoreWindow^> window)
		//{
		//	Window = ComPtr<CoreWindow^>(window);
		//}

		//static ComPtr<CoreWindow^> GetCoreWindow()
		//{
		//	return Window;
		//}

	private:

		//static ComPtr<CoreWindow^> Window;

		static ID3D11Device1*        Device;
		static ID3D11DeviceContext1* Context;
	};


}

#endif
