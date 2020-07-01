
#include "AEngineTest.h"

//7. Need to port additional test shaders:
//  x 1. Toon Shader.
//  - 2. Window BYTE color Shader + vertex format.
//  - 3. Batched Actor shader + vertex format. (adds colors)
//  - 4. Basic animation shader test.


using namespace AShaderLib;
using namespace AEngineLib;


const float PI     = 3.141592654f;
const float PIDIV4 = (PI/4.0f);


AMatrix4x4 ProjectionMx;
AMatrix4x4 OrthoMx;
AMatrix4x4 ViewMx;

shared_ptr<AVertexBuffer> CanvasVertexBuffer;
shared_ptr<AIndexBuffer>  CanvasIndexBuffer;

shared_ptr<AVertexBuffer> CubeVertexBuffer;
shared_ptr<AIndexBuffer>  CubeIndexBuffer;

shared_ptr<AVertexBuffer> ScreenCanvasVertexBuffer;


ARenderer* Renderer;


AEngineTest::AEngineTest()
{
	auto engine = AEngine::GetEngine();

	engine->SetInitializedDelegate([&]()
	{
		// called when the engine is ready (devices, resources can be acquired).
		CreateDeviceResources();
		CreateWindowResources();
	});

	engine->SetScreenResizedDelegate([&]()
	{
		CreateWindowResources();
	});

	engine->SetUpdateDelegate([&](double timetotal, double timedelta)
	{
		Update((float)timetotal,(float)timedelta);
	});

	engine->SetRenderDelegate([&]()
	{
		Render();		
	});
}

void AEngineTest::CreateDeviceResources()
{
	Renderer = ARenderer::GetRenderer();
		
	{
		ActorVertex actorvertices[] =
		{
			{  AVector3(-0.5f, -0.5f, -0.5f), AVector3(-0.7f, -0.7f, -0.7f),  {0,0}},
			{  AVector3(-0.5f, -0.5f,  0.5f), AVector3(-0.7f, -0.7f, 0.7f),   {0,1}},
			{  AVector3(-0.5f,  0.5f, -0.5f), AVector3(-0.7f, 0.7f, -0.7f),   {1,0}},
			{  AVector3(-0.5f,  0.5f,  0.5f), AVector3(-0.7f, 0.7f, 0.7f),    {1,1}},
				  					  			  
			{  AVector3( 0.5f, -0.5f, -0.5f), AVector3(0.7f, -0.7f, -0.7f),  {0,0}},
			{  AVector3( 0.5f, -0.5f,  0.5f), AVector3(0.7f, -0.7f,  0.7f),  {0,1}},
			{  AVector3( 0.5f,  0.5f, -0.5f), AVector3(0.7f,  0.7f, -0.7f),  {1,0}},
			{  AVector3( 0.5f,  0.5f,  0.5f), AVector3(0.7f,  0.7f,  0.7f),  {1,1}}
		};
		
		CubeVertexBuffer = AEngineLib::AVertexBuffer::GetVertexBuffer(VertexFormats::Actor);

		vector<ActorVertex> actorvector;
		for( const auto& a : actorvertices )
		{
			actorvector.push_back(a);
		}

		CubeVertexBuffer->UpdateActorBuffer(actorvector);
	}

	{
        float offset = -0.5f;
        
		CanvasVertex canvasvertices[] =
		{
			{ AVector3(0.f + offset, 0.f + offset,0), AColor(1,1,1,1), {0,0}},
			{ AVector3(1.f + offset, 0.f + offset,0), AColor(1,1,1,1), {1,0}},
			{ AVector3(1.f + offset, 1.f + offset,0), AColor(1,1,1,1), {1,1}},
			{ AVector3(0.f + offset, 1.f + offset,0), AColor(1,1,1,1), {0,1}},
		};
		
		vector<CanvasVertex> canvasvector;
		for( auto& a : canvasvertices)
		{
			canvasvector.push_back(a);
		}
        
		ScreenCanvasVertexBuffer = AVertexBuffer::GetVertexBuffer(VertexFormats::Canvas);
		ScreenCanvasVertexBuffer->UpdateCanvasBuffer(canvasvector);
    }
    
    // load test canvas vertices
	{
		float offset = 64;

		CanvasVertex canvasvertices[] =
		{
			{ AVector3(0 + offset,   0 + offset,0),     AColor(1,1,1,1), {0,0}},
			{ AVector3(128 + offset, 0 + offset,0),     AColor(1,1,1,1), {2,0}},
			{ AVector3(128 + offset, 128 + offset,0),   AColor(1,1,1,1), {2,2}},
			{ AVector3(0 + offset,   128 + offset,0),   AColor(1,1,1,1), {0,2}},
		};
		
		vector<CanvasVertex> canvasvector;
		for( auto& a : canvasvertices)
		{
			canvasvector.push_back(a);
		}
			
		CanvasVertexBuffer = AVertexBuffer::GetVertexBuffer(VertexFormats::Canvas);
		CanvasVertexBuffer->UpdateCanvasBuffer(canvasvector);

		unsigned short canvasIndices[] = 
		{
			0,1,2, // -x
			0,2,3,
		};

		vector<unsigned short> indexvector;
		for(auto& i : canvasIndices)
		{
			indexvector.push_back(i);
		}

		CanvasIndexBuffer = AIndexBuffer::GetIndexBuffer();
		CanvasIndexBuffer->UpdateIndexBuffer(indexvector);
	}

	// Load the star texture
	starTexture = ATexture::GetTexture("star.png");

	unsigned short cubeIndices[] = 
	{
		0,2,1, // -x
		1,2,3,

		4,5,6, // +x
		5,7,6,

		0,1,5, // -y
		0,5,4,

		2,6,7, // +y
		2,7,3,

		0,4,6, // -z
		0,6,2,

		1,3,7, // +z
		1,7,5,
	};

	CubeIndexBuffer = AIndexBuffer::GetIndexBuffer();

	vector<unsigned short> indexvector;

	for( auto& i : cubeIndices)
	{
		indexvector.push_back(i);
	}

	CubeIndexBuffer->UpdateIndexBuffer(indexvector);	
}

