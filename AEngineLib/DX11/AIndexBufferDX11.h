

#ifndef __AINDEXBUFFERDX11_H__
#define __AINDEXBUFFERDX11_H__


#include "AIndexBuffer.h"

#include <vector>
#include <string>

struct ID3D11Buffer;


using std::wstring;


namespace AEngineLib
{
	using std::vector;



	class AIndexBufferDX11 : public AIndexBuffer
	{
	public:
		AIndexBufferDX11()
			: Buffer(nullptr)
		{
		}

		virtual ~AIndexBufferDX11();

		// Create from file.
		//NOTE: add delegate for grabbing files if you want alternative to filename.
		static shared_ptr<AIndexBuffer> CreateIndexBufferDX11();

		virtual void Apply();
		virtual void UpdateIndexBuffer(const vector<unsigned short>& indices);

	private:

		ID3D11Buffer* Buffer;

	};

}


#endif
