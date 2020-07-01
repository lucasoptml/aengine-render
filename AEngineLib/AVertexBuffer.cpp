


#include "AVertexBuffer.h"

#include <algorithm>
#include <set>

#if AENGINE_DX11 == 1
	#include "DX11/AVertexBufferDX11.h"
#elif AENGINE_GL == 1
    #include "GL/AEngineLibGL.h"
#endif

	
namespace AEngineLib
{
	set<shared_ptr<AVertexBuffer>> AVertexBuffer::Buffers = set<shared_ptr<AVertexBuffer> >();
		
	// ToLower with new foreach syntax
	inline void convertToLower(string s)
	{
		string out = s;

		for( char& c : s )
		{
			c = tolower(c);
		};
	}

	shared_ptr<AVertexBuffer> AVertexBuffer::GetVertexBuffer(VertexFormats format)
	{

		shared_ptr<AVertexBuffer> newbuffer;

		// Create a new effect given the implementation
#if AENGINE_DX11 == 1
		newbuffer = AVertexBufferDX11::CreateVertexBufferDX11(format);
#elif AENGINE_GL == 1
        newbuffer = AVertexBufferGL::CreateVertexBufferGL(format);
#else
		newbuffer = shared_ptr<AVertexBuffer>( new AVertexBuffer(format) );
#endif

		if(newbuffer)
		{
			Buffers.insert(newbuffer);
		}

		return newbuffer;
	}

}