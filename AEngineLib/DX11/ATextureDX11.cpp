


#include "AEngineLibDX11.h"

#include "DX11/WICTextureLoader.h"

#include <d3d11.h>

#include <vector>

#include <stdio.h>

namespace AEngineLib
{
	using std::vector;

		ATextureDX11::ATextureDX11(string name, ID3D11Texture2D* tex)
			: ATexture(name), 
			  Texture(tex)
		{
			D3D11_TEXTURE2D_DESC desc;

			Texture->GetDesc(&desc);
						
			Width = desc.Width;
			Height = desc.Height;
			bAlpha = true;//TODO: fix this desc.Format == DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;

			
			// assuming its a font, need to change colors
			//if(desc.Format == DXGI_FORMAT_B8G8R8A8_UNORM)
			//{
			//	
			//	auto Context = AEngineDX11::GetDeviceContext();


			//	D3D11_MAPPED_SUBRESOURCE mapresource;

			//	auto result = Context->Map(Texture,1,D3D11_MAP::D3D11_MAP_READ_WRITE,0,&mapresource);

			//	byte* texturepixels = (byte*)mapresource.pData;

			//	//memcpy(locked.pBits,&RGBABuffer[0],RGBABuffer.size());

			//	int pixels = Width*Height;
			//	for(int i = 0; i < pixels; i++)
			//	{
			//		byte& destr = texturepixels[i*4 + 2];
			//		byte& destg = texturepixels[i*4 + 1];
			//		byte& destb = texturepixels[i*4 + 0];
			//		byte& desta = texturepixels[i*4 + 3];

			//		destr = 255;
			//		destg = 255;
			//		destb = 255;
			//		//desta = srca;
			//	}

			//	Context->Unmap(Texture,0);
			//}



		}


	ATextureDX11::~ATextureDX11()
	{
		//TODO: release resources correctly.
	}




	bool get_file_contents(string filename, vector<unsigned char>& contents)
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

	shared_ptr<ATexture> ATextureDX11::CreateTextureDX11(string name)
	{
		vector<byte> texturedata;
		get_file_contents(name, texturedata);

		auto textureptr = CreateTextureDX11(name,texturedata);

		return textureptr;
	}

	shared_ptr<ATexture> ATextureDX11::CreateTextureDX11(string name, const vector<byte>& filebytes)
	{
		auto Device  = AEngineLib::AEngineDX11::GetDevice();
		auto Context = AEngineLib::AEngineDX11::GetDeviceContext();

		ID3D11Resource*           textureresource = nullptr;
		ID3D11ShaderResourceView* resourceview;

		CreateWICTextureFromMemory(Device, Context ,(uint8_t*)&filebytes[0],filebytes.size(),&textureresource,&resourceview);

		auto Texture = static_cast<ID3D11Texture2D*>(textureresource);
		
		auto newtexture = new ATextureDX11(name, Texture);

		auto textureptr = shared_ptr<ATexture>(newtexture);

		return textureptr;
	}

	shared_ptr<ATexture> ATextureDX11::CreateTextureDX11(string name, const vector<byte>& buffer, int width, int height, int components)
	{
		auto Device  = AEngineLib::AEngineDX11::GetDevice();
		auto Context = AEngineLib::AEngineDX11::GetDeviceContext();

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = width;
		desc.Height = height;
		
		if(components == 4)
		{
			desc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		}
		else if(components == 3)
		{
			desc.Format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8X8_UNORM;
		}

		D3D11_SUBRESOURCE_DATA subresource;
		subresource.pSysMem = &buffer[0];
		subresource.SysMemPitch = components * width;
		
		ID3D11Texture2D* texture = nullptr;
		Device->CreateTexture2D(&desc,&subresource,&texture);

		auto newtexture = new ATextureDX11(name, texture);

		auto textureptr = shared_ptr<ATexture>(newtexture);

		return textureptr;
	}

} // end namespace