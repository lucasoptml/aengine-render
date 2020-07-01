

#ifndef __AVERTEXBUFFERDX11_H__
#define __AVERTEXBUFFERDX11_H__


#include "AVertexBuffer.h"

#include <vector>
#include <string>

struct ID3D11Buffer;


using std::wstring;


namespace AEngineLib
{
	using std::vector;


	class AVertexBufferDX11 : public AVertexBuffer
	{
	public:
		AVertexBufferDX11(VertexFormats format)
			: 
			AVertexBuffer(format),
			Buffer(nullptr),
			NumVertices(0)
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

		virtual ~AVertexBufferDX11();

		// Create from file.
		//NOTE: add delegate for grabbing files if you want alternative to filename.
		static shared_ptr<AVertexBuffer> CreateVertexBufferDX11(VertexFormats format);

		virtual void Apply();
		virtual void UpdateBuffer(const void* start, size_t vertexsize, size_t num);

	private:

		ID3D11Buffer* Buffer;

		size_t	      NumVertices;
		size_t        VertexSize;
	};

}


#endif
