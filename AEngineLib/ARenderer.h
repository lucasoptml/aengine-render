
#ifndef __ARENDERER_H__
#define __ARENDERER_H__

#include "AVertexBuffer.h"
#include "AIndexBuffer.h"
#include "AShader.h"

#include <string>
#include <memory>
#include <map>
#include <functional>
#include <set>
#include <vector>

namespace AEngineLib
{
	using std::vector;
	using std::set;
	using std::string;
	using std::shared_ptr;
	using std::map;

	using namespace AShaderLib;


	class AENGINE_API ARenderer
	{
	public:

		//NOTE: will create object of a certain implementation
		static ARenderer* GetRenderer();

		virtual void BeginFrame()
		{
		}

		virtual void EnableDepth()
		{
		}

		virtual void DisableDepth()
		{
		}

		virtual void EnableAlpha()
		{
		}

		virtual void DisableAlpha()
		{
		}

		virtual void Clear(const AColor& color)
		{
		}

		virtual void Draw(AVertexBuffer* vertexbuffer, AIndexBuffer* indexbuffer, AShader* shader)
		{
		}

		virtual void EndFrame()
		{
		}
		
		int GetScreenWidth()
		{
			return ScreenWidth;
		}

		int GetScreenHeight()
		{
			return ScreenHeight;
		}

	protected:

		ARenderer()
			:
			ScreenWidth(0),
			ScreenHeight(0)
		{
		}

		int ScreenWidth;
		int ScreenHeight;

	private:
		
		static ARenderer* Renderer;

	};

}


#endif