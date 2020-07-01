


#include "AEngineLibGL.h"

#include <OpenGLES/ES2/gl.h>

#include <vector>

#include <stdio.h>

namespace AEngineLib
{
	using std::vector;

	AIndexBufferGL::~AIndexBufferGL()
	{
		//TODO: release resources correctly.
	}



	//
	// AVertexBuffer
	//

	shared_ptr<AIndexBuffer> AIndexBufferGL::CreateIndexBufferGL()
	{
		auto newbuffer = new AIndexBufferGL();

		auto ptr = shared_ptr<AIndexBuffer>(newbuffer);

		return ptr;
	}


	void AIndexBufferGL::Apply()
	{
        GLCALL(glBindBuffer,(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer));
	}

	void AIndexBufferGL::UpdateIndexBuffer(const vector<unsigned short>& newindices)
	{
		//NOTE: should assert on vertsize different than VertexSize

        auto indexptr = &newindices[0];
        
        auto memsize = sizeof(unsigned short) * newindices.size();
        
        GLCALL(glGenBuffers,(1, &IndexBuffer));

        GLCALL(glBindBuffer,(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer));

        GLCALL(glBufferData,(GL_ELEMENT_ARRAY_BUFFER, memsize, indexptr, GL_STATIC_DRAW));
        
        NumIndices = newindices.size();
        
        Indices = newindices;
	}

} // end namespace