void AEngineTest::CreateWindowResources()
{
	ARenderer* renderer = ARenderer::GetRenderer();

	//TODO: grab from ARenderer GetScreenWidth, Height
	
    float width  = (float)renderer->GetScreenWidth();
	float height = (float)renderer->GetScreenHeight();

	// fix for zero width/height (null renderer).

	width  = width  ? width  : 100;
	height = height ? height : 100;

	float aspectRatio = width / height;
	float fovAngleY   = 70.0f * PI / 180.0f;

	//TODO: orientation transform 3d likely should be handled elsewhere.

	ProjectionMx = 
		AMatrix4x4::Transpose(
			AMatrix4x4::PerspectiveFOV(fovAngleY,aspectRatio,0.01f,100.0f)
		);
    
	OrthoMx = AMatrix4x4::Transpose(
			AMatrix4x4::OrthoOffcenter(0,width,0,height,0,1)
		);
    
    auto testortho0 = AMatrix4x4::Multiply( OrthoMx, AVector4(0,0,0,1) );
    auto testortho1 = AMatrix4x4::Multiply( OrthoMx, AVector4(1,1,1,1) );
    auto testortho2 = AMatrix4x4::Multiply( OrthoMx, AVector4(100,100,0,1) );
    
    ALog::LogArguments("Screen Width",  width);
    ALog::LogArguments("Screen Height", height);
    
    ALog::LogArguments("Ortho Multiply (0,0,0)",     testortho0.x, testortho0.y, testortho0.z, testortho0.w);
    ALog::LogArguments("Ortho Multiply (1,1,1)",     testortho1.x, testortho1.y, testortho1.z, testortho1.w);
    ALog::LogArguments("Ortho Multiply (100,100,1)", testortho2.x, testortho2.y, testortho2.z, testortho2.w);
    
}

void AEngineTest::Update(double timeTotal, double timeDelta)
{
	(void) timeDelta; // Unused parameter.
    
	AVector3 eye ( 0.0f,  0.7f, 1.5f );
	AVector3 at  ( 0.0f, -0.1f, 0.0f );
	AVector3 up  ( 0.0f, 1.0f,  0.0f );
    
	ViewMx = AMatrix4x4::Transpose( 
				AMatrix4x4::LookAt(up,at,eye) 
			);
    
	TimeSeconds = timeTotal;
    
}

void AEngineTest::Render()
{
	//
	// Setup opEffectLib.
	//


	Renderer = ARenderer::GetRenderer();

	whiteEffect     = AShaderLib::AShader::GetShader("White",  AShaderLib::VertexFormats::Actor);
	windowEffect    = AShaderLib::AShader::GetShader("Window", AShaderLib::VertexFormats::Canvas);
	auto toonEffect = AShaderLib::AShader::GetShader("Toon",   AShaderLib::VertexFormats::Actor);

	AColor bluecolor( 0.098f, 0.098f, 0.439f, 1.000f );

	Renderer->BeginFrame();
	Renderer->Clear(bluecolor);

	auto DrawCube = [&](AColor color, AVector3 offset, AVector3 scale, AShader* shader, shared_ptr<ATexture> texture)
	{
		GlobalConstants globals;

		globals.ProjMx      = ProjectionMx;
		globals.ViewMx      = ViewMx;
		globals.TimeSeconds = (float)TimeSeconds;

		ObjectConstants objectconstants;

		// setup the world matrix
		objectconstants.WorldMx = AMatrix4x4::Transpose(
			AMatrix4x4::Multiply(
				AMatrix4x4::Multiply(
					AMatrix4x4::Scaling(scale.x, scale.y, scale.z),
					AMatrix4x4::RotationY((float)TimeSeconds * PIDIV4)
				),
				AMatrix4x4::Translation(offset.x,offset.y,offset.z)
			)
		);

		objectconstants.DrawColor = color;
		objectconstants.Fatness = 0;

		shader->UpdateGlobalConstants( globals );
		shader->UpdateObjectConstants( objectconstants );
		shader->UpdateTexture2D(TextureSlots::Diffuse, texture);
		
		ALog::Log("Reference WorldMx: " + objectconstants.WorldMx.ToString());
		ALog::Log("Reference ViewMx: "  + globals.ViewMx.ToString());
		ALog::Log("Reference ProjMx: "  + globals.ProjMx.ToString());



		Renderer->Draw(CubeVertexBuffer.get(),CubeIndexBuffer.get(),shader);
	};

	Renderer->DisableAlpha();

	DrawCube(AColor(1,1,1,1), 
			    AVector3(0,0.5,0), 
				AVector3(0.2f,0.2f,0.2f), 
				whiteEffect.get(), 
				starTexture); 

	DrawCube(AColor(1,1,1,1), 
			    AVector3(0.5f,0.0,0), 
				AVector3(0.2f,0.2f,0.2f), 
				whiteEffect.get(), 
				starTexture); 

	
	{
		//Toon Lighting Shader Cube

		//TODO: need a real cube with real texture coords and normals.
		auto toonlookup  = ATexture::GetTexture("Effects/Lookups/toonlookup.png");
		auto rimonly     = ATexture::GetTexture("Effects/Lighting/rimonly.png");
		auto grayTexture = ATexture::GetTexture("Effects/Textures/LargeRock.png");

		toonEffect->UpdateTexture2D(TextureSlots::ToonLookup, toonlookup);
		toonEffect->UpdateTexture2D(TextureSlots::Lighting,   rimonly);

		DrawCube(AColor(1,1,1,1), 
					AVector3(-0.5f,0.0,0), 
					AVector3(0.2f,0.2f,0.2f), 
					toonEffect.get(), 
					grayTexture); 
	}
		

	{
		auto logoEffect = AShader::GetShader("Logo", VertexFormats::Actor);
		auto diffuse    = ATexture::GetTexture("Effects/Logo/lettering.png");

		auto border = ATexture::GetTexture("Effects/Logo/border.png");
		auto lookup = ATexture::GetTexture("Effects/Logo/lookup.png");

		toonEffect->UpdateTexture2D(TextureSlots::SpecularMask, border);
		toonEffect->UpdateTexture2D(TextureSlots::Lookup,       lookup);

		DrawCube(AColor(1,1,1,1), 
			    AVector3(0.0f,0.0,0), 
				AVector3(0.2f,0.2f,0.2f), 
				logoEffect.get(), 
				diffuse); 
	}
     
     
	// draw the canvas test
	{
		Renderer->EnableAlpha();

		GlobalConstants globals;

		globals.ProjMx      = OrthoMx;
		globals.TimeSeconds = (float)TimeSeconds;

		windowEffect->UpdateGlobalConstants(globals);

		windowEffect->UpdateTexture2D(TextureSlots::Diffuse, starTexture);

		Renderer->Draw(CanvasVertexBuffer.get(),CanvasIndexBuffer.get(),windowEffect.get());
	}

    // draw the screen space canvas test
	/*
	{
        auto windowScreenEffect = AShader::GetShader("WindowScreen", VertexFormats::Canvas);
        
		//windowScreenEffect->EnableAlpha();
        
		//GlobalConstants globals;
        
		//globals.ProjMx      = OrthoMx;
		//globals.TimeSeconds = (float)TimeSeconds;
        
		//windowEffect->UpdateGlobalConstants(globals);
        
		//windowEffect->UpdateTexture2D(TextureSlots::Diffuse, starTexture);
        
		Renderer->Draw(ScreenCanvasVertexBuffer.get(),CanvasIndexBuffer.get(),windowScreenEffect.get());
	}
    */

	Renderer->EndFrame();

}
