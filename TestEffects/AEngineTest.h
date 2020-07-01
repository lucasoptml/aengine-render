#pragma once

#include "AEngineLib.h"

using namespace AEngineLib;

// This class renders a simple spinning cube.
class AEngineTest
{
public:
	AEngineTest();

	// happens initially + on device reset
	virtual void CreateDeviceResources();
	
	// happens initially + on size change
	virtual void CreateWindowResources();

	// Method for updating time-dependent objects.
	void Update(double timeTotal, double timeDelta);

	// happens on render
	virtual void Render();

private:

	shared_ptr<AShader> whiteEffect;
	shared_ptr<AShader> windowEffect;

	shared_ptr<ATexture> starTexture;

	double TimeSeconds;

};
