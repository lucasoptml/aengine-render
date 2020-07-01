
#ifndef __ASHADER_H__
#define __ASHADER_H__


#include <string>
#include <memory>
#include <map>
#include <functional>
#include <math.h>

using std::string;
using std::shared_ptr;
using std::map;


namespace AEngineLib
{
	class ATexture;
}



namespace AShaderLib
{

	//
	// Struct Types
	//

    
    struct AVector2;
	struct AVector3;
    struct AVector4;

	struct AENGINE_API AMatrix4x4
	{
		union
		{
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[4][4];
		};


		inline float& get(int row, int col)
		{
			float* base = &_11;
			float* value = (base + 4*row + col);

			return *value;
		}

		inline float get(int row, int col) const
		{
			const float* base = &_11;
			const float* value = (base + 4*row + col);

			return *value;
		}
		
		string ToString();

		static AMatrix4x4 Multiply(const AMatrix4x4& a, const AMatrix4x4& b);
        static AVector4   Multiply(const AMatrix4x4& mx, const AVector4& v);
        
		static AMatrix4x4 Transpose(const AMatrix4x4& mx);
		static AMatrix4x4 Translation(float x, float y, float z);
		static AMatrix4x4 Scaling(float x, float y, float z);
		
        static AMatrix4x4 RotationX(float r);
		static AMatrix4x4 RotationY(float r);
		static AMatrix4x4 RotationZ(float r);
		
        static AMatrix4x4 LookAt(const AVector3& up, const AVector3& at, const AVector3& location);
		static AMatrix4x4 PerspectiveFOV(float FovAngleY, float AspectHByW, float NearZ, float FarZ);
		static AMatrix4x4 OrthoOffcenter(float ViewLeft, float ViewRight, float ViewTop, float ViewBottom, float NearZ, float FarZ);

	};


	struct AVector3
	{
		AVector3()
		{}

		AVector3(float inx, float iny, float inz)
			: x(inx), y(iny), z(inz)
		{}

		float x;
		float y;
		float z;

		inline static AVector3 Add(const AVector3& a, const AVector3& b)
		{
			return AVector3(a.x + b.x, a.y + b.y, a.z + b.z);
		}

		inline static AVector3 Subtract(const AVector3& a, const AVector3& b)
		{
			return AVector3(a.x - b.x, a.y - b.y, a.z - b.z);
		}

		inline static AVector3 Negate(const AVector3& v)
		{
			return AVector3(-v.x,-v.y,-v.z);
		}

		inline static AVector3 Normalize(const AVector3& v)
		{
			float ilength = InvLength(v);

			AVector3 result;

			result.x = v.x * ilength;
			result.y = v.y * ilength;
			result.z = v.z * ilength;

			return result;
		}

		inline static float InvLength(const AVector3& v)
		{
			return 1.0f / Length(v);
		}

		inline static float Length(const AVector3& v)
		{
			float lengthsquared = LengthSquared(v);

			return sqrt(lengthsquared);
		}

		inline static float LengthSquared(const AVector3& v)
		{
			return Dot(v,v);
		}

		inline static float Dot(const AVector3& a, const AVector3& b)
		{
			return a.x*b.x + a.y*b.y + a.z*b.z;
		}

		inline static AVector3 Cross(const AVector3& V1, const AVector3& V2)
		{
			AVector3 vResult;
			
			vResult.x = (V1.y * V2.z) - (V1.z * V2.y);
			vResult.y = (V1.z * V2.x) - (V1.x * V2.z);
			vResult.z = (V1.x * V2.y) - (V1.y * V2.x);

			return vResult;
    	}
	};

	struct AVector2
	{
		float x;
		float y;
	};

    struct AVector4
	{
        AVector4() : x(0), y(0), z(0), w(1)
        {
        }
        
        AVector4(float inx, float iny, float inz, float inw)
        : x(inx), y(iny), z(inz), w(inw)
        {
        }
        
		float x;
		float y;
        float z;
        float w;
	};
    
	struct AColor
	{
		AColor() 
			:
			r(1),
			g(1),
			b(1),
			a(1)
		{}

		AColor(float inr, float ing, float inb, float ina)
			: r(inr), g(ing), b(inb), a(ina)
		{
		}

		float r;
		float g;
		float b;
		float a;
	};


#ifdef AENGINE_DX11
	typedef __declspec(align(16)) AMatrix4x4 AMatrix4x4A;
	typedef __declspec(align(16)) AVector3   AVector3A;
#else
	typedef AMatrix4x4 AMatrix4x4A;
	typedef AVector3   AVector3A;
#endif

	//
	// Constants
	//

	struct GlobalConstants
	{
		AMatrix4x4A ViewMx;
		AMatrix4x4A ProjMx;

		float TimeSeconds;

		// Camera Related Variables
		AVector3 CameraWorldForward;

		AVector3A CameraWorldRight;

		AVector3A CameraWorldUp;
		AVector3A CameraWorldPosition;

		float ViewportWidth;
		float ViewportHeight;

		float NearClipping; 
		float FarClipping; 


	};
	
	struct ObjectConstants
	{
		ObjectConstants()
			: 
			Fatness(0)
		{
		}

		AMatrix4x4 WorldMx;
		AColor     DrawColor;

		float Fatness;

		float _pad0;
		float _pad1;
		float _pad2;
	};

	struct AnimationConstants
	{
		AMatrix4x4 SkinPalette[60];
	};

	struct OptionalConstants
	{
		float Factor;
		float Frequency;
	};


	enum VertexFormats
	{
		Canvas,
		Actor,
		CanvasByte,
		Animation
	};

	enum TextureSlots
	{
		Diffuse,
		Lighting,
		Specular,
		Lookup,
		ToonLookup,
		SkyLookup,
		SpecularMask,
		CloudLighting,
		ShineTexture,
		HighlightTexture
	};

	struct ActorVertex
	{
		AVector3 Position;
		AVector3 Normal;
		AVector2 TexCoord;
	};

	struct AnimationVertex
	{
		AVector3 Position;
		AVector3 Normal;
		AVector2 TexCoord;
	};

	struct CanvasVertex
	{
		AVector3 Position;
		AColor   Color;
		AVector2 TexCoord;
	};

	struct CanvasByteVertex
	{
		AVector3 Position;
		unsigned char R;
		unsigned char G;
		unsigned char B;
		unsigned char A;
		AVector2 TexCoord;
	};

	// NOTE: execution looks like this:
	// 1. Get Shader
	// 2. Set Vertex Format (for rendering)
	// 3. Update Constants
	// 4. Update Special Constants (animation, options)
	// 5. Apply (set shader for rendering, could set vertex format too)
	class AENGINE_API AShader
	{
	public:
		AShader(string name)
			: Name(name)
		{
		}

		//NOTE: will create an effect of a certain implementation
		static shared_ptr<AShader> GetShader(string shadername, VertexFormats format);

		//
		// Member Functions
		//

		virtual bool UpdateTexture2D(TextureSlots slot, shared_ptr<AEngineLib::ATexture> texture)
		{
			return false;
		}

		//
		// Constant Buffer Based Updates
		//

		static void StaticUpdateGlobalConstants(const GlobalConstants& globals)
		{
			Globals = globals;
			bChangedGlobals = true;
		}

		bool UpdateGlobalConstants()
		{
			return UpdateGlobalConstants(Globals);
		}

		virtual bool UpdateGlobalConstants(const GlobalConstants& globals)
		{
			return false;
		}

		virtual bool UpdateObjectConstants(const ObjectConstants& constants)
		{
			return false;
		}

		virtual bool UpdateAnimationConstants(const AnimationConstants& constants)
		{
			return false;
		}
		
		virtual bool UpdateOptionalConstants(const OptionalConstants& constants)
		{
			return false;
		}

		//NOTE: when we need additional constant types, just need to add another function.

		// apply the effect before rendering
		virtual void Apply()
		{
			
		}

		static void Log(string s)
		{
			// Write errors to log
			if(LogFunction)
			{
				LogFunction(s);
			}
		}

		static void SetLogFunction( std::function< void(string) > fcn )
		{
			LogFunction = fcn;
		}

		static const GlobalConstants& StaticGetGlobalConstants()
		{
			return Globals;
		}

	protected:

		static GlobalConstants Globals;
		static bool			   bChangedGlobals;
		
	private:

		string Name;

		// List of loaded shaders
		static map< string, shared_ptr<AShader> > Shaders;

		// Log function
		static std::function< void(string) >      LogFunction;
	};

}


#endif