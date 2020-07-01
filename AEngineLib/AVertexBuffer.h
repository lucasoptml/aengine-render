
#ifndef __AVERTEXBUFFER_H__
#define __AVERTEXBUFFER_H__

#include "AShader.h"

#include <string>
#include <memory>
#include <map>
#include <functional>
#include <set>
#include <vector>

namespace AEngineLib
{
	using std::set;
	using AShaderLib::VertexFormats;
	using namespace AShaderLib;
	using std::string;
	using std::shared_ptr;
	using std::map;
	using std::vector;
	using AEngineLib::CanvasVertex;

	class AENGINE_API AVertexBuffer
	{
	public:
		AVertexBuffer(VertexFormats format)
			: Format(format)
		{
		}

		//NOTE: will create an effect of a certain implementation
		static shared_ptr<AVertexBuffer> GetVertexBuffer(VertexFormats format);

		//
		// Member Functions
		//

		// apply the vertex buffer before rendering
		virtual void Apply()
		{
		}

		// NOTE: just used virtuals to avoid creating new classes for each vertex type (would be cleaner)

		void UpdateActorBuffer(const vector<ActorVertex>& vertices)
		{
			//NOTE: should assert on format mismatch.

			UpdateBuffer(&vertices[0], sizeof(ActorVertex), vertices.size());
		}

		void UpdateAnimationBuffer(const vector<AnimationVertex>& vertices)
		{
			UpdateBuffer(&vertices[0], sizeof(AnimationVertex), vertices.size());
		}

		void UpdateCanvasBuffer(const vector<CanvasVertex>& vertices)
		{
			UpdateBuffer(&vertices[0], sizeof(CanvasVertex), vertices.size());
		}

		void UpdateCanvasByteBuffer(const vector<CanvasByteVertex>& vertices)
		{
			UpdateBuffer(&vertices[0], sizeof(CanvasByteVertex), vertices.size());
		}

		VertexFormats GetFormat()
		{
			return Format;
		}

	protected:

		virtual void UpdateBuffer(const void* start, size_t vertexsize, size_t numverts)
		{
		}

	private:

		VertexFormats Format;

		// List of created buffers
		static set< shared_ptr<AVertexBuffer> > Buffers;
	};

}


#endif