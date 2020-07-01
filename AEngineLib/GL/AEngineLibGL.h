

#ifndef __AENGINELIBGL_H__
#define __AENGINELIBGL_H__

#include "AEngineLib.h"
#include "AShaderGL.h"
#include "ATextureGL.h"
#include "AVertexBufferGL.h"
#include "AIndexBufferGL.h"
#include "ARendererGL.h"


#ifdef GLDEBUG

    #define GLCALLVAL(name,args) \
        AEngineLib::ALog::LogReturn( name args ); \
        AEngineLib::ALog::Log(std::string("GLCALL: ") + #name + #args); \
        AEngineLib::ALog::LogArguments args; \
        ARendererGL::CheckErrorsGL();

    #define GLCALL(name,args) \
        name args ; \
        AEngineLib::ALog::Log("GLCALL: " #name #args); \
        AEngineLib::ALog::LogArguments args; \
        ARendererGL::CheckErrorsGL();

#else

    #define GLCALLVAL(name,args) name args;
    #define GLCALL(name,args) name args;

#endif



namespace AEngineLib
{
    using std::string;

	class AEngineGL : public AEngine
	{
	public:
        static string GetResourcePath();
        
	private:

	};


}

#endif
