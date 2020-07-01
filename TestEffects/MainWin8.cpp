#include "AEngineLib.h"
#include "AEngineTest.h"

using namespace Windows::ApplicationModel::Core;


[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	AEngineTest engine;

	AEngine::Start();
	return 0;
}
