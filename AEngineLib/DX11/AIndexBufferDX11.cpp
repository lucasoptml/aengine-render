


#include "AEngineLibDX11.h"

#include <d3d11.h>

#include <vector>

#include <stdio.h>

namespace AEngineLib
{
	using std::vector;

	AIndexBufferDX11::~AIndexBufferDX11()
	{
		//TODO: release resources correctly.
	}



	//
	// AVertexBuffer
	//

	shared_ptr<AIndexBuffer> AIndexBufferDX11::CreateIndexBufferDX11()
	{
		auto newbuffer = new AIndexBufferDX11();

		auto ptr = shared_ptr<AIndexBuffer>(newbuffer);

		return ptr;
	}


	void AIndexBufferDX11::Apply()
	{
		auto Context = AEngineDX11::GetDeviceContext();

		Context->IASetIndexBuffer(Buffer,DXGI_FORMAT_R16_UINT,0);

		Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	}

	void AIndexBufferDX11::UpdateIndexBuffer(const vector<unsigned short>& indices)
	{
		//NOTE: should assert on vertsize different than VertexSize

		//NOTE: obviously should not create every frame, instead should update
		
		
		auto Device = AEngineDX11::GetDevice();

		D3D11_SUBRESOURCE_DATA data = {0};
		data.pSysMem = &indices[0];
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		size_t memsize = indices.size() * sizeof(unsigned short);

		CD3D11_BUFFER_DESC desc(memsize,D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER);

		Device->CreateBuffer( &desc, &data, &Buffer );

		NumIndices = indices.size();
	}

} // end namespace