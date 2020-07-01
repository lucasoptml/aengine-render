


#include "AIndexBuffer.h"

#include <algorithm>

#if AENGINE_DX11 == 1
	#include "DX11/AIndexBufferDX11.h"
#elif AENGINE_GL == 1
    #include "GL/AIndexBufferGL.h"
#endif

	
namespace AEngineLib
{
	set< shared_ptr<AIndexBuffer> > AIndexBuffer::Indexes  = set< shared_ptr<AIndexBuffer> >();
	
	shared_ptr<AIndexBuffer> AIndexBuffer::GetIndexBuffer()
	{
		shared_ptr<AIndexBuffer> newindex;

		// Create a new effect given the implementation
#if AENGINE_DX11 == 1
		newindex = AIndexBufferDX11::CreateIndexBufferDX11();
#elif AENGINE_GL == 1
        newindex = AIndexBufferGL::CreateIndexBufferGL();
#else
		newindex = shared_ptr<AIndexBuffer>( new AIndexBuffer() );
#endif

		if(newindex)
		{
			Indexes.insert(newindex);
		}

		return newindex;
	}

}