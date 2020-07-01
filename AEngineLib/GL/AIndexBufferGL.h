

#ifndef __AINDEXBUFFERGL_H__
#define __AINDEXBUFFERGL_H__


#include "AIndexBuffer.h"

#include <vector>
#include <string>


#include <OpenGLES/ES2/gl.h>


namespace AEngineLib
{
	using std::vector;


	class AIndexBufferGL : public AIndexBuffer
	{
	public:
		AIndexBufferGL()
			//: Buffer(nullptr)
		{
		}

		virtual ~AIndexBufferGL();

		// Create from file.
		//NOTE: add delegate for grabbing files if you want alternative to filename.
		static shared_ptr<AIndexBuffer> CreateIndexBufferGL();

		virtual void Apply();
		virtual void UpdateIndexBuffer(const vector<unsigned short>& indices);

        const vector<unsigned short>& GetIndices()
        {
            return Indices;
        }
        
	private:

        GLuint                 IndexBuffer;
        
        vector<unsigned short> Indices;

	};

}


#endif
