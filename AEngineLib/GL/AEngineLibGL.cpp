//
// AEngineLib.cpp : Defines the exported functions for the DLL application.
//

#include "AEngineLibGL.h"

//NOTE: ios related, not gl
#include <CoreFoundation/CoreFoundation.h>


namespace AEngineLib
{

    string AEngineGL::GetResourcePath()
    {
        string pathstr;
        
        CFBundleRef mainBundle   = CFBundleGetMainBundle();
        CFURLRef    resourcesURL = CFBundleCopyBundleURL(mainBundle);
        CFStringRef str          = CFURLCopyFileSystemPath( resourcesURL, kCFURLPOSIXPathStyle );
        CFRelease(resourcesURL);
        
        char path[PATH_MAX];
        
        
        CFStringGetCString( str, path, FILENAME_MAX, kCFStringEncodingASCII );
        CFRelease(str);
        
        pathstr = path;
        pathstr += "/";
        
        return pathstr;
    }

    
}