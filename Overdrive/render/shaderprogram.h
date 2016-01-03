#pragma once

#include <string>
#include <boost/filesystem.hpp>
#include <ostream>
#include <memory>
#include "../opengl.h"
#include "shader.h"

namespace overdrive {
	namespace render {
		// [NOTE] This uses lazy initialization so that the ShaderProgram can be created on the stack (without an active openGL context)
		//		  The program will be compiled, linked, verified etc upon the first time a shader is attached
		// [NOTE] The amount of uniforms and/or attributes is too low to justify using a mapping structure
		// [TODO] to/from binary formats should be possible

		// http://docs.gl/gl4/glCreateProgram
		class ShaderProgram {
		public:
			ShaderProgram();
			~ShaderProgram();

			ShaderProgram(const ShaderProgram&) = delete;
			ShaderProgram(ShaderProgram&&) = delete;
			ShaderProgram& operator = (const ShaderProgram&) = delete;
			ShaderProgram& operator = (ShaderProgram&&) = delete;

			GLuint getHandle() const;

			void attachShader(const std::string& source, eShaderType type);
			void loadShader(const boost::filesystem::path& p, eShaderType type);

			void link();
			bool isLinked() const;
			void validate();

			void bind();	// ~> use
			void unbind();

			void listUniforms() const;
			void listAttributes() const;
			
			Shader* getShader(eShaderType type) const;

			void bindAttributeLocation(GLuint id, const std::string& name);
			void bindFragDataLocation(GLuint id, const std::string& name);

		private:
			GLuint mHandle;
			std::unique_ptr<Shader> mShaders[6]; // fixed positions for all shader types

			bool mIsLinked;
		};

		// my god there are sooo many uniform types.... (some of which >4.1 or >4.2)
		// this list is derived from http://docs.gl/gl4/glGetActiveUniform
		// [NOTE] probably a documentation error, but both openGL.org and docs.gl show 'usampler2DArray' for GL_UNSIGNED_INT_SAMPLER_1D_ARRAY. 
		//		  I'm guessing that it should be 'usampler1DArray'
		enum class eUniformType: GLenum {
			FLOAT										= GL_FLOAT,
			FLOAT_VEC2									= GL_FLOAT_VEC2,
			FLOAT_VEC3									= GL_FLOAT_VEC3,
			FLOAT_VEC4									= GL_FLOAT_VEC4,
			
			DOUBLE										= GL_DOUBLE,
			DOUBLE_VEC2									= GL_DOUBLE_VEC2,
			DOUBLE_VEC3									= GL_DOUBLE_VEC3,
			DOUBLE_VEC4									= GL_DOUBLE_VEC4,
			
			INT											= GL_INT,
			INT_VEC2									= GL_INT_VEC2,
			INT_VEC3									= GL_INT_VEC3,
			INT_VEC4									= GL_INT_VEC4,
			
			UNSIGNED_INT								= GL_UNSIGNED_INT,
			UNSIGNED_INT_VEC2							= GL_UNSIGNED_INT_VEC2,
			UNSIGNED_INT_VEC3							= GL_UNSIGNED_INT_VEC3,
			UNSIGNED_INT_VEC4							= GL_UNSIGNED_INT_VEC4,
			
			BOOL										= GL_BOOL,
			BOOL_VEC2									= GL_BOOL_VEC2,
			BOOL_VEC3									= GL_BOOL_VEC3,
			BOOL_VEC4									= GL_BOOL_VEC4,

			FLOAT_MAT2									= GL_FLOAT_MAT2,
			FLOAT_MAT3									= GL_FLOAT_MAT3,
			FLOAT_MAT4									= GL_FLOAT_MAT4,
			FLOAT_MAT2x3								= GL_FLOAT_MAT2x3,
			FLOAT_MAT2x4								= GL_FLOAT_MAT2x4,
			FLOAT_MAT3x2								= GL_FLOAT_MAT3x2,
			FLOAT_MAT3x4								= GL_FLOAT_MAT3x4,
			FLOAT_MAT4x2								= GL_FLOAT_MAT4x2,
			FLOAT_MAT4x3								= GL_FLOAT_MAT4x3,

			DOUBLE_MAT2									= GL_DOUBLE_MAT2,
			DOUBLE_MAT3									= GL_DOUBLE_MAT3,
			DOUBLE_MAT4									= GL_DOUBLE_MAT4,
			DOUBLE_MAT2x3								= GL_DOUBLE_MAT2x3,
			DOUBLE_MAT2x4								= GL_DOUBLE_MAT2x4,
			DOUBLE_MAT3x2								= GL_DOUBLE_MAT3x2,
			DOUBLE_MAT3x4								= GL_DOUBLE_MAT3x4,
			DOUBLE_MAT4x2								= GL_DOUBLE_MAT4x2,
			DOUBLE_MAT4x3								= GL_DOUBLE_MAT4x3,

