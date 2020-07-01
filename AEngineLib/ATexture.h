
#ifndef __ATEXTURE_H__
#define __ATEXTURE_H__


#include <memory>
#include <string>
#include <map>
#include <vector>


namespace AEngineLib
{
	using std::string;
	using std::shared_ptr;
	using std::map;
	using std::vector;

	typedef unsigned char AByte;

	class AENGINE_API ATexture
	{
	protected:
		ATexture(string name)
			: Name(name)
		{
		}

	public:

		// Create texture, load from filesystem
		static shared_ptr<ATexture> GetTexture(string texturename);

		// Create texture from file bytes
		static shared_ptr<ATexture> CreateTexture(string texturename, const vector<AByte>& buffer, int width, int height, int components);
		
		// Create texture from pixel buffer
		static shared_ptr<ATexture> CreateTexture(string texturename, const vector<AByte>& filebuffer);

		int GetWidth()
		{
			return Width;
		}
	
		int GetHeight()
		{
			return Height;
		}

		bool HasAlpha()
		{
			return bAlpha;
		}

	protected:

		static bool LoadTextureRGBA( const vector<AByte>& rgbfile, const vector<AByte>& afile, vector<AByte>& rgbabuffer, int& x, int& y );
		static bool LoadTexture( const vector<AByte>& texfilebuffer, vector<AByte>& buffer, int& x, int& y, int& components );
				
		unsigned int Width;
		unsigned int Height;
		bool         bAlpha;

	private:

		string Name;
		
		// List of loaded shaders
		static map< string, shared_ptr<ATexture> > Textures;

	};

}


#endif