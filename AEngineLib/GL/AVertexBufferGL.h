

#ifndef __AVERTEXBUFFERGL_H__
#define __AVERTEXBUFFERGL_H__


#include "AVertexBuffer.h"

#include <vector>
#include <string>


#include <OpenGLES/ES2/gl.h>


namespace AEngineLib
{
	using std::vector;

	class AVertexBufferGL : public AVertexBuffer
	{
	public:
		AVertexBufferGL(VertexFormats format)
			: 
			AVertexBuffer(format),
            VertexBuffer(0xb33fb33f)
		{
			
			switch(format)
			{
			case VertexFormats::Actor:
				VertexSize = sizeof(ActorVertex);
				break;
			case VertexFormats::Animation:
				VertexSize = sizeof(AnimationVertex);
				break;
			case VertexFormats::Canvas:
				VertexSize = sizeof(CanvasVertex);
				break;
			case VertexFormats::CanvasByte:
				VertexSize = sizeof(CanvasByteVertex);
				break;
			}

		}

		virtual ~AVertexBufferGL();

		// Create from file.
		//NOTE: add delegate for grabbing files if you want alternative to filename.
		static shared_ptr<AVertexBuffer> CreateVertexBufferGL(VertexFormats format);

		virtual void Apply();
		virtual void UpdateBuffer(const void* start, size_t vertexsize, size_t num);

        const vector<char>& GetBufferData()
        {
            return BufferData;
        }
        
	private:

        vector<char>  BufferData;
        
        GLuint        VertexBuffer;

		size_t	      NumVertices;
		size_t        VertexSize;
	};

}


#endif
