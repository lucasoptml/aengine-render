#include "AEngineTest.h"

using namespace AEngineLib;

int main()
{
	AEngineTest enginetest;

	auto engine = AEngine::GetEngine();

	double totaltime = 0;

	engine->Initialized();

	while(1)
	{
		double deltatime = 0.1;
		
		engine->Update(0,0.1);
		engine->Render();

		totaltime += deltatime;
	}

	
}





