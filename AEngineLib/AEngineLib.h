

#ifndef __AENGINELIB_H__
#define __AENGINELIB_H__


// Platform Detection
#ifdef __APPLE__
    #include "TargetConditionals.h"
#endif

#ifndef AENGINE_API
	#define AENGINE_API __declspec(dllimport)
#endif

#ifdef TARGET_OS_IPHONE
    #define AENGINE_IOS 1
    #define AENGINE_GL  1
#endif


// include all parts of AEngineLib
#include "AShader.h"
#include "AVertexBuffer.h"
#include "AIndexBuffer.h"
#include "ATexture.h"
#include "ARenderer.h"

// include all standard libraries
#include <functional>

#include <sstream>


namespace AEngineLib
{
	using std::function;
    using namespace std::placeholders;
    
	class AENGINE_API AEngine
	{
	public:

		virtual void Initialized()
		{
			if(InitializedDelegate)
			{
				InitializedDelegate();
			}
		}

		virtual void Update(double timetotal, double timedelta )
		{
			if(UpdateDelegate)
			{
				UpdateDelegate(timetotal,timedelta);
			}
		}

		virtual void Render()
		{
			if(RenderDelegate)
			{
				RenderDelegate();
			}
		}

		virtual void ScreenResized()
		{
			if(ScreenResizedDelegate)
			{
				ScreenResizedDelegate();
			}
		}
        
		virtual void PointerMoved(float x, float y)
		{
			if(PointerMoveDelegate)
			{
				PointerMoveDelegate(x,y);
			}
		}

		virtual void PointerPress()
		{
			if(PointerPressDelegate)
			{
				PointerPressDelegate();
			}
		}

		virtual void PointerRelease()
		{
			if(PointerReleaseDelegate)
			{
				PointerReleaseDelegate();
			}
		}

		virtual void KeyInput(wchar_t value)
		{
			if(KeyInputDelegate)
			{
				KeyInputDelegate(value);
			}
		}

        virtual void Log(string s)
        {
            if(LogDelegate)
            {
                LogDelegate(s);
            }
        }


		//
		// Setters
		//

		void SetInitializedDelegate( function<void()> initialized )
		{
			InitializedDelegate = initialized;
		}

		void SetUpdateDelegate( function<void( double, double )> update )
		{
			UpdateDelegate = update;
		}

		void SetRenderDelegate( function<void()> render)
		{
			RenderDelegate = render;
		}

		void SetScreenResizedDelegate( function< void() > resized )
		{
			ScreenResizedDelegate = resized;
		}

		void SetShownDelegate( function< void() > shown )
		{
			ShownDelegate = shown;
		}

		void SetHiddenDelegate( function< void() > hidden )
		{
			HiddenDelegate = hidden;
		}

		void SetPointerMoveDelegate( function< void(float,float) > move)
		{
			PointerMoveDelegate = move;
		}

		void SetPointerPressDelegate( function<void()> press )
		{
			PointerPressDelegate = press;
		}

		void SetPointerReleaseDelegate( function<void()> release )
		{
			PointerReleaseDelegate = release;
		}
        
		void SetKeyInputDelegate( function<void(wchar_t)> input )
		{
			KeyInputDelegate = input;
		}

        void SetLogDelegate( function<void(string)> log )
        {
            LogDelegate = log;
        }
		
		//
		// Static Functions
		//
		
		static AEngine* GetEngine();
        
		static void Start();

	protected:

		// ctor
		AEngine()
		{

		}

	private:
		
		//
		// Delegates
		//

		function< void() >               InitializedDelegate;

		function< void(double, double) > UpdateDelegate;
		function< void() >				 RenderDelegate;

		function< void() >				 ScreenResizedDelegate;
		
		function< void() >               ShownDelegate;
		function< void() >				 HiddenDelegate;
		function< void(float,float) >    PointerMoveDelegate;
		function< void() >				 PointerPressDelegate;
		function< void() >				 PointerReleaseDelegate;

		function< void(wchar_t) >        KeyInputDelegate;

        function< void(string) >         LogDelegate;
        
