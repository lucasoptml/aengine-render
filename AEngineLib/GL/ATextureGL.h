

#ifndef __ATEXTUREGL_H__
#define __ATEXTUREGL_H__


#include "ATexture.h"

#include <vector>
#include <string>


#include <OpenGLES/ES2/gl.h>


namespace ATextureLib
{
	using std::vector;


	class ATextureGL : public ATexture
	{
	public:
		ATextureGL(string name, GLuint texture, bool IsSolid)
			: ATexture(name),
			  Texture(texture),
              bIsSolid(IsSolid)
		{
		}

		virtual ~ATextureGL();

		// Create from file.
		//NOTE: add delegate for grabbing files if you want alternative to filename.
		static shared_ptr<ATexture> CreateTextureGL(string name);

        GLuint GetTextureGL()
        {
            return Texture;
        }
        
		private:

        bool   bIsSolid;
        GLuint Texture;
        
	};

}


#endif
