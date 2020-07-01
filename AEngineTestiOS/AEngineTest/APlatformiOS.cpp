//
//  APlatformiOS.cpp
//  AEngineTest
//
//  Created by Lucas Ellis on 12/25/12.
//  Copyright (c) 2012 Lucas Ellis. All rights reserved.
//

#include "APlatformiOS.h"

#include <OpenGLES/ES2/gl.h>
#include <ios>

double _curRed = 0;
double _timeTotal = 0;

#include <math.h>
#include "../../AEngineLib/AEngineLib.h"

#include "AEngineTest.h"

using namespace AEngineLib;

bool bInitialized = false;

void APlatformiOS::Initialize()
{
    if(bInitialized)
        return;
    
    bInitialized = true;
    
    printf("C++ Initialize\n");

    //NOTE: creating this binds it to the engine internally.
    auto test = new AEngineTest();
    
    auto engine = AEngine::GetEngine();
    
    engine->SetLogDelegate([](string s)
                           {
                               auto logstring = s + "\n";
                               
                               printf("%s",logstring.c_str());
                           }) ;
    
    engine->Initialized();
    
}

void APlatformiOS::Update(double deltatime)
{
    printf("c++ Update: %f\n", deltatime);
    
    _timeTotal += deltatime;
    
    _curRed = sin(_timeTotal);

    auto engine = AEngine::GetEngine();

    engine->Update(_timeTotal,deltatime);
    
}

void APlatformiOS::Render()
{
    printf("c++ Render\n");
    
    auto engine = AEngine::GetEngine();

    engine->Render();
    
    //glClearColor(_curRed, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}




