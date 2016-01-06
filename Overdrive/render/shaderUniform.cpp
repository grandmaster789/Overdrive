#include "stdafx.h"
#include "shaderUniform.h"

namespace overdrive {
	namespace render {
		namespace {
			enum eUniformBits {
				REF_BY_VERTEX = (0x1 << 0),
				REF_BY_FRAGMENT = (0x1 << 1),
				REF_BY_GEOMETRY = (0x1 << 2),
				REF_BY_TESS_CONTROL = (0x1 << 3),
				REF_BY_TESS_EVALUATION = (0x1 << 4),
				REF_BY_COMPUTE = (0x1 << 5),
				IS_ROW_MAJOR = (0x1 << 6),
			};
		}

		ShaderUniform::ShaderUniform(
			const std::string& name, 
			GLint* properties
		):
			mName(name),
			mPackedData(0)
		{
			/*
				The properties need to be queried thusly:

				GLenum properties[] = {
					GL_NAME_LENGTH, // not used in this class
					GL_TYPE,
					GL_LOCATION,

					GL_ARRAY_SIZE,
					GL_ARRAY_STRIDE,
					GL_MATRIX_STRIDE,

					GL_OFFSET,
					GL_BLOCK_INDEX,
					GL_ATOMIC_COUNTER_BUFFER_INDEX,
					
					GL_IS_ROW_MAJOR,
										
					GL_REFERENCED_BY_VERTEX_SHADER,
					GL_REFERENCED_BY_FRAGMENT_SHADER,
					GL_REFERENCED_BY_GEOMETRY_SHADER,
					GL_REFERENCED_BY_TESS_CONTROL_SHADER,
					GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
					GL_REFERENCED_BY_COMPUTE_SHADER
				};
			*/

			mType = toUniformType(static_cast<GLenum>(properties[1]));
			mLocation = properties[2];
			
			mArraySize = properties[3];
			mArrayStride = properties[4];
			mMatrixStride = properties[5];

			mOffset = properties[6];
			mBlockIndex = properties[7];
			mAtomicCounterBufferIndex = properties[8];

			if (properties[9])
				mPackedData |= IS_ROW_MAJOR;

			if (properties[10])
				mPackedData |= REF_BY_VERTEX;
			if (properties[11])
				mPackedData |= REF_BY_FRAGMENT;
			if (properties[12])
				mPackedData |= REF_BY_GEOMETRY;
			if (properties[13])
				mPackedData |= REF_BY_TESS_CONTROL;
			if (properties[14])
				mPackedData |= REF_BY_TESS_EVALUATION;
			if (properties[15])
				mPackedData |= REF_BY_COMPUTE;
		}

		const std::string& ShaderUniform::getName() const {
			return mName;
		}

		eUniformType ShaderUniform::getType() const {
			return mType;
		}

		GLint ShaderUniform::getLocation() const {
			return mLocation;
		}

		GLint ShaderUniform::getArraySize() const {
			return mArraySize;
		}

		GLint ShaderUniform::getArrayStride() const {
			return mArrayStride;
		}

		GLint ShaderUniform::getMatrixStride() const {
			return mMatrixStride;
		}

		GLint ShaderUniform::getOffset() const {
			return mOffset;
		}
	
		GLint ShaderUniform::getBlockIndex() const {
			return mBlockIndex;
		}

		GLint ShaderUniform::getAtomicCounterBufferIndex() const {
			return mAtomicCounterBufferIndex;
		}

		bool ShaderUniform::isReferencedBy(eShaderType shader) const {
			switch (shader) {
			case eShaderType::VERTEX:
				return (mPackedData & REF_BY_VERTEX) != 0;

			case eShaderType::FRAGMENT:
				return (mPackedData & REF_BY_FRAGMENT) != 0;

			case eShaderType::GEOMETRY:
				return (mPackedData & REF_BY_GEOMETRY) != 0;

			case eShaderType::TESSELATION_CONTROL:
				return (mPackedData & REF_BY_TESS_CONTROL) != 0;

			case eShaderType::TESSELATION_EVAL:
				return (mPackedData & REF_BY_TESS_EVALUATION) != 0;

			case eShaderType::COMPUTE:
				return (mPackedData & REF_BY_COMPUTE) != 0;

			default:
				throw ShaderException("Unsupported shader type");
			}
		}

		bool ShaderUniform::isRowMajor() const {
			return (mPackedData & IS_ROW_MAJOR) != 0;
		}
		
		eUniformType toUniformType(GLenum value) {
			switch (value) {
			case GL_FLOAT: return eUniformType::FLOAT;
			case GL_FLOAT_VEC2: return eUniformType::FLOAT_VEC2;
			case GL_FLOAT_VEC3: return eUniformType::FLOAT_VEC3;
			case GL_FLOAT_VEC4: return eUniformType::FLOAT_VEC4;

			case GL_DOUBLE: return eUniformType::DOUBLE;
			case GL_DOUBLE_VEC2: return eUniformType::DOUBLE_VEC2;
			case GL_DOUBLE_VEC3: return eUniformType::DOUBLE_VEC3;
			case GL_DOUBLE_VEC4: return eUniformType::DOUBLE_VEC4;

			case GL_INT: return eUniformType::INT;
			case GL_INT_VEC2: return eUniformType::INT_VEC2;
			case GL_INT_VEC3: return eUniformType::INT_VEC3;
			case GL_INT_VEC4: return eUniformType::INT_VEC4;

			case GL_UNSIGNED_INT: return eUniformType::UNSIGNED_INT;
			case GL_UNSIGNED_INT_VEC2: return eUniformType::UNSIGNED_INT_VEC2;
			case GL_UNSIGNED_INT_VEC3: return eUniformType::UNSIGNED_INT_VEC3;
			case GL_UNSIGNED_INT_VEC4: return eUniformType::UNSIGNED_INT_VEC4;

			case GL_BOOL: return eUniformType::BOOL;
			case GL_BOOL_VEC2: return eUniformType::BOOL_VEC2;
			case GL_BOOL_VEC3: return eUniformType::BOOL_VEC3;
			case GL_BOOL_VEC4: return eUniformType::BOOL_VEC4;

			case GL_FLOAT_MAT2: return eUniformType::FLOAT_MAT2;
			case GL_FLOAT_MAT3: return eUniformType::FLOAT_MAT3;
			case GL_FLOAT_MAT4: return eUniformType::FLOAT_MAT4;
			case GL_FLOAT_MAT2x3: return eUniformType::FLOAT_MAT2x3;
			case GL_FLOAT_MAT2x4: return eUniformType::FLOAT_MAT2x4;
			case GL_FLOAT_MAT3x2: return eUniformType::FLOAT_MAT3x2;
			case GL_FLOAT_MAT3x4: return eUniformType::FLOAT_MAT3x4;
			case GL_FLOAT_MAT4x2: return eUniformType::FLOAT_MAT4x2;
			case GL_FLOAT_MAT4x3: return eUniformType::FLOAT_MAT4x3;

			case GL_DOUBLE_MAT2: return eUniformType::DOUBLE_MAT2;
			case GL_DOUBLE_MAT3: return eUniformType::DOUBLE_MAT3;
			case GL_DOUBLE_MAT4: return eUniformType::DOUBLE_MAT4;
			case GL_DOUBLE_MAT2x3: return eUniformType::DOUBLE_MAT2x3;
			case GL_DOUBLE_MAT2x4: return eUniformType::DOUBLE_MAT2x4;
			case GL_DOUBLE_MAT3x2: return eUniformType::DOUBLE_MAT3x2;
			case GL_DOUBLE_MAT3x4: return eUniformType::DOUBLE_MAT3x4;
			case GL_DOUBLE_MAT4x2: return eUniformType::DOUBLE_MAT4x2;
			case GL_DOUBLE_MAT4x3: return eUniformType::DOUBLE_MAT4x3;

			case GL_SAMPLER_1D: return eUniformType::SAMPLER_1D;
			case GL_SAMPLER_2D: return eUniformType::SAMPLER_2D;
			case GL_SAMPLER_3D: return eUniformType::SAMPLER_3D;
			case GL_SAMPLER_CUBE: return eUniformType::SAMPLER_CUBE;
			case GL_SAMPLER_1D_SHADOW: return eUniformType::SAMPLER_1D_SHADOW;
			case GL_SAMPLER_2D_SHADOW: return eUniformType::SAMPLER_2D_SHADOW;
			case GL_SAMPLER_1D_ARRAY: return eUniformType::SAMPLER_1D_ARRAY;
			case GL_SAMPLER_2D_ARRAY: return eUniformType::SAMPLER_2D_ARRAY;
			case GL_SAMPLER_1D_ARRAY_SHADOW: return eUniformType::SAMPLER_1D_ARRAY_SHADOW;
			case GL_SAMPLER_2D_ARRAY_SHADOW: return eUniformType::SAMPLER_2D_ARRAY_SHADOW;
			case GL_SAMPLER_2D_MULTISAMPLE: return eUniformType::SAMPLER_2D_MULTISAMPLE;
			case GL_SAMPLER_2D_MULTISAMPLE_ARRAY: return eUniformType::SAMPLER_2D_MULTISAMPLE_ARRAY;
			case GL_SAMPLER_CUBE_SHADOW: return eUniformType::SAMPLER_CUBE_SHADOW;
			case GL_SAMPLER_BUFFER: return eUniformType::SAMPLER_BUFFER;
			case GL_SAMPLER_2D_RECT: return eUniformType::SAMPLER_2D_RECT;
			case GL_SAMPLER_2D_RECT_SHADOW: return eUniformType::SAMPLER_2D_RECT_SHADOW;

			case GL_INT_SAMPLER_1D: return eUniformType::INT_SAMPLER_1D;
			case GL_INT_SAMPLER_2D: return eUniformType::INT_SAMPLER_2D;
			case GL_INT_SAMPLER_3D: return eUniformType::INT_SAMPLER_3D;
			case GL_INT_SAMPLER_CUBE: return eUniformType::INT_SAMPLER_CUBE;
			case GL_INT_SAMPLER_1D_ARRAY: return eUniformType::INT_SAMPLER_1D_ARRAY;
			case GL_INT_SAMPLER_2D_ARRAY: return eUniformType::INT_SAMPLER_2D_ARRAY;
			case GL_INT_SAMPLER_2D_MULTISAMPLE: return eUniformType::INT_SAMPLER_2D_MULTISAMPLE;
			case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return eUniformType::INT_SAMPLER_2D_MULTISAMPLE_ARRAY;
			case GL_INT_SAMPLER_BUFFER: return eUniformType::INT_SAMPLER_BUFFER;
			case GL_INT_SAMPLER_2D_RECT: return eUniformType::INT_SAMPLER_2D_RECT;

			case GL_UNSIGNED_INT_SAMPLER_1D: return eUniformType::UNSIGNED_INT_SAMPLER_1D;
			case GL_UNSIGNED_INT_SAMPLER_2D: return eUniformType::UNSIGNED_INT_SAMPLER_2D;
			case GL_UNSIGNED_INT_SAMPLER_3D: return eUniformType::UNSIGNED_INT_SAMPLER_3D;
			case GL_UNSIGNED_INT_SAMPLER_CUBE: return eUniformType::UNSIGNED_INT_SAMPLER_CUBE;
			case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY: return eUniformType::UNSIGNED_INT_SAMPLER_1D_ARRAY;
			case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY: return eUniformType::UNSIGNED_INT_SAMPLER_2D_ARRAY;
			case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: return eUniformType::UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE;
			case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return eUniformType::UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY;
			case GL_UNSIGNED_INT_SAMPLER_BUFFER: return eUniformType::UNSIGNED_INT_SAMPLER_BUFFER;
			case GL_UNSIGNED_INT_SAMPLER_2D_RECT: return eUniformType::UNSIGNED_INT_SAMPLER_2D_RECT;

			case GL_IMAGE_1D: return eUniformType::IMAGE_1D;
			case GL_IMAGE_2D: return eUniformType::IMAGE_2D;
			case GL_IMAGE_3D: return eUniformType::IMAGE_3D;
			case GL_IMAGE_2D_RECT: return eUniformType::IMAGE_2D_RECT;
			case GL_IMAGE_CUBE: return eUniformType::IMAGE_CUBE;
			case GL_IMAGE_BUFFER: return eUniformType::IMAGE_BUFFER;
			case GL_IMAGE_1D_ARRAY: return eUniformType::IMAGE_1D_ARRAY;
			case GL_IMAGE_2D_ARRAY: return eUniformType::IMAGE_2D_ARRAY;
			case GL_IMAGE_2D_MULTISAMPLE: return eUniformType::IMAGE_2D_MULTISAMPLE;
			case GL_IMAGE_2D_MULTISAMPLE_ARRAY: return eUniformType::IMAGE_2D_MULTISAMPLE_ARRAY;

			case GL_INT_IMAGE_1D: return eUniformType::INT_IMAGE_1D;
			case GL_INT_IMAGE_2D: return eUniformType::INT_IMAGE_2D;
			case GL_INT_IMAGE_3D: return eUniformType::INT_IMAGE_3D;
			case GL_INT_IMAGE_2D_RECT: return eUniformType::INT_IMAGE_2D_RECT;
			case GL_INT_IMAGE_CUBE: return eUniformType::INT_IMAGE_CUBE;
			case GL_INT_IMAGE_BUFFER: return eUniformType::INT_IMAGE_BUFFER;
			case GL_INT_IMAGE_1D_ARRAY: return eUniformType::INT_IMAGE_1D_ARRAY;
			case GL_INT_IMAGE_2D_ARRAY: return eUniformType::INT_IMAGE_2D_ARRAY;
			case GL_INT_IMAGE_2D_MULTISAMPLE: return eUniformType::INT_IMAGE_2D_MULTISAMPLE;
			case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY: return eUniformType::INT_IMAGE_2D_MULTISAMPLE_ARRAY;

			case GL_UNSIGNED_INT_IMAGE_1D: return eUniformType::UNSIGNED_INT_IMAGE_1D;
			case GL_UNSIGNED_INT_IMAGE_2D: return eUniformType::UNSIGNED_INT_IMAGE_2D;
			case GL_UNSIGNED_INT_IMAGE_3D: return eUniformType::UNSIGNED_INT_IMAGE_3D;
			case GL_UNSIGNED_INT_IMAGE_2D_RECT: return eUniformType::UNSIGNED_INT_IMAGE_2D_RECT;
			case GL_UNSIGNED_INT_IMAGE_CUBE: return eUniformType::UNSIGNED_INT_IMAGE_CUBE;
			case GL_UNSIGNED_INT_IMAGE_BUFFER: return eUniformType::UNSIGNED_INT_IMAGE_BUFFER;
			case GL_UNSIGNED_INT_IMAGE_1D_ARRAY: return eUniformType::UNSIGNED_INT_IMAGE_1D_ARRAY;
			case GL_UNSIGNED_INT_IMAGE_2D_ARRAY: return eUniformType::UNSIGNED_INT_IMAGE_2D_ARRAY;
			case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE: return eUniformType::UNSIGNED_INT_IMAGE_2D_MULTISAMPLE;
			case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY: return eUniformType::UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY;

			case GL_UNSIGNED_INT_ATOMIC_COUNTER: return eUniformType::UNSIGNED_INT_ATOMIC_COUNTER;

			default:
				throw ShaderException("Unsupported uniform type");
			}
		}

