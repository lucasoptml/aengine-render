


#include "AEngineLibGL.h"

#include <vector>

#include <stdio.h>

namespace AEngineLib
{
	using std::vector;

	AVertexBufferGL::~AVertexBufferGL()
	{
		//TODO: release resources correctly.
	}
    
	//
	// AVertexBuffer
	//
    
	shared_ptr<AVertexBuffer> AVertexBufferGL::CreateVertexBufferGL(VertexFormats format)
	{
		auto newbuffer = new AVertexBufferGL(format);

		auto ptr = shared_ptr<AVertexBuffer>(newbuffer);

		return ptr;
	}

	void AVertexBufferGL::Apply()
	{
        GLCALL(glBindBuffer,(GL_ARRAY_BUFFER, VertexBuffer));

	}

	void AVertexBufferGL::UpdateBuffer(const void* start, size_t vertsize, size_t num)
	{
		//NOTE: should assert on vertsize different than VertexSize
      
        NumVertices = num;
        
        auto memsize = vertsize * num;
        
		//NumIndices = indices.size();
        GLCALL(glGenBuffers,(1, &VertexBuffer));
        
        GLCALL(glBindBuffer,(GL_ARRAY_BUFFER, VertexBuffer));
      
        GLCALL(glBufferData,(GL_ARRAY_BUFFER, memsize, start, GL_STATIC_DRAW));
    }

} // end namespace