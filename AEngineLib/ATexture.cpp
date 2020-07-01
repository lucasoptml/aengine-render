


#include "ATexture.h"


#if AENGINE_DX11 == 1
    #include "DX11/ATextureDX11.h"
#elif AENGINE_GL == 1
    #include "GL/AEngineLibGL.h"
#endif

// texture loader
#define STBI_NO_DDS
#define STBI_NO_STDIO
#define STBI_NO_HDR

#include "TextureLoader/stb_image_aug.h"
#include "TextureLoader/stb_image_aug.cpp"

#include <algorithm>

namespace AEngineLib
{
	map< string, shared_ptr<ATexture> > ATexture::Textures = map< string, shared_ptr<ATexture> >();
	

	// ToLower with new foreach syntax
	inline void convertToLower(string s)
	{
		string out = s;

		for( char& c : s )
		{
			c = tolower(c);
		};
	}

	shared_ptr<ATexture> ATexture::GetTexture(string name)
	{
		convertToLower(name);

		auto found = Textures.find(name);

		if(found != Textures.end())
		{
			return found->second;
		}

		shared_ptr<ATexture> newresource;

		// Create a new effect given the implementation
#if AENGINE_DX11 == 1
		newresource = ATextureDX11::CreateTextureDX11(name);
#elif AENGINE_GL == 1
        newresource = ATextureGL::CreateTextureGL(name);
#else
		newresource = shared_ptr<ATexture>( new ATexture(name) );
#endif

		if(newresource)
		{
			Textures[name] = newresource;
		}

		return newresource;
	}

	shared_ptr<ATexture> ATexture::CreateTexture(string name, const vector<AByte>& filebytes)
	{
		convertToLower(name);

		auto found = Textures.find(name);

		assert(found == Textures.end());

		shared_ptr<ATexture> newresource;

		// Create a new effect given the implementation
#if AENGINE_DX11 == 1
		newresource = ATextureDX11::CreateTextureDX11(name,filebytes);
#elif AENGINE_GL == 1
        newresource = ATextureGL::CreateTextureGL(name,filebytes);
#else
		newresource = shared_ptr<ATexture>( new ATexture(name) );
#endif

		if(newresource)
		{
			Textures[name] = newresource;
		}

		return newresource;
	}

	shared_ptr<ATexture> ATexture::CreateTexture(string name, const vector<AByte>& buffer, int width, int height, int components)
	{
		convertToLower(name);

		auto found = Textures.find(name);

		assert(found == Textures.end());

		shared_ptr<ATexture> newresource;

		// Create a new effect given the implementation
#if AENGINE_DX11 == 1
		newresource = ATextureDX11::CreateTextureDX11(name,buffer,width,height,components);
#elif AENGINE_GL == 1
        newresource = ATextureGL::CreateTextureGL(name,buffer,width,height,components);
#else
		newresource = shared_ptr<ATexture>( new ATexture(name) );
#endif

		if(newresource)
		{
			Textures[name] = newresource;
		}

		return newresource;
	}


	bool ATexture::LoadTextureRGBA( const vector<AByte>& rgbjpg, const vector<AByte>& ajpeg, vector<AByte>& rgbabuffer, int& x, int& y )
	{
		vector<AByte> rgbbuffer;
		vector<AByte> abuffer;

		int components;

		if(!LoadTexture(rgbjpg,rgbbuffer,x,y,components))
			return false;

		if(components != 3)
			return false;

		if(!LoadTexture(ajpeg,abuffer,x,y,components))
			return false;

		if(components != 1)
			return false;

		// done loading, now
		int size = x*y*4;
		rgbabuffer.resize( size );

		for(int i = 0; i < size/4; i++)
		{
			AByte& destr = rgbabuffer[i*4 + 0];
			AByte& destg = rgbabuffer[i*4 + 1];
			AByte& destb = rgbabuffer[i*4 + 2];
			AByte& desta = rgbabuffer[i*4 + 3];

			AByte srcr = rgbbuffer[i*3 + 0];
			AByte srcg = rgbbuffer[i*3 + 1];
			AByte srcb = rgbbuffer[i*3 + 2];
			AByte srca = abuffer[i];

			destr = srcr;
			destg = srcg;
			destb = srcb;
			desta = srca;
		}

		return true;
	}


	bool ATexture::LoadTexture( const vector<AByte>& texfilebuffer, vector<AByte>& buffer, int& x, int& y, int& components )
	{
		int req_comp = 0;

		AByte* data = (AByte*) stbi_load_from_memory((stbi_uc*)&texfilebuffer[0],(int)texfilebuffer.size(),&x,&y,&components,req_comp);

		if(!data)
		{
			return false;
		}
		else
		{
			//copy into our buffer and free
			size_t buffersize = x*y*components;
		
			buffer.resize(buffersize);

			memcpy(&buffer[0],data,buffersize);

			// done with the internal data, free it.
			free(data);

			return true;
		}

		return false;
	}


}