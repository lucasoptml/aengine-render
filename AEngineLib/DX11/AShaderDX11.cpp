


#include "AEngineLibDX11.h"

#include <d3d11.h>

#include <stdio.h>

namespace AShaderLib
{
	ID3D11Buffer* AShaderDX11::GlobalConstantBuffer = nullptr;
	ID3D11Buffer* AShaderDX11::ObjectConstantBuffer = nullptr;
	ID3D11Buffer* AShaderDX11::AnimationConstantBuffer = nullptr;
	ID3D11Buffer* AShaderDX11::OptionalConstantBuffer = nullptr;


	ID3D11Device* AShaderDX11::GetDevice()
	{
		return AEngineLib::AEngineDX11::GetDevice();
	}

	ID3D11DeviceContext* AShaderDX11::GetDeviceContext()
	{
		return AEngineLib::AEngineDX11::GetDeviceContext();
	}

	AShaderDX11::~AShaderDX11()
	{
		//TODO: release resources correctly.
	}




	bool get_file_contents(string filename, vector<char>& contents)
	{
	  std::FILE *fp;
	  fopen_s(&fp,filename.c_str(), "rb");
  
	  if (fp)
	  {
		std::fseek(fp, 0, SEEK_END);
		contents.resize(std::ftell(fp));
		std::rewind(fp);
		std::fread(&contents[0], 1, contents.size(), fp);
		std::fclose(fp);
    
		return true;
	  }

	  return false;
	}

	//
	// opShader
	//

	shared_ptr<AShader> AShaderDX11::CreateShaderDX11(string name, VertexFormats format)
	{
		auto pixelfile  = name + ".pixel.cso";
		auto vertexfile = name + ".vertex.cso";

		//1. load vertex file into a buffer
		//2. load pixel file into a buffer
		vector<char> pixelshaderdata;
		get_file_contents(pixelfile, pixelshaderdata);

		vector<char> vertexshaderdata;
		get_file_contents(vertexfile, vertexshaderdata);

		ID3D11PixelShader*  pixelshader;
		ID3D11VertexShader* vertexshader;

		auto Device = GetDevice();

		Device->CreatePixelShader(  &pixelshaderdata[0],  pixelshaderdata.size(),  nullptr, &pixelshader );
		Device->CreateVertexShader( &vertexshaderdata[0], vertexshaderdata.size(), nullptr, &vertexshader );

		auto shader = new AShaderDX11( name, vertexshaderdata, vertexshader, pixelshader );

		shader->SetVertexFormat(format);

		auto shaderptr = shared_ptr<AShader>(shader);

		return shaderptr;
	}

	bool AShaderDX11::UpdateGlobalConstants(const GlobalConstants& constants)
	{
		// if the global constant buffer is not created, create it.
		
		if(!GlobalConstantBuffer)
		{
			CD3D11_BUFFER_DESC desc( sizeof(GlobalConstants), D3D11_BIND_CONSTANT_BUFFER);
		


			auto result = GetDevice()->CreateBuffer(&desc,nullptr,&GlobalConstantBuffer);
		}

		auto DeviceContext = GetDeviceContext();

		DeviceContext->UpdateSubresource(GlobalConstantBuffer,0,nullptr,&constants,0,0);

		return true;
	}

	bool AShaderDX11::UpdateObjectConstants(const ObjectConstants& constants)
	{
		// if the global constant buffer is not created, create it.
		
		if(!ObjectConstantBuffer)
		{
			CD3D11_BUFFER_DESC desc( sizeof(ObjectConstants), D3D11_BIND_CONSTANT_BUFFER);
			
			auto result = GetDevice()->CreateBuffer(&desc,nullptr,&ObjectConstantBuffer);
		}

		auto DeviceContext = GetDeviceContext();

		DeviceContext->UpdateSubresource(ObjectConstantBuffer,0,nullptr,&constants,0,0);

		return true;
	}


	bool AShaderDX11::UpdateAnimationConstants(const AnimationConstants& constants)
	{
		// if the global constant buffer is not created, create it.
		
		if(!AnimationConstantBuffer)
		{
			CD3D11_BUFFER_DESC desc( sizeof(AnimationConstants), D3D11_BIND_CONSTANT_BUFFER);
			
			auto result = GetDevice()->CreateBuffer(&desc,nullptr,&AnimationConstantBuffer);
		}

		auto DeviceContext = GetDeviceContext();

		DeviceContext->UpdateSubresource(AnimationConstantBuffer,0,nullptr,&constants,0,0);

		return true;
	}

	bool AShaderDX11::UpdateOptionalConstants(const OptionalConstants& constants)
	{
		// if the global constant buffer is not created, create it.
		
		if(!OptionalConstantBuffer)
		{
			CD3D11_BUFFER_DESC desc( sizeof(OptionalConstants), D3D11_BIND_CONSTANT_BUFFER);
			
			auto result = GetDevice()->CreateBuffer(&desc,nullptr,&OptionalConstantBuffer);
		}

		auto DeviceContext = GetDeviceContext();

		DeviceContext->UpdateSubresource(OptionalConstantBuffer,0,nullptr,&constants,0,0);

		return true;
	}

