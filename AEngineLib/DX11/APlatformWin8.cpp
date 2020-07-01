

#include "APlatformWin8.h"

#include "BasicTimer.h"

#include "AEngineLib.h"
#include "DX11/AEngineLibDX11.h"

#include <ppltasks.h>




using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace concurrency;


using AEngineLib::AEngine;

//NOTE: events to handle:
//      Main:
//			- Activated, Suspending, Resuming
//			- Event Loop, Update Loop (timer)
//		Engine:
//			- Shown, Hidden, VisibilityChanged (?)
//			- Update
//			- Pointer: Press, Release, Move
//			- WindowResized
//			- CreateRenderDevice (or Recreate)
//			- GetInstance() static
//		EngineWin8
//			- SetCoreWindow, GetCoreWindow
namespace AEngineLibPlatform
{

APlatformWin8::APlatformWin8() :
	m_windowClosed(false),
	m_windowVisible(true)
{
	//initialize the engine

	AEngine::GetEngine()->SetLogDelegate([](string s)
	{
		std::wstring w;
		w.assign(s.begin(),s.end());
		
		printf((s + "\n").c_str());
		
		OutputDebugString(w.c_str());
	});

}

void APlatformWin8::Initialize(CoreApplicationView^ applicationView)
{
	applicationView->Activated +=
        ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &APlatformWin8::OnActivated);

	CoreApplication::Suspending +=
        ref new EventHandler<SuspendingEventArgs^>(this, &APlatformWin8::OnSuspending);

	CoreApplication::Resuming +=
        ref new EventHandler<Platform::Object^>(this, &APlatformWin8::OnResuming);


}

void APlatformWin8::SetWindow(CoreWindow^ window)
{
	window->SizeChanged += 
        ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &APlatformWin8::OnWindowSizeChanged);

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &APlatformWin8::OnVisibilityChanged);

	window->Closed += 
        ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &APlatformWin8::OnWindowClosed);

	window->PointerCursor = ref new CoreCursor(CoreCursorType::Arrow, 0);

	window->PointerPressed +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &APlatformWin8::OnPointerPressed);

	window->PointerMoved +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &APlatformWin8::OnPointerMoved);

	window->PointerReleased +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &APlatformWin8::OnPointerReleased);

	window->KeyDown += 
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &APlatformWin8::OnKeyDown);

	window->KeyUp += 
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &APlatformWin8::OnKeyUp);

	window->CharacterReceived += 
		ref new TypedEventHandler<CoreWindow^, CharacterReceivedEventArgs^>(this, &APlatformWin8::OnKeyInput);


	auto engine = AEngine::GetEngine();

	engine->Initialized();
}

void APlatformWin8::Load(Platform::String^ entryPoint)
{
}

void APlatformWin8::Run()
{
	BasicTimer^ timer = ref new BasicTimer();

	while (!m_windowClosed)
	{
		if (m_windowVisible)
		{
			timer->Update();
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
		
			auto engine = AEngine::GetEngine();
			engine->Update(timer->Total, timer->Delta);
			engine->Render();
		}
		else
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
		}
	}
}

void APlatformWin8::Uninitialize()
{
}

void APlatformWin8::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	auto engine = AEngine::GetEngine();

	engine->ScreenResized();
}

void APlatformWin8::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
}

void APlatformWin8::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	m_windowClosed = true;
}

void APlatformWin8::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
{
	auto engine = AEngine::GetEngine();

	engine->PointerPress();
}

void APlatformWin8::OnPointerMoved(CoreWindow^ sender, PointerEventArgs^ args)
{
	auto engine = AEngine::GetEngine();

	engine->PointerMoved(args->CurrentPoint->Position.X,args->CurrentPoint->Position.Y);

	//NOTE: should possibly buffer these calls here.
}

void APlatformWin8::OnPointerReleased(CoreWindow^ sender, PointerEventArgs^ args)
{
	auto engine = AEngine::GetEngine();

	engine->PointerRelease();
}

void APlatformWin8::OnKeyDown(CoreWindow^ sender, KeyEventArgs^ args)
{
	auto engine = AEngine::GetEngine();

	//engine->KeyDown(args->VirtualKey);
	//TODO: translate to AEngine virtual keys enum
}

void APlatformWin8::OnKeyUp(CoreWindow^ sender, KeyEventArgs^ args)
{
	auto engine = AEngine::GetEngine();

	//engine->PointerRelease();
		//TODO: translate to AEngine virtual keys enum
}

void APlatformWin8::OnKeyInput(CoreWindow^ sender, CharacterReceivedEventArgs^ args)
{
	auto engine = AEngine::GetEngine();

	engine->KeyInput((wchar_t)args->KeyCode);
	//engine->PointerRelease();
		//TODO: translate to AEngine virtual keys enum
}

void APlatformWin8::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	CoreWindow::GetForCurrentThread()->Activate();
}

void APlatformWin8::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
	// Save app state asynchronously after requesting a deferral. Holding a deferral
	// indicates that the application is busy performing suspending operations. Be
	// aware that a deferral may not be held indefinitely. After about five seconds,
	// the app will be forced to exit.
	SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();

	create_task([this, deferral]()
	{
		// Insert your code here.

		deferral->Complete();
	});
}
 
void APlatformWin8::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
	// Restore any data or state that was unloaded on suspend. By default, data
	// and state are persisted when resuming from suspend. Note that this event
	// does not occur if the app was previously terminated.
}

}

//
// 
//


//TODO: to fix, think I need to add APlatform::Start();
//TODO: and hide everything behind that.

ref class Direct3DApplicationSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource
{
public:
	virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();

};

IFrameworkView^ Direct3DApplicationSource::CreateView()
{
    return ref new AEngineLibPlatform::APlatformWin8();
		//AEngineLib::APlatformWin8Wrapper::GetFrameworkView();
}



void AEngineLib::AEngine::Start()
{
	auto direct3DApplicationSource = ref new Direct3DApplicationSource();
	CoreApplication::Run(direct3DApplicationSource);
}