		//
		// Static Data
		//
		
		static AEngine* Engine;
	};
    
    class ALog
    {
    public:
        static void Log(string s)
        {
            AEngine::GetEngine()->Log(s);
        }
        
        static void LogArguments()
        {
    
        }

    
        template<typename t1>
        static void LogArguments(t1 a1)
        {
            string s = "(";
            
            s += ConvertToString(a1);
            
            s += ")";
            Log(s);
        }
    
        template<typename t1, typename t2>
        static void LogArguments(t1 a1, t2 a2)
        {
            string s = "(";
            
            s += ConvertToString(a1);
            
            s += ", ";
            
            s += ConvertToString(a2);
           
            s += ")";
            Log(s);
        }
    
        template<typename t1, typename t2, typename t3>
        static void LogArguments(t1 a1, t2 a2, t3 a3)
        {
            string s = "(";
            
            s += ConvertToString(a1);
            
            s += ", ";
            
            s += ConvertToString(a2);
            
            s += ", ";
            
            s += ConvertToString(a3);
            
            s += ")";
            Log(s);
        }
    
        template<typename t1, typename t2, typename t3, typename t4>
        static void LogArguments(t1 a1, t2 a2, t3 a3, t4 a4)
        {
            string s = "(";
            
            s += ConvertToString(a1);
            
            s += ", ";
            
            s += ConvertToString(a2);
            
            s += ", ";
            
            s += ConvertToString(a3);

            s += ", ";
            
            s += ConvertToString(a4);
            
            s += ")";
            Log(s);

        }
    
        template<typename t1, typename t2, typename t3, typename t4, typename t5>
        static void LogArguments(t1 a1, t2 a2, t3 a3, t4 a4, t5 a5)
        {
            string s = "(";
            
            s += ConvertToString(a1);
            
            s += ", ";
            
            s += ConvertToString(a2);
            
            s += ", ";
            
            s += ConvertToString(a3);
            
            s += ", ";
            
            s += ConvertToString(a4);
         
            s += ", ";

            s += ConvertToString(a5);
            
            s += ")";
            Log(s);
        }
    
        template<typename t1, typename t2, typename t3, typename t4, typename t5, typename t6>
        static void LogArguments(t1 a1, t2 a2, t3 a3, t4 a4, t5 a5, t6 a6)
        {
            string s = "(";
            

            s += ConvertToString(a1);
            
            s += ", ";
            
            s += ConvertToString(a2);
            
            s += ", ";
            
            s += ConvertToString(a3);
            
            s += ", ";
            
            s += ConvertToString(a4);
            
            s += ", ";
            
            s += ConvertToString(a5);
            
            s += ", ";
            
            s += ConvertToString(a6);
            
            s += ")";
            Log(s);
        }

        template<typename t1,
                 typename t2,
                 typename t3,
                 typename t4,
                 typename t5,
                 typename t6,
                 typename t7,
                 typename t8,
                 typename t9>
        static void LogArguments(t1 a1, t2 a2, t3 a3, t4 a4, t5 a5, t6 a6, t7 a7, t8 a8, t9 a9)
        {
            string s = "(";
            
            
            s += ConvertToString(a1);
            
            s += ", ";
            
            s += ConvertToString(a2);
            
            s += ", ";
            
            s += ConvertToString(a3);
            
            s += ", ";
            
            s += ConvertToString(a4);
            
            s += ", ";
            
            s += ConvertToString(a5);
            
            s += ", ";
            
            s += ConvertToString(a6);
            
            s += ", ";
            
            s += ConvertToString(a7);
            
            s += ", ";
            
            s += ConvertToString(a8);
            
            s += ", ";
            
            s += ConvertToString(a9);
            
            s += ")";
            Log(s);
        }
    
        template<typename T>
        static string ConvertToString(T a)
        {
            std::stringstream s;
            
            s << a;
            
            string result = s.str();
            
            return result;
        }

        template<typename T>
        static T LogReturn( T arg )
        {
            return arg;
        }
    
        static void LogReturn( void )
        {
        
        }
    
    };

}



#endif