	void AShaderDX11::Apply()
	{
		auto Context = GetDeviceContext();
		
		Context->IASetInputLayout(InputLayout);
		
		Context->PSSetShader(PixelShader,  nullptr, 0);
		Context->VSSetShader(VertexShader, nullptr, 0);

		if(GlobalConstantBuffer)
		{
			Context->VSSetConstantBuffers(0,1,&GlobalConstantBuffer);
			Context->PSSetConstantBuffers(0,1,&GlobalConstantBuffer);
		}

		if(ObjectConstantBuffer)
		{
			Context->VSSetConstantBuffers(1,1,&ObjectConstantBuffer);
			Context->PSSetConstantBuffers(1,1,&ObjectConstantBuffer);
		}

		if(AnimationConstantBuffer)
		{
			Context->VSSetConstantBuffers(2,1,&AnimationConstantBuffer);
			Context->PSSetConstantBuffers(2,1,&AnimationConstantBuffer);
		}

		if(OptionalConstantBuffer)
		{
			Context->VSSetConstantBuffers(3,1,&OptionalConstantBuffer);
			Context->PSSetConstantBuffers(3,1,&OptionalConstantBuffer);
		}

	}

	void AShaderDX11::SetVertexFormat(VertexFormats format)
	{
		auto Device = GetDevice();

		switch(format)
		{
			case VertexFormats::Actor:
			{
				const D3D11_INPUT_ELEMENT_DESC vertexDesc[] = 
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 3*4, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 6*4, D3D11_INPUT_PER_VERTEX_DATA, 0 }
				};

				Device->CreateInputLayout(vertexDesc,3,&ShaderData[0],ShaderData.size(),&InputLayout);

			}
			break;
			case VertexFormats::Animation:
			{
				const D3D11_INPUT_ELEMENT_DESC vertexDesc[] = 
				{
					{ "POSITION",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,    D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "NORMAL",       0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 3*4,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD",     0, DXGI_FORMAT_R32G32_FLOAT,       0, 6*4,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "BLENDINDICES", 0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, 8*4,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "BLENDWEIGHT",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 9*4,  D3D11_INPUT_PER_VERTEX_DATA, 0 }
				};

				Device->CreateInputLayout(vertexDesc,5,&ShaderData[0],ShaderData.size(),&InputLayout);

			}
			break;
			case VertexFormats::Canvas:
			{
				const D3D11_INPUT_ELEMENT_DESC vertexDesc[] = 
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 3*4, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 7*4, D3D11_INPUT_PER_VERTEX_DATA, 0 }
				};

				Device->CreateInputLayout(vertexDesc,3,&ShaderData[0],ShaderData.size(),&InputLayout);

			}
			break;
			case VertexFormats::CanvasByte:
			{
				const D3D11_INPUT_ELEMENT_DESC vertexDesc[] = 
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, 3*4, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 4*4, D3D11_INPUT_PER_VERTEX_DATA, 0 }
				};

				Device->CreateInputLayout(vertexDesc,3,&ShaderData[0],ShaderData.size(),&InputLayout);

			}
			break;
		}
	}

	bool AShaderDX11::UpdateTexture2D( TextureSlots slot, shared_ptr<AEngineLib::ATexture> texture )
	{
		int index = 0;

		switch(slot)
		{
			case TextureSlots::Diffuse:
				index = 0;
				break;
			case TextureSlots::Lighting:
			case TextureSlots::Specular:
			case TextureSlots::Lookup:
				index = 1;
				break;
			case TextureSlots::ToonLookup:
			case TextureSlots::SkyLookup:
			case TextureSlots::SpecularMask:
				index = 2;
				break;
			case TextureSlots::CloudLighting:
			case TextureSlots::ShineTexture:
			case TextureSlots::HighlightTexture:
				index = 3;
				break;
		}


		{
			//NOTE: can assume its this
			//NOTE: easier would be to add an #ifdef around a ToTextureDX11() function.
			auto TextureDX11 = static_cast<AEngineLib::ATextureDX11*>( texture.get() );
			auto TextureResource = TextureDX11->GetDX11Texture();

			//NOTE: need to cleanup memory.

			ID3D11ShaderResourceView*& ResourceView = TextureResourceViews[index];
			ID3D11Texture2D*           OldTexture   = Textures[index];

			if(!ResourceView || TextureResource != OldTexture )
			{
				if(ResourceView)
				{
					ResourceView->Release();
					ResourceView = nullptr;
				}

				D3D11_TEXTURE2D_DESC texturedesc;
				TextureResource->GetDesc(&texturedesc);

				D3D11_SHADER_RESOURCE_VIEW_DESC desc;
				desc.Texture2D.MipLevels       = texturedesc.MipLevels;
				desc.Texture2D.MostDetailedMip = 0;//texturedesc.MipLevels - 1;
				desc.Format                    = texturedesc.Format;
				desc.ViewDimension             = D3D_SRV_DIMENSION_TEXTURE2D;

				auto result = GetDevice()->CreateShaderResourceView(TextureResource,&desc,&ResourceView);

				Textures[index] = TextureResource;
			}

			auto DeviceContext = GetDeviceContext();
			DeviceContext->PSSetShaderResources(index,1,&ResourceView);
		} 
		
		return true;

	}


} // end namespace