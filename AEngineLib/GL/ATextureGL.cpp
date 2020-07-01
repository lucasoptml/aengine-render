


#include "AEngineLibGL.h"

#include <OpenGLES/ES2/gl.h>

#include <vector>

#include <stdio.h>



namespace ATextureLib
{
	using std::vector;
    using AEngineLib::AEngineGL;
    using AEngineLib::ARendererGL;
    
	ATextureGL::~ATextureGL()
	{
		//TODO: release resources correctly.
	}


	bool get_file_contents(string filename, vector<byte>& contents)
	{
	  std::FILE *fp;
	  fp = fopen(filename.c_str(), "rb");
  
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

	shared_ptr<ATexture> ATextureGL::CreateTextureGL(string name)
	{
        
        auto bundlepath = AEngineGL::GetResourcePath();
        
        auto newname = bundlepath + name;
        
        vector<byte> file;
        get_file_contents( newname, file );

        int components;
        int x;
        int y;
        vector<byte> buffer;
        
        if(!LoadTexture( file, buffer, x, y, components ))
        {
            throw;
        }
        
        void* pixels = &buffer[0];
        
        GLuint texture;
        
        //GLint internalformat;
        GLint format;
        //GLint type;
        
        if(components == 3)
        {
            format = GL_RGB;
        }
        else if(components == 4)
        {
            format = GL_RGBA;
        }
        
        bool bIsSolid = components == 3;
        
        GLCALL(glGenTextures,( 1, &texture ));
        
        GLCALL(glBindTexture,( GL_TEXTURE_2D, texture ));

        GLCALL(glTexImage2D,( GL_TEXTURE_2D, 0, format, x, y, 0, format, GL_UNSIGNED_BYTE, pixels ));
        
		auto newtexture = new ATextureGL( name, texture, bIsSolid );

		auto textureptr = shared_ptr<ATexture>( newtexture );

		return textureptr;
	}
    
} // end namespace