		std::ostream& operator << (std::ostream& os, const ShaderUniform& uniform) {
			os
				<< uniform.getName()
				<< " (" << uniform.getType() << ")"
				<< " => " << uniform.getLocation();

			return os;
		}

		std::ostream& operator << (std::ostream& os, const eUniformType& type) {
			switch (type) {
			case eUniformType::FLOAT: os << "float"; break;
			case eUniformType::FLOAT_VEC2: os << "vec2"; break;
			case eUniformType::FLOAT_VEC3: os << "vec3"; break;
			case eUniformType::FLOAT_VEC4: os << "vec4"; break;

			case eUniformType::DOUBLE: os << "double"; break;
			case eUniformType::DOUBLE_VEC2: os << "dvec2"; break;
			case eUniformType::DOUBLE_VEC3: os << "dvec3"; break;
			case eUniformType::DOUBLE_VEC4: os << "dvec4"; break;

			case eUniformType::INT: os << "int"; break;
			case eUniformType::INT_VEC2: os << "ivec2"; break;
			case eUniformType::INT_VEC3: os << "ivec3"; break;
			case eUniformType::INT_VEC4: os << "ivec4"; break;

			case eUniformType::UNSIGNED_INT: os << "unsigned int"; break;
			case eUniformType::UNSIGNED_INT_VEC2: os << "uvec2"; break;
			case eUniformType::UNSIGNED_INT_VEC3: os << "uvec3"; break;
			case eUniformType::UNSIGNED_INT_VEC4: os << "uvec4"; break;

			case eUniformType::BOOL: os << "bool"; break;
			case eUniformType::BOOL_VEC2: os << "bvec2"; break;
			case eUniformType::BOOL_VEC3: os << "bvec3"; break;
			case eUniformType::BOOL_VEC4: os << "bvec4"; break;

			case eUniformType::FLOAT_MAT2: os << "mat2"; break;
			case eUniformType::FLOAT_MAT3: os << "mat3"; break;
			case eUniformType::FLOAT_MAT4: os << "mat4"; break;
			case eUniformType::FLOAT_MAT2x3: os << "mat2x3"; break;
			case eUniformType::FLOAT_MAT2x4: os << "mat2x4"; break;
			case eUniformType::FLOAT_MAT3x2: os << "mat3x2"; break;
			case eUniformType::FLOAT_MAT3x4: os << "mat3x4"; break;
			case eUniformType::FLOAT_MAT4x2: os << "mat4x2"; break;
			case eUniformType::FLOAT_MAT4x3: os << "mat4x3"; break;

			case eUniformType::DOUBLE_MAT2: os << "dmat2"; break;
			case eUniformType::DOUBLE_MAT3: os << "dmat3"; break;
			case eUniformType::DOUBLE_MAT4: os << "dmat4"; break;
			case eUniformType::DOUBLE_MAT2x3: os << "dmat2x3"; break;
			case eUniformType::DOUBLE_MAT2x4: os << "dmat2x4"; break;
			case eUniformType::DOUBLE_MAT3x2: os << "dmat3x2"; break;
			case eUniformType::DOUBLE_MAT3x4: os << "dmat3x4"; break;
			case eUniformType::DOUBLE_MAT4x2: os << "dmat4x2"; break;
			case eUniformType::DOUBLE_MAT4x3: os << "dmat4x3"; break;

			case eUniformType::SAMPLER_1D: os << "sampler1D"; break;
			case eUniformType::SAMPLER_2D: os << "sampler2D"; break;
			case eUniformType::SAMPLER_3D: os << "sampler3D"; break;
			case eUniformType::SAMPLER_CUBE: os << "samplerCube"; break;
			case eUniformType::SAMPLER_1D_SHADOW: os << "sampler1DShadow"; break;
			case eUniformType::SAMPLER_2D_SHADOW: os << "sampler2DShadow"; break;
			case eUniformType::SAMPLER_1D_ARRAY: os << "sampler1DArray"; break;
			case eUniformType::SAMPLER_2D_ARRAY: os << "sampler2DArray"; break;
			case eUniformType::SAMPLER_1D_ARRAY_SHADOW: os << "sampler1DArrayShadow"; break;
			case eUniformType::SAMPLER_2D_ARRAY_SHADOW: os << "sampler2DArrayShadow"; break;
			case eUniformType::SAMPLER_2D_MULTISAMPLE: os << "sampler2DMS"; break;
			case eUniformType::SAMPLER_2D_MULTISAMPLE_ARRAY: os << "sampler2DMSArray"; break;
			case eUniformType::SAMPLER_CUBE_SHADOW: os << "samplerCubeShadow"; break;
			case eUniformType::SAMPLER_BUFFER: os << "samplerBuffer"; break;
			case eUniformType::SAMPLER_2D_RECT: os << "sampler2DRect"; break;
			case eUniformType::SAMPLER_2D_RECT_SHADOW: os << "sampler2DRectShadow"; break;

			case eUniformType::INT_SAMPLER_1D: os << "isampler1D"; break;
			case eUniformType::INT_SAMPLER_2D: os << "isampler2D"; break;
			case eUniformType::INT_SAMPLER_3D: os << "isampler3D"; break;
			case eUniformType::INT_SAMPLER_CUBE: os << "isamplerCube"; break;
			case eUniformType::INT_SAMPLER_1D_ARRAY: os << "isampler1DArray"; break;
			case eUniformType::INT_SAMPLER_2D_ARRAY: os << "isampler2DArray"; break;
			case eUniformType::INT_SAMPLER_2D_MULTISAMPLE: os << "isampler2DMS"; break;
			case eUniformType::INT_SAMPLER_2D_MULTISAMPLE_ARRAY: os << "isampler2DMSArray"; break;
			case eUniformType::INT_SAMPLER_BUFFER: os << "isamplerBuffer"; break;
			case eUniformType::INT_SAMPLER_2D_RECT: os << "isampler2DRect"; break;

			case eUniformType::UNSIGNED_INT_SAMPLER_1D: os << "usampler1D"; break;
			case eUniformType::UNSIGNED_INT_SAMPLER_2D: os << "usampler2D"; break;
			case eUniformType::UNSIGNED_INT_SAMPLER_3D: os << "usampler3D"; break;
			case eUniformType::UNSIGNED_INT_SAMPLER_CUBE: os << "usamplerCube"; break;
			case eUniformType::UNSIGNED_INT_SAMPLER_1D_ARRAY: os << "usampler1DArray"; break; // [NOTE] weirdly this is listed as usampler2Darray both on docs.gl and opengl.org
			case eUniformType::UNSIGNED_INT_SAMPLER_2D_ARRAY: os << "usampler2DArray"; break;
			case eUniformType::UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: os << "usampler2DMS"; break;
			case eUniformType::UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: os << "usampler2DMSArray"; break;
			case eUniformType::UNSIGNED_INT_SAMPLER_BUFFER: os << "usamplerBuffer"; break;
			case eUniformType::UNSIGNED_INT_SAMPLER_2D_RECT: os << "usampler2DRect"; break;

			case eUniformType::IMAGE_1D: os << "image1D"; break;
			case eUniformType::IMAGE_2D: os << "image2D"; break;
			case eUniformType::IMAGE_3D: os << "image3D"; break;
			case eUniformType::IMAGE_2D_RECT: os << "image2DRect"; break;
			case eUniformType::IMAGE_CUBE: os << "imageCube"; break;
			case eUniformType::IMAGE_BUFFER: os << "imageBuffer"; break;
			case eUniformType::IMAGE_1D_ARRAY: os << "image1DArray"; break;
			case eUniformType::IMAGE_2D_ARRAY: os << "image2DArray"; break;
			case eUniformType::IMAGE_2D_MULTISAMPLE: os << "image2DMS"; break;
			case eUniformType::IMAGE_2D_MULTISAMPLE_ARRAY: os << "image2DMSArray"; break;

			case eUniformType::INT_IMAGE_1D: os << "iimage1D"; break;
			case eUniformType::INT_IMAGE_2D: os << "iimage2D"; break;
			case eUniformType::INT_IMAGE_3D: os << "iimage3D"; break;
			case eUniformType::INT_IMAGE_2D_RECT: os << "iimage2DRect"; break;
			case eUniformType::INT_IMAGE_CUBE: os << "iimageCube"; break;
			case eUniformType::INT_IMAGE_BUFFER: os << "iimageBuffer"; break;
			case eUniformType::INT_IMAGE_1D_ARRAY: os << "iimage1DArray"; break;
			case eUniformType::INT_IMAGE_2D_ARRAY: os << "iimage2DArray"; break;
			case eUniformType::INT_IMAGE_2D_MULTISAMPLE: os << "iimage2DMS"; break;
			case eUniformType::INT_IMAGE_2D_MULTISAMPLE_ARRAY: os << "iimage2DMSArray"; break;

			case eUniformType::UNSIGNED_INT_IMAGE_1D: os << "uimage1D"; break;
			case eUniformType::UNSIGNED_INT_IMAGE_2D: os << "uimage2D"; break;
			case eUniformType::UNSIGNED_INT_IMAGE_3D: os << "uimage3D"; break;
			case eUniformType::UNSIGNED_INT_IMAGE_2D_RECT: os << "uimage2DRect"; break;
			case eUniformType::UNSIGNED_INT_IMAGE_CUBE: os << "uimageCube"; break;
			case eUniformType::UNSIGNED_INT_IMAGE_BUFFER: os << "uimageBuffer"; break;
			case eUniformType::UNSIGNED_INT_IMAGE_1D_ARRAY: os << "uimage1DArray"; break;
			case eUniformType::UNSIGNED_INT_IMAGE_2D_ARRAY: os << "uimage2DArray"; break;
			case eUniformType::UNSIGNED_INT_IMAGE_2D_MULTISAMPLE: os << "uimage2DMS"; break;
			case eUniformType::UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY: os << "uimage2DMSArray"; break;

			case eUniformType::UNSIGNED_INT_ATOMIC_COUNTER: os << "atomic_uint"; break;

			default:
				os << "Unknown uniform type: " << static_cast<std::underlying_type<eUniformType>::type>(type);
			}

			return os;
		}
	}
}