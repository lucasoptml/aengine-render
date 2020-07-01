
#ifndef __AINDEXBUFFER_H__
#define __AINDEXBUFFER_H__

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


	class AENGINE_API AIndexBuffer
	{
	public:
		AIndexBuffer()
			: NumIndices(0)
		{
		}

		//NOTE: will create object of a certain implementation
		static shared_ptr<AIndexBuffer> GetIndexBuffer();

		virtual void UpdateIndexBuffer(const vector<unsigned short>& indices)
		{

		}

		// apply before rendering
		virtual void Apply()
		{
			
		}

		int GetNumIndices()
		{
			return NumIndices;
		}

		int GetNumPrimitives()
		{
			return NumIndices / 3;
		}
		
	protected:

		int NumIndices;

	private:

		static set< shared_ptr<AIndexBuffer> > Indexes;

		
	};

}


#endif