

#include "AEngineLib.h"
//#include "AShader.h"

#include <algorithm>

#if AENGINE_DX11 == 1
	#include "DX11/AShaderDX11.h"
#elif AENGINE_GL == 1
    #include "GL/AShaderGL.h"
#endif

using namespace AEngineLib;

namespace AShaderLib
{
	map<string,shared_ptr<AShader>> AShader::Shaders     = map< string, shared_ptr<AShader> >();
	
	std::function<void(string)>     AShader::LogFunction = nullptr;
	
	GlobalConstants AShader::Globals;
	bool            AShader::bChangedGlobals = false;

	// ToLower with new foreach syntax
	inline void convertToLower(string s)
	{
		string out = s;

		for( char& c : s )
		{
			c = tolower(c);
		};
	}

	shared_ptr<AShader> AShader::GetShader(string name, VertexFormats format)
	{
		convertToLower(name);

		auto found = Shaders.find(name);

		if(found != Shaders.end())
		{
			return found->second;
		}

		shared_ptr<AShader> neweffect;

		// Create a new effect given the implementation
#if AENGINE_DX11 == 1
		neweffect = AShaderDX11::CreateShaderDX11(name,format);
#elif AENGINE_GL == 1
        neweffect = AShaderGL::CreateShaderGL(name,format);
#else
		neweffect = shared_ptr<AShader>( new AShader(name) );
#endif

		if(neweffect)
		{
			Shaders[name] = neweffect;
		}

		return neweffect;
	}


	//
	// Matrix Math
	//

		AMatrix4x4 AMatrix4x4::Multiply(const AMatrix4x4& M1, const AMatrix4x4& M2)
		{
			AMatrix4x4 mResult;

			float x = M1.m[0][0];
			float y = M1.m[0][1];
			float z = M1.m[0][2];
			float w = M1.m[0][3];
			// Perform the operation on the first row
			mResult.m[0][0] = (M2.m[0][0]*x)+(M2.m[1][0]*y)+(M2.m[2][0]*z)+(M2.m[3][0]*w);
			mResult.m[0][1] = (M2.m[0][1]*x)+(M2.m[1][1]*y)+(M2.m[2][1]*z)+(M2.m[3][1]*w);
			mResult.m[0][2] = (M2.m[0][2]*x)+(M2.m[1][2]*y)+(M2.m[2][2]*z)+(M2.m[3][2]*w);
			mResult.m[0][3] = (M2.m[0][3]*x)+(M2.m[1][3]*y)+(M2.m[2][3]*z)+(M2.m[3][3]*w);
			// Repeat for all the other rows
			x = M1.m[1][0];
			y = M1.m[1][1];
			z = M1.m[1][2];
			w = M1.m[1][3];
			mResult.m[1][0] = (M2.m[0][0]*x)+(M2.m[1][0]*y)+(M2.m[2][0]*z)+(M2.m[3][0]*w);
			mResult.m[1][1] = (M2.m[0][1]*x)+(M2.m[1][1]*y)+(M2.m[2][1]*z)+(M2.m[3][1]*w);
			mResult.m[1][2] = (M2.m[0][2]*x)+(M2.m[1][2]*y)+(M2.m[2][2]*z)+(M2.m[3][2]*w);
			mResult.m[1][3] = (M2.m[0][3]*x)+(M2.m[1][3]*y)+(M2.m[2][3]*z)+(M2.m[3][3]*w);
			x = M1.m[2][0];
			y = M1.m[2][1];
			z = M1.m[2][2];
			w = M1.m[2][3];
			mResult.m[2][0] = (M2.m[0][0]*x)+(M2.m[1][0]*y)+(M2.m[2][0]*z)+(M2.m[3][0]*w);
			mResult.m[2][1] = (M2.m[0][1]*x)+(M2.m[1][1]*y)+(M2.m[2][1]*z)+(M2.m[3][1]*w);
			mResult.m[2][2] = (M2.m[0][2]*x)+(M2.m[1][2]*y)+(M2.m[2][2]*z)+(M2.m[3][2]*w);
			mResult.m[2][3] = (M2.m[0][3]*x)+(M2.m[1][3]*y)+(M2.m[2][3]*z)+(M2.m[3][3]*w);
			x = M1.m[3][0];
			y = M1.m[3][1];
			z = M1.m[3][2];
			w = M1.m[3][3];
			mResult.m[3][0] = (M2.m[0][0]*x)+(M2.m[1][0]*y)+(M2.m[2][0]*z)+(M2.m[3][0]*w);
			mResult.m[3][1] = (M2.m[0][1]*x)+(M2.m[1][1]*y)+(M2.m[2][1]*z)+(M2.m[3][1]*w);
			mResult.m[3][2] = (M2.m[0][2]*x)+(M2.m[1][2]*y)+(M2.m[2][2]*z)+(M2.m[3][2]*w);
			mResult.m[3][3] = (M2.m[0][3]*x)+(M2.m[1][3]*y)+(M2.m[2][3]*z)+(M2.m[3][3]*w);

			return mResult;
		}

        AVector4 AMatrix4x4::Multiply(const AMatrix4x4& mx, const AVector4& v)
        {
            AVector4 result;
            
            result.x = mx._11 * v.x + mx._21 * v.y + mx._31 * v.z + mx._41 * v.w;
            result.y = mx._12 * v.x + mx._22 * v.y + mx._32 * v.z + mx._42 * v.w;
            result.z = mx._13 * v.x + mx._23 * v.y + mx._33 * v.z + mx._43 * v.w;
            result.w = mx._14 * v.x + mx._24 * v.y + mx._34 * v.z + mx._44 * v.w;
            
            return result;
            
        }
    
    
		AMatrix4x4 AMatrix4x4::Transpose(const AMatrix4x4& mx)
		{
			AMatrix4x4 result;

			result.get(0,0) = mx.get(0,0);
			result.get(0,1) = mx.get(1,0);
			result.get(0,2) = mx.get(2,0);
			result.get(0,3) = mx.get(3,0);

			result.get(1,0) = mx.get(0,1);
			result.get(1,1) = mx.get(1,1);
			result.get(1,2) = mx.get(2,1);
			result.get(1,3) = mx.get(3,1);

			result.get(2,0) = mx.get(0,2);
			result.get(2,1) = mx.get(1,2);
			result.get(2,2) = mx.get(2,2);
			result.get(2,3) = mx.get(3,2);

			result.get(3,0) = mx.get(0,3);
			result.get(3,1) = mx.get(1,3);
			result.get(3,2) = mx.get(2,3);
			result.get(3,3) = mx.get(3,3);

			return result;
		}

		AMatrix4x4 AMatrix4x4::Translation(float x, float y, float z)
		{
			AMatrix4x4 result;

			result.m[0][0] = 1.0f;
			result.m[0][1] = 0.0f;
			result.m[0][2] = 0.0f;
			result.m[0][3] = 0.0f;
		
			result.m[1][0] = 0.0f;
			result.m[1][1] = 1.0f;
			result.m[1][2] = 0.0f;
			result.m[1][3] = 0.0f;
		
			result.m[2][0] = 0.0f;
			result.m[2][1] = 0.0f;
			result.m[2][2] = 1.0f;
			result.m[2][3] = 0.0f;
		
			result.m[3][0] = x;
			result.m[3][1] = y;
			result.m[3][2] = z;
			result.m[3][3] = 1.0f;

			return result;
		}

		AMatrix4x4 AMatrix4x4::Scaling(float x, float y, float z)
		{
			AMatrix4x4 result;

			result.m[0][0] = x;
			result.m[0][1] = 0.0f;
			result.m[0][2] = 0.0f;
			result.m[0][3] = 0.0f;
		
			result.m[1][0] = 0.0f;
			result.m[1][1] = y;
			result.m[1][2] = 0.0f;
			result.m[1][3] = 0.0f;
		
			result.m[2][0] = 0.0f;
			result.m[2][1] = 0.0f;
			result.m[2][2] = z;
			result.m[2][3] = 0.0f;
		
			result.m[3][0] = 0.0f;
			result.m[3][1] = 0.0f;
			result.m[3][2] = 0.0f;
			result.m[3][3] = 1.0f;

			return result;
		}

		AMatrix4x4 AMatrix4x4::RotationX(float Angle)
		{
			float    fSinAngle;
			float    fCosAngle;
			//XMScalarSinCos(&fSinAngle, &fCosAngle, Angle);

			fSinAngle = sin(Angle);
			fCosAngle = cos(Angle);

			AMatrix4x4 M;
			M.m[0][0] = 1.0f;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = fCosAngle;
			M.m[1][2] = fSinAngle;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = -fSinAngle;
			M.m[2][2] = fCosAngle;
			M.m[2][3] = 0.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = 0.0f;
			M.m[3][3] = 1.0f;
			return M;

			//return result;
		}

		AMatrix4x4 AMatrix4x4::RotationY(float Angle)
		{
			float    fSinAngle;
			float    fCosAngle;
			//XMScalarSinCos(&fSinAngle, &fCosAngle, Angle);

			fSinAngle = sin(Angle);
			fCosAngle = cos(Angle);

			AMatrix4x4 M;
			M.m[0][0] = fCosAngle;
			M.m[0][1] = 0.0f;
			M.m[0][2] = -fSinAngle;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = 1.0f;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = fSinAngle;
			M.m[2][1] = 0.0f;
			M.m[2][2] = fCosAngle;
			M.m[2][3] = 0.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = 0.0f;
			M.m[3][3] = 1.0f;

			return M;
		}

		AMatrix4x4 AMatrix4x4::RotationZ(float Angle)
		{
			float    fSinAngle;
			float    fCosAngle;
			//XMScalarSinCos(&fSinAngle, &fCosAngle, Angle);
			fSinAngle = sin(Angle);
			fCosAngle = cos(Angle);

			AMatrix4x4 M;
			M.m[0][0] = fCosAngle;
			M.m[0][1] = fSinAngle;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = -fSinAngle;
			M.m[1][1] = fCosAngle;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = 1.0f;
			M.m[2][3] = 0.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = 0.0f;
			M.m[3][3] = 1.0f;
			return M;
		}

		AMatrix4x4 AMatrix4x4::LookAt(const AVector3& UpDirection, const AVector3& FocusPosition, const AVector3& EyePosition)
		{
			AVector3 NegEyeDirection = AVector3::Subtract(EyePosition, FocusPosition);
			//return XMMatrixLookToLH(EyePosition, NegEyeDirection, UpDirection);

	

			AVector3 R2 = AVector3::Normalize(NegEyeDirection);

			AVector3 R0 = AVector3::Cross(UpDirection, R2);
			R0 = AVector3::Normalize(R0);

			AVector3 R1 = AVector3::Cross(R2, R0);

			AVector3 NegEyePosition = AVector3::Negate(EyePosition);

			auto D0 = AVector3::Dot(R0, NegEyePosition);
			auto D1 = AVector3::Dot(R1, NegEyePosition);
			auto D2 = AVector3::Dot(R2, NegEyePosition);

			AMatrix4x4 M;

			//NOTE: not sure how to compute these...

			M._11 = R0.x;
			M._12 = R0.y;
			M._13 = R0.z;
			M._14 = D0;

			M._21 = R1.x;
			M._22 = R1.y;
			M._23 = R1.z;
			M._24 = D1;
			
			M._31 = R2.x;
			M._32 = R2.y;
			M._33 = R2.z;
			M._34 = D2;

			M._41 = 0.0f;
			M._42 = 0.0f;
			M._43 = 0.0f;
			M._44 = 1.0f;

			//M.r[0] = XMVectorSelect(D0, R0, g_XMSelect1110.v);
			//M.r[1] = XMVectorSelect(D1, R1, g_XMSelect1110.v);
			//M.r[2] = XMVectorSelect(D2, R2, g_XMSelect1110.v);
			//M.r[3] = g_XMIdentityR3.v;

			M = Transpose(M);

			return M;
		}

		AMatrix4x4 AMatrix4x4::PerspectiveFOV(float FovAngleY, float AspectHByW, float NearZ, float FarZ)
		{

			float    SinFov;
			float    CosFov;
				
			float angle = 0.5f * FovAngleY;
			SinFov = sin(angle);
			CosFov = cos(angle);
				
			//XMScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

			float Height = CosFov / SinFov;
			float Width = Height / AspectHByW;
			float fRange = FarZ / (NearZ-FarZ);

			AMatrix4x4 M;
			M.m[0][0] = Width;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = Height;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = fRange;
			M.m[2][3] = -1.0f;

			M.m[3][0] = 0.0f;
			M.m[3][1] = 0.0f;
			M.m[3][2] = fRange * NearZ;
			M.m[3][3] = 0.0f;
			

			return M;
		}

		AMatrix4x4 AMatrix4x4::OrthoOffcenter(float ViewLeft, float ViewRight, float ViewTop, float ViewBottom, float NearZ, float FarZ)
		{
			//AMatrix4x4 result;

			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = 1.0f / (NearZ-FarZ);

			AMatrix4x4 M;
			M.m[0][0] = ReciprocalWidth + ReciprocalWidth;
			M.m[0][1] = 0.0f;
			M.m[0][2] = 0.0f;
			M.m[0][3] = 0.0f;

			M.m[1][0] = 0.0f;
			M.m[1][1] = ReciprocalHeight + ReciprocalHeight;
			M.m[1][2] = 0.0f;
			M.m[1][3] = 0.0f;

			M.m[2][0] = 0.0f;
			M.m[2][1] = 0.0f;
			M.m[2][2] = fRange;
			M.m[2][3] = 0.0f;

			M._41 = -(ViewLeft + ViewRight) * ReciprocalWidth;
			M._42 = -(ViewTop + ViewBottom) * ReciprocalHeight;
			M._43 = fRange * NearZ;
			M._44 = 1.0f;

			return M;
		}

		string AMatrix4x4::ToString()
		{
			string mx = "[\n";

			mx += "[";
			mx += ALog::ConvertToString(_11);
			mx += ",";
			mx += ALog::ConvertToString(_12);
			mx += ",";
			mx += ALog::ConvertToString(_13);
			mx += ",";
			mx += ALog::ConvertToString(_14);
			mx += "]\n";

			mx += "[";
			mx += ALog::ConvertToString(_21);
			mx += ",";
			mx += ALog::ConvertToString(_22);
			mx += ",";
			mx += ALog::ConvertToString(_23);
			mx += ",";
			mx += ALog::ConvertToString(_24);
			mx += "]\n";

			mx += "[";
			mx += ALog::ConvertToString(_31);
			mx += ",";
			mx += ALog::ConvertToString(_32);
			mx += ",";
			mx += ALog::ConvertToString(_33);
			mx += ",";
			mx += ALog::ConvertToString(_34);
			mx += "]\n";

			mx += "[";
			mx += ALog::ConvertToString(_41);
			mx += ",";
			mx += ALog::ConvertToString(_42);
			mx += ",";
			mx += ALog::ConvertToString(_43);
			mx += ",";
			mx += ALog::ConvertToString(_44);
			mx += "]\n";

			mx += "\n]";

			return mx;
		}




}