			SAMPLER_1D									= GL_SAMPLER_1D,
			SAMPLER_2D									= GL_SAMPLER_2D,
			SAMPLER_3D									= GL_SAMPLER_3D,
			SAMPLER_CUBE								= GL_SAMPLER_CUBE,
			SAMPLER_1D_SHADOW							= GL_SAMPLER_1D_SHADOW,
			SAMPLER_2D_SHADOW							= GL_SAMPLER_2D_SHADOW,
			SAMPLER_1D_ARRAY							= GL_SAMPLER_1D_ARRAY,
			SAMPLER_2D_ARRAY							= GL_SAMPLER_2D_ARRAY,
			SAMPLER_1D_ARRAY_SHADOW						= GL_SAMPLER_1D_ARRAY_SHADOW,
			SAMPLER_2D_ARRAY_SHADOW						= GL_SAMPLER_2D_ARRAY_SHADOW,
			SAMPLER_2D_MULTISAMPLE						= GL_SAMPLER_2D_MULTISAMPLE,
			SAMPLER_2D_MULTISAMPLE_ARRAY				= GL_SAMPLER_2D_MULTISAMPLE_ARRAY,
			SAMPLER_CUBE_SHADOW							= GL_SAMPLER_CUBE_SHADOW,
			SAMPLER_BUFFER								= GL_SAMPLER_BUFFER,
			SAMPLER_2D_RECT								= GL_SAMPLER_2D_RECT,
			SAMPLER_2D_RECT_SHADOW						= GL_SAMPLER_2D_RECT_SHADOW,

			INT_SAMPLER_1D								= GL_INT_SAMPLER_1D,
			INT_SAMPLER_2D								= GL_INT_SAMPLER_2D,
			INT_SAMPLER_3D								= GL_INT_SAMPLER_3D,
			INT_SAMPLER_CUBE							= GL_INT_SAMPLER_CUBE,
			INT_SAMPLER_1D_ARRAY						= GL_INT_SAMPLER_1D_ARRAY,
			INT_SAMPLER_2D_ARRAY						= GL_INT_SAMPLER_2D_ARRAY,
			INT_SAMPLER_2D_MULTISAMPLE					= GL_INT_SAMPLER_2D_MULTISAMPLE,
			INT_SAMPLER_2D_MULTISAMPLE_ARRAY			= GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY,
			INT_SAMPLER_BUFFER							= GL_INT_SAMPLER_BUFFER,
			INT_SAMPLER_2D_RECT							= GL_INT_SAMPLER_2D_RECT,

			UNSIGNED_INT_SAMPLER_1D						= GL_UNSIGNED_INT_SAMPLER_1D,
			UNSIGNED_INT_SAMPLER_2D						= GL_UNSIGNED_INT_SAMPLER_2D,
			UNSIGNED_INT_SAMPLER_3D						= GL_UNSIGNED_INT_SAMPLER_3D,
			UNSIGNED_INT_SAMPLER_CUBE					= GL_UNSIGNED_INT_SAMPLER_CUBE,
			UNSIGNED_INT_SAMPLER_1D_ARRAY				= GL_UNSIGNED_INT_SAMPLER_1D_ARRAY,
			UNSIGNED_INT_SAMPLER_2D_ARRAY				= GL_UNSIGNED_INT_SAMPLER_2D_ARRAY,
			UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE			= GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE,
			UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY	= GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY,
			UNSIGNED_INT_SAMPLER_BUFFER					= GL_UNSIGNED_INT_SAMPLER_BUFFER,
			UNSIGNED_INT_SAMPLER_2D_RECT				= GL_UNSIGNED_INT_SAMPLER_2D_RECT,

			IMAGE_1D									= GL_IMAGE_1D,
			IMAGE_2D									= GL_IMAGE_2D,
			IMAGE_3D									= GL_IMAGE_3D,
			IMAGE_2D_RECT								= GL_IMAGE_2D_RECT,
			IMAGE_CUBE									= GL_IMAGE_CUBE,
			IMAGE_BUFFER								= GL_IMAGE_BUFFER,
			IMAGE_1D_ARRAY								= GL_IMAGE_1D_ARRAY,
			IMAGE_2D_ARRAY								= GL_IMAGE_2D_ARRAY,
			IMAGE_2D_MULTISAMPLE						= GL_IMAGE_2D_MULTISAMPLE,
			IMAGE_2D_MULTISAMPLE_ARRAY					= GL_IMAGE_2D_MULTISAMPLE_ARRAY,

			INT_IMAGE_1D								= GL_INT_IMAGE_1D,
			INT_IMAGE_2D								= GL_INT_IMAGE_2D,
			INT_IMAGE_3D								= GL_INT_IMAGE_3D,
			INT_IMAGE_2D_RECT							= GL_INT_IMAGE_2D_RECT,
			INT_IMAGE_CUBE								= GL_INT_IMAGE_CUBE,
			INT_IMAGE_BUFFER							= GL_INT_IMAGE_BUFFER,
			INT_IMAGE_1D_ARRAY							= GL_INT_IMAGE_1D_ARRAY,
			INT_IMAGE_2D_ARRAY							= GL_INT_IMAGE_2D_ARRAY,
			INT_IMAGE_2D_MULTISAMPLE					= GL_INT_IMAGE_2D_MULTISAMPLE,
			INT_IMAGE_2D_MULTISAMPLE_ARRAY				= GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY,

			UNSIGNED_INT_IMAGE_1D						= GL_UNSIGNED_INT_IMAGE_1D,
			UNSIGNED_INT_IMAGE_2D						= GL_UNSIGNED_INT_IMAGE_2D,
			UNSIGNED_INT_IMAGE_3D						= GL_UNSIGNED_INT_IMAGE_3D,
			UNSIGNED_INT_IMAGE_2D_RECT					= GL_UNSIGNED_INT_IMAGE_2D_RECT,
			UNSIGNED_INT_IMAGE_CUBE						= GL_UNSIGNED_INT_IMAGE_CUBE,
			UNSIGNED_INT_IMAGE_BUFFER					= GL_UNSIGNED_INT_IMAGE_BUFFER,
			UNSIGNED_INT_IMAGE_1D_ARRAY					= GL_UNSIGNED_INT_IMAGE_1D_ARRAY,
			UNSIGNED_INT_IMAGE_2D_ARRAY					= GL_UNSIGNED_INT_IMAGE_2D_ARRAY,
			UNSIGNED_INT_IMAGE_2D_MULTISAMPLE			= GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE,
			UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY		= GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY,

			UNSIGNED_INT_ATOMIC_COUNTER					= GL_UNSIGNED_INT_ATOMIC_COUNTER
		};

		// http://docs.gl/gl4/glGetActiveAttrib
		enum class eAttributeType: GLenum {
			FLOAT				= GL_FLOAT,
			FLOAT_VEC2			= GL_FLOAT_VEC2,
			FLOAT_VEC3			= GL_FLOAT_VEC3,
			FLOAT_VEC4			= GL_FLOAT_VEC4,

			FLOAT_MAT2			= GL_FLOAT_MAT2,
			FLOAT_MAT3			= GL_FLOAT_MAT3,
			FLOAT_MAT4			= GL_FLOAT_MAT4,
			FLOAT_MAT2x3		= GL_FLOAT_MAT2x3,
			FLOAT_MAT2x4		= GL_FLOAT_MAT2x4,
			FLOAT_MAT3x2		= GL_FLOAT_MAT3x2,
			FLOAT_MAT3x4		= GL_FLOAT_MAT3x4,
			FLOAT_MAT4x2		= GL_FLOAT_MAT4x2,
			FLOAT_MAT4x3		= GL_FLOAT_MAT4x3,

			INT					= GL_INT,
			INT_VEC2			= GL_INT_VEC2,
			INT_VEC3			= GL_INT_VEC3,
			INT_VEC4			= GL_INT_VEC4,

			UNSIGNED_INT		= GL_UNSIGNED_INT,
			UNSIGNED_INT_VEC2	= GL_UNSIGNED_INT_VEC2,
			UNSIGNED_INT_VEC3	= GL_UNSIGNED_INT_VEC3,
			UNSIGNED_INT_VEC4	= GL_UNSIGNED_INT_VEC4,

			DOUBLE				= GL_DOUBLE,
			DOUBLE_VEC2			= GL_DOUBLE_VEC2,
			DOUBLE_VEC3			= GL_DOUBLE_VEC3,
			DOUBLE_VEC4			= GL_DOUBLE_VEC4,

			DOUBLE_MAT2			= GL_DOUBLE_MAT2,
			DOUBLE_MAT3			= GL_DOUBLE_MAT3,
			DOUBLE_MAT4			= GL_DOUBLE_MAT4,
			DOUBLE_MAT2x3		= GL_DOUBLE_MAT2x3,
			DOUBLE_MAT2x4		= GL_DOUBLE_MAT2x4,
			DOUBLE_MAT3x2		= GL_DOUBLE_MAT3x2,
			DOUBLE_MAT3x4		= GL_DOUBLE_MAT3x4,
			DOUBLE_MAT4x2		= GL_DOUBLE_MAT4x2,
			DOUBLE_MAT4x3		= GL_DOUBLE_MAT4x3
		};

		eUniformType toUniformType(GLenum value);
		eAttributeType toAttributeType(GLenum value);

		std::ostream& operator << (std::ostream& os, const ShaderProgram& program);
		std::ostream& operator << (std::ostream& os, const eUniformType& type);		// outputs the corresponding glsl type
		std::ostream& operator << (std::ostream& os, const eAttributeType& type);
	}
}