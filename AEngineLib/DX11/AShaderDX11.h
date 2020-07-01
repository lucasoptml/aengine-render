#pragma once


#include "AShader.h"

#include <vector>
#include <string>


struct ID3DX11Effect;
struct ID3D11Device;
struct ID3D11Buffer;
struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11ShaderResourceView;
struct ID3D11Texture2D;

using std::wstring;


namespace AShaderLib
{
	using std::vector;

	inline wstring StringToWString(string s)
	{
		wstring ws;
		ws.assign(s.begin(),s.end());
		return ws;
	}


	class AENGINE_API AShaderDX11 : public AShader
	{
	public:
		AShaderDX11(string name, const vector<char>& shaderData, ID3D11VertexShader* vs, ID3D11PixelShader* ps)
			: AShader(name), 
			  VertexShader(vs), 
			  PixelShader(ps), 
			  ShaderData(shaderData)
		{
			GlobalConstantBuffer	= nullptr;
			AnimationConstantBuffer = nullptr;
			ObjectConstantBuffer	= nullptr;
			OptionalConstantBuffer	= nullptr;
			InputLayout				= nullptr;

			for( auto& i : TextureResourceViews )
			{
				i = nullptr;
			}

		}

		virtual ~AShaderDX11();

		static shared_ptr<AShader> CreateShaderDX11(string name, VertexFormats format);

		virtual bool UpdateTexture2D(TextureSlots  slot, shared_ptr<AEngineLib::ATexture> texture);

		virtual bool UpdateGlobalConstants   (const GlobalConstants&    constants);
		virtual bool UpdateObjectConstants   (const ObjectConstants&    constants);
		virtual bool UpdateAnimationConstants(const AnimationConstants& constants);
		virtual bool UpdateOptionalConstants (const OptionalConstants&  constants);

		// apply the effect before rendering
		virtual void Apply();

		static ID3D11Device*        GetDevice();
		static ID3D11DeviceContext* GetDeviceContext();

	private:

		void SetVertexFormat(VertexFormats format);

		ID3D11InputLayout* InputLayout;

		ID3D11ShaderResourceView* TextureResourceViews[4];
		ID3D11Texture2D*          Textures[4];

		static ID3D11Buffer* GlobalConstantBuffer;
		static ID3D11Buffer* ObjectConstantBuffer;
		static ID3D11Buffer* AnimationConstantBuffer;
		static ID3D11Buffer* OptionalConstantBuffer;

		ID3D11VertexShader* VertexShader;
		ID3D11PixelShader*  PixelShader;

		const vector<char> ShaderData;

	};

}
