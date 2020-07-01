

#ifndef __ATEXTUREDX11_H__
#define __ATEXTUREDX11_H__


#include "ATexture.h"

#include <vector>
#include <string>



struct ID3D11Texture2D;

using std::wstring;


namespace AEngineLib
{
	using std::vector;

	inline wstring StringToWString(string s)
	{
		wstring ws;
		ws.assign(s.begin(),s.end());
		return ws;
	}


	class ATextureDX11 : public ATexture
	{
	public:
		ATextureDX11(string name, ID3D11Texture2D* tex);

		virtual ~ATextureDX11();

		// Create from file.
		//NOTE: add delegate for grabbing files if you want alternative to filename.
		static shared_ptr<ATexture> CreateTextureDX11(string name);

		static shared_ptr<ATexture> CreateTextureDX11(string name, const vector<AByte>& buffer, int width, int height, int components);

		static shared_ptr<ATexture> CreateTextureDX11(string name, const vector<AByte>& filebytes);

		ID3D11Texture2D* GetDX11Texture()
		{
			return Texture;
		}

	private:

		ID3D11Texture2D* Texture;
	};

}


#endif
