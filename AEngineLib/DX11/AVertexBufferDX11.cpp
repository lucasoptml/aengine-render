


#include "AEngineLibDX11.h"

#include <d3d11.h>

#include <vector>

#include <stdio.h>

namespace AEngineLib
{
	using std::vector;

	AVertexBufferDX11::~AVertexBufferDX11()
	{
		//TODO: release resources correctly.
	}



	//
	// AVertexBuffer
	//

	shared_ptr<AVertexBuffer> AVertexBufferDX11::CreateVertexBufferDX11(VertexFormats format)
	{
		auto newbuffer = new AVertexBufferDX11(format);

		auto ptr = shared_ptr<AVertexBuffer>(newbuffer);

		return ptr;
	}


	void AVertexBufferDX11::Apply()
	{
		auto Context = AEngineDX11::GetDeviceContext();

		UINT stride = VertexSize;
		UINT offset = 0;
		Context->IASetVertexBuffers(0,1,&Buffer,&stride,&offset);
	}

	void AVertexBufferDX11::UpdateBuffer(const void* start, size_t vertsize, size_t num)
	{
		//NOTE: should assert on vertsize different than VertexSize

		//NOTE: obviously should not create every frame, instead should update
		NumVertices = num;
		
		auto Device = AEngineDX11::GetDevice();

		CD3D11_BUFFER_DESC desc(VertexSize*NumVertices,D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER);
		
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = start;
		data.SysMemPitch = vertsize * num;

		Device->CreateBuffer( &desc, &data, &Buffer );

	}

} // end namespace