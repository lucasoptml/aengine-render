


#include "AEngineLibGL.h"

#include <stdio.h>

using namespace AEngineLib;

namespace AShaderLib
{
    using std::string;
    
    
	AShaderGL::AShaderGL(string name, VertexFormats format, GLuint vshader, GLuint pshader, GLuint program)
        :
        AShader(name),
        VertexFormat(format),
        VertexShader(vshader),
        PixelShader(pshader),
        Program(program)
	{
        SetupVertexFormat(VertexFormat);
	}
    
    AShaderGL::~AShaderGL()
    {
    }

	bool get_file_contents(string filename, vector<char>& contents)
	{
	  std::FILE *fp;
 
        fp = fopen(filename.c_str(),"rb");
        
	  if (fp)
	  {
		std::fseek(fp, 0, SEEK_END);
		contents.resize(std::ftell(fp));
		std::rewind(fp);
		std::fread(&contents[0], 1, contents.size(), fp);
		std::fclose(fp);
    
		return true;
	  }

	  return false;
	}

	//
	// AShader
	//
    
    GLuint CreateVertexShader(string name)
    {
        ALog::Log(string("Create Vertex Shader: ") + name);
        
        string basepath = AEngineGL::GetResourcePath() + "/Effects/GLSL/";
        
        auto vertexfile = basepath + name + ".vertex.glsl";
        
        vector<char> vertexshaderdata;
		get_file_contents(vertexfile, vertexshaderdata);
        
        vector<char> globalshaderdata;
        get_file_contents(basepath + "Global.glsli", globalshaderdata);
        
        
        const GLchar* filestrings[] =
        {
            //&globalshaderdata[0],
            &vertexshaderdata[0],
        };
        
        const GLint lengths[] =
        {
            //(int)globalshaderdata.size(),
            (int)vertexshaderdata.size()
            
        };
        
        GLuint shader = GLCALL(glCreateShader,(GL_VERTEX_SHADER));
        

        GLCALL(glShaderSource,(shader, 1, filestrings, lengths));
 
        GLCALL(glCompileShader,(shader));
 
        // check for errors
        int success;
        GLCALL(glGetShaderiv, (shader, GL_COMPILE_STATUS, &success));
        
        char errorMsg[2048];
        
        if (success == 0)
        {
            GLCALL(glGetShaderInfoLog,(shader, sizeof(errorMsg), NULL, errorMsg));

            ALog::Log(errorMsg);
            
            //throw "Error";
        }
        return shader;
    }
    
    GLuint CreatePixelShader(string name)
    {
        ALog::Log(string("Create Pixel Shader: ") + name);
        
        string basepath = AEngineGL::GetResourcePath() + "/Effects/GLSL/";
        
        auto shaderfile = basepath + name + ".pixel.glsl";
        
        vector<char> shaderdata;
		get_file_contents(shaderfile, shaderdata);
        
        vector<char> globalshaderdata;
        get_file_contents(basepath + "Global.glsli", globalshaderdata);
        
        const GLchar* filestrings[] =
        {
            //&globalshaderdata[0],
            &shaderdata[0]
        };
        
        const GLint lengths[] =
        {
            //(int)globalshaderdata.size(),
            (int)shaderdata.size()
        };
       
        GLuint shader = GLCALL(glCreateShader,(GL_FRAGMENT_SHADER));
 
        GLCALL(glShaderSource,(shader,1,filestrings,lengths));

        GLCALL(glCompileShader,(shader));

        // check for errors
        int success;
        GLCALL(glGetShaderiv, (shader, GL_COMPILE_STATUS, &success));
        ARendererGL::CheckErrorsGL();

        char errorMsg[2048];
        
        if (success == 0)
        {
            GLCALL(glGetShaderInfoLog,(shader, sizeof(errorMsg), NULL, errorMsg));
            ARendererGL::CheckErrorsGL();

            ALog::Log(errorMsg);
            
            //throw "Pixel Shader Error";
        }
        return shader;
    }
    

	shared_ptr<AShader> AShaderGL::CreateShaderGL(string name, VertexFormats format)
	{
        auto vertexshader = CreateVertexShader(name);
        auto pixelshader  = CreatePixelShader(name);
        
        auto program = glCreateProgram();
        
        GLCALL(glAttachShader, (program,vertexshader));
        
        GLCALL(glAttachShader, (program,pixelshader));
       
        GLCALL(glLinkProgram, (program));
        
        auto shader = new AShaderGL(name,format,vertexshader,pixelshader,program);
 
		auto shaderptr = shared_ptr<AShader>(shader);

		return shaderptr;
	}

	bool AShaderGL::UpdateGlobalConstants(const GlobalConstants& constants)
	{
        Apply();
        
		// if the global constant buffer is not created, create it.
		
        auto ViewMx              = GLCALL(glGetUniformLocation,(Program, "ViewMx"));
        
        auto ProjMx              = GLCALL(glGetUniformLocation,(Program, "ProjMx"));
        
        auto TimeSeconds         = GLCALL(glGetUniformLocation,(Program, "TimeSeconds"));

        auto CameraWorldForward  = GLCALL(glGetUniformLocation,(Program, "CameraWorldForward"));

        auto CameraWorldRight    = GLCALL(glGetUniformLocation,(Program, "CameraWorldRight"));

        auto CameraWorldUp       = GLCALL(glGetUniformLocation,(Program, "CameraWorldUp"));

        auto CameraWorldPosition = GLCALL(glGetUniformLocation,(Program, "CameraWorldPosition"));

        auto ViewportWidth       = GLCALL(glGetUniformLocation,(Program, "ViewportWidth"));

        auto ViewportHeight      = GLCALL(glGetUniformLocation,(Program, "ViewportHeight"));

        auto NearClipping        = GLCALL(glGetUniformLocation,(Program, "NearClipping"));

        auto FarClipping         = GLCALL(glGetUniformLocation,(Program, "FarClipping"));
        
        //NOTE: setting uniform values will fail if glUseProgram has not been called!
        GLCALL(glUniformMatrix4fv, (ViewMx, 1, GL_FALSE, &constants.ViewMx._11));

        GLCALL(glUniformMatrix4fv, (ProjMx, 1, GL_FALSE, (float*)&constants.ProjMx));

        GLCALL(glUniform1f, (TimeSeconds, constants.TimeSeconds));
        
        GLCALL(glUniform3f,(CameraWorldForward,
                    constants.CameraWorldForward.x,
                    constants.CameraWorldForward.y,
                    constants.CameraWorldForward.z));
        
        GLCALL(glUniform3f, (CameraWorldRight,
                    constants.CameraWorldRight.x,
                    constants.CameraWorldRight.y,
                    constants.CameraWorldRight.z));

        
        GLCALL(glUniform3f, (CameraWorldPosition,
                    constants.CameraWorldPosition.x,
                    constants.CameraWorldPosition.y,
                    constants.CameraWorldPosition.z));
        
        GLCALL(glUniform1f,(ViewportWidth,  constants.ViewportWidth));

        GLCALL(glUniform1f,(ViewportHeight, constants.ViewportHeight));

        GLCALL(glUniform1f,(NearClipping, constants.NearClipping));

        GLCALL(glUniform1f,(FarClipping,  constants.FarClipping));
        
		return true;
	}
    
	bool AShaderGL::UpdateObjectConstants(const ObjectConstants& constants)
	{
        Apply();
        
		// if the global constant buffer is not created, create it.

        auto WorldMx   = GLCALL(glGetUniformLocation,(Program, "WorldMx"));

        auto DrawColor = GLCALL(glGetUniformLocation,(Program, "DrawColor"));

        auto Fatness   = GLCALL(glGetUniformLocation,(Program, "Fatness"));

        GLCALL(glUniformMatrix4fv, (WorldMx, 1, GL_FALSE, (float*)&constants.WorldMx));
        
        GLCALL(glUniform4f,( DrawColor,
                    constants.DrawColor.r,
                    constants.DrawColor.g,
                    constants.DrawColor.b,
                    constants.DrawColor.a));
        
        GLCALL(glUniform1f,(Fatness, constants.Fatness));
        
		return true;
	}


	bool AShaderGL::UpdateAnimationConstants(const AnimationConstants& constants)
	{
        Apply();
        
		// if the global constant buffer is not created, create it.
        
        auto SkinPalette = GLCALL(glGetUniformLocation,(Program, "SkinPalette"));
        
        GLCALL(glUniformMatrix4fv,(SkinPalette, 64, GL_FALSE, (float*)&constants.SkinPalette));
        
		return true;
	}

	bool AShaderGL::UpdateOptionalConstants(const OptionalConstants& constants)
	{
        Apply();
        
		// if the global constant buffer is not created, create it.

        auto Factor    = GLCALL(glGetUniformLocation,(Program, "Factor"));
        
        auto Frequency = GLCALL(glGetUniformLocation,(Program, "Frequency"));
        
        GLCALL(glUniform1f,(Factor,    constants.Factor));

        GLCALL(glUniform1f,(Frequency, constants.Frequency));

		return true;
	}

	void AShaderGL::EnableAlpha()
	{
        GLCALL(glEnable,(GL_BLEND));

        GLCALL(glBlendFunc,(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));
        
	}

	void AShaderGL::DisableAlpha()
	{
        GLCALL(glDisable,(GL_BLEND));
	}

	void AShaderGL::Apply()
	{
        GLCALL(glUseProgram,(Program));
        
        
        ApplyVertexFormat(VertexFormat);
	}
    
    void AShaderGL::SetupVertexFormat(VertexFormats format)
    {
		switch(format)
		{
			case VertexFormats::Actor:
			{

                
			}
            break;
			case VertexFormats::Animation:
			{

                
			}
            break;
			case VertexFormats::Canvas:
			{

                
            }
            break;
		}
    }
    
	void AShaderGL::ApplyVertexFormat(VertexFormats format)
	{
		switch(format)
		{
			case VertexFormats::Actor:
			{
                auto size = sizeof(ActorVertex);
                
                auto LocalPosition = GLCALL(glGetAttribLocation,(Program,"LocalPosition"));

                auto LocalNormal   = GLCALL(glGetAttribLocation,(Program,"LocalNormal"));

                auto TexCoord      = GLCALL(glGetAttribLocation,(Program,"TexCoord"));
    
                if(LocalPosition != -1)
                {
                    GLCALL(glEnableVertexAttribArray,(LocalPosition));
                    
                    GLCALL(glVertexAttribPointer,(LocalPosition, 3, GL_FLOAT, GL_FALSE, size, 0));
                }
                
                if(LocalNormal != -1)
                {
                    GLCALL(glEnableVertexAttribArray,(LocalNormal));
                    
                    GLCALL(glVertexAttribPointer,(LocalNormal,   3, GL_FLOAT, GL_FALSE, size, (const GLvoid*)(4*3)));
                }
                
                if(TexCoord != -1)
                {
                    GLCALL(glEnableVertexAttribArray,(TexCoord));
                    
                    GLCALL(glVertexAttribPointer,(TexCoord,      2, GL_FLOAT, GL_FALSE, size, (const GLvoid*)(4*6)));
                }

			}
			break;
			case VertexFormats::Animation:
			{

			}
			break;
			case VertexFormats::Canvas:
			{
                auto size = sizeof(CanvasVertex);
                
                auto Position  = glGetAttribLocation(Program,"Position");
                ARendererGL::CheckErrorsGL();

                auto Color     = glGetAttribLocation(Program,"Color");
                ARendererGL::CheckErrorsGL();

                auto TexCoord  = glGetAttribLocation(Program,"TexCoord");
                ARendererGL::CheckErrorsGL();
               
                if(Position != -1)
                {
                    GLCALL(glEnableVertexAttribArray,(Position));
                    
                    GLCALL(glVertexAttribPointer,(Position, 3, GL_FLOAT, GL_FALSE, size, 0));
                }

                if(Color != -1)
                {
                    GLCALL(glEnableVertexAttribArray,(Color));
    
                    GLCALL(glVertexAttribPointer,(Color,    4, GL_FLOAT, GL_FALSE, size, (const GLvoid*)(4*3)));
                }

                if( TexCoord != -1 )
                {
                    GLCALL(glEnableVertexAttribArray,(TexCoord));
                    
                    GLCALL(glVertexAttribPointer,(TexCoord, 2, GL_FLOAT, GL_FALSE, size, (const GLvoid*)(4*7)));
                }
              
            }
			break;
		}
	}
   

	bool AShaderGL::UpdateTexture2D( TextureSlots slot, shared_ptr<ATextureLib::ATexture> texture )
	{
		int index = 0;

		switch(slot)
		{
			case TextureSlots::Diffuse:
				index = 0;
				break;
			case TextureSlots::Lighting:
			case TextureSlots::Specular:
			case TextureSlots::Lookup:
				index = 1;
				break;
			case TextureSlots::ToonLookup:
			case TextureSlots::SkyLookup:
			case TextureSlots::SpecularMask:
				index = 2;
				break;
			case TextureSlots::CloudLighting:
			case TextureSlots::ShineTexture:
			case TextureSlots::HighlightTexture:
				index = 3;
				break;
		}

		{
			//NOTE: can assume its this
			//NOTE: easier would be to add an #ifdef around a ToTextureDX11() function.
			auto TextureGL = static_cast<ATextureLib::ATextureGL*>( texture.get() );
			auto TextureResource = TextureGL->GetTextureGL();

			//NOTE: need to cleanup memory.
            //gl
		

			//auto DeviceContext = GetDeviceContext();
			//DeviceContext->PSSetShaderResources(index,1,&ResourceView);
		} 
		
		return true;

	}


} // end namespace