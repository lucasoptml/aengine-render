

#include "AShader.h"

#include <vector>
#include <string>

#include <OpenGLES/ES2/gl.h>

namespace AShaderLib
{
	using std::vector;

	class AShaderGL : public AShader
	{
	public:
		AShaderGL(string name, VertexFormats format, GLuint vshader, GLuint pshader, GLuint program);

		virtual ~AShaderGL();

		static shared_ptr<AShader> CreateShaderGL(string name, VertexFormats format);

		virtual bool UpdateTexture2D(TextureSlots  slot, shared_ptr<ATextureLib::ATexture> texture);

		virtual bool UpdateGlobalConstants   (const GlobalConstants&    constants);
		virtual bool UpdateObjectConstants   (const ObjectConstants&    constants);
		virtual bool UpdateAnimationConstants(const AnimationConstants& constants);
		virtual bool UpdateOptionalConstants (const OptionalConstants&  constants);

		virtual void EnableAlpha();
		virtual void DisableAlpha();

		// apply the effect before rendering
		virtual void Apply();

	private:

        void SetupVertexFormat(VertexFormats format);
		void ApplyVertexFormat(VertexFormats format);

        VertexFormats VertexFormat;

        GLuint        VertexShader;
        GLuint        PixelShader;
        GLuint        Program;

        //TODO: add all the constant location cache here.
	};

}
