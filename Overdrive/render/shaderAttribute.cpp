#include "stdafx.h"
#include "shaderattribute.h"

namespace overdrive {
	namespace render {
		namespace {
			enum eAttributeBits {
				REF_BY_VERTEX			= (0x1 << 0),
				REF_BY_FRAGMENT			= (0x1 << 1),
				REF_BY_GEOMETRY			= (0x1 << 2),
				REF_BY_TESS_CONTROL		= (0x1 << 3),
				REF_BY_TESS_EVALUATION	= (0x1 << 4),
				REF_BY_COMPUTE			= (0x1 << 5),
				IS_PER_PATCH			= (0x1 << 6)
			};
		}

		ShaderAttribute::ShaderAttribute(
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

					GL_REFERENCED_BY_VERTEX_SHADER,
					GL_REFERENCED_BY_FRAGMENT_SHADER,
					GL_REFERENCED_BY_GEOMETRY_SHADER,
					GL_REFERENCED_BY_TESS_CONTROL_SHADER,
					GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
					GL_REFERENCED_BY_COMPUTE_SHADER,

					GL_IS_PER_PATCH,
					GL_LOCATION_COMPONENT
				};
			*/

			mType = toAttributeType(static_cast<GLenum>(properties[1]));
			mLocation = properties[2];

			if (properties[3])
				mPackedData |= REF_BY_VERTEX;
			if (properties[4])
				mPackedData |= REF_BY_FRAGMENT;
			if (properties[5])
				mPackedData |= REF_BY_GEOMETRY;
			if (properties[6])
				mPackedData |= REF_BY_TESS_CONTROL;
			if (properties[7])
				mPackedData |= REF_BY_TESS_EVALUATION;
			if (properties[8])
				mPackedData |= REF_BY_COMPUTE;
			
			if (properties[9])
				mPackedData |= IS_PER_PATCH;

			mFirstComponentLocation = properties[10];
		}


		const std::string& ShaderAttribute::getName() const {
			return mName;
		}

		eAttributeType ShaderAttribute::getType() const {
			return mType;
		}

		GLint ShaderAttribute::getLocation() const {
			return mLocation;
		}

		GLint ShaderAttribute::getFirstComponentLocation() const {
			return mFirstComponentLocation;
		}

		bool ShaderAttribute::isReferencedBy(eShaderType shader) const {
			switch (shader) {
			case eShaderType::VERTEX:				
				return (mPackedData & eAttributeBits::REF_BY_VERTEX) != 0;

			case eShaderType::FRAGMENT:
				return (mPackedData & eAttributeBits::REF_BY_FRAGMENT) != 0;

			case eShaderType::GEOMETRY: 
				return (mPackedData & eAttributeBits::REF_BY_GEOMETRY) != 0;

			case eShaderType::TESSELATION_CONTROL: 
				return (mPackedData & eAttributeBits::REF_BY_TESS_CONTROL) != 0;

			case eShaderType::TESSELATION_EVAL: 
				return (mPackedData & eAttributeBits::REF_BY_TESS_EVALUATION) != 0;

			case eShaderType::COMPUTE:
				return (mPackedData & eAttributeBits::REF_BY_COMPUTE) != 0;

			default:
				throw ShaderException("Unsupported shader type");
			}
		}

		bool ShaderAttribute::isPerPatch() const {
			return (mPackedData & eAttributeBits::IS_PER_PATCH) != 0;
		}

		eAttributeType toAttributeType(GLenum value) {
			switch (value) {
			case GL_FLOAT: return eAttributeType::FLOAT;
			case GL_FLOAT_VEC2: return eAttributeType::FLOAT_VEC2;
			case GL_FLOAT_VEC3: return eAttributeType::FLOAT_VEC3;
			case GL_FLOAT_VEC4: return eAttributeType::FLOAT_VEC4;

			case GL_FLOAT_MAT2: return eAttributeType::FLOAT_MAT2;
			case GL_FLOAT_MAT3: return eAttributeType::FLOAT_MAT3;
			case GL_FLOAT_MAT4: return eAttributeType::FLOAT_MAT4;
			case GL_FLOAT_MAT2x3: return eAttributeType::FLOAT_MAT2x3;
			case GL_FLOAT_MAT2x4: return eAttributeType::FLOAT_MAT2x4;
			case GL_FLOAT_MAT3x2: return eAttributeType::FLOAT_MAT3x2;
			case GL_FLOAT_MAT3x4: return eAttributeType::FLOAT_MAT3x4;
			case GL_FLOAT_MAT4x2: return eAttributeType::FLOAT_MAT4x2;
			case GL_FLOAT_MAT4x3: return eAttributeType::FLOAT_MAT4x3;

			case GL_INT: return eAttributeType::INT;
			case GL_INT_VEC2: return eAttributeType::INT_VEC2;
			case GL_INT_VEC3: return eAttributeType::INT_VEC3;
			case GL_INT_VEC4: return eAttributeType::INT_VEC4;

			case GL_UNSIGNED_INT: return eAttributeType::UNSIGNED_INT;
			case GL_UNSIGNED_INT_VEC2: return eAttributeType::UNSIGNED_INT_VEC2;
			case GL_UNSIGNED_INT_VEC3: return eAttributeType::UNSIGNED_INT_VEC3;
			case GL_UNSIGNED_INT_VEC4: return eAttributeType::UNSIGNED_INT_VEC4;

			case GL_DOUBLE: return eAttributeType::DOUBLE;
			case GL_DOUBLE_VEC2: return eAttributeType::DOUBLE_VEC2;
			case GL_DOUBLE_VEC3: return eAttributeType::DOUBLE_VEC3;
			case GL_DOUBLE_VEC4: return eAttributeType::DOUBLE_VEC4;

			case GL_DOUBLE_MAT2: return eAttributeType::DOUBLE_MAT2;
			case GL_DOUBLE_MAT3: return eAttributeType::DOUBLE_MAT3;
			case GL_DOUBLE_MAT4: return eAttributeType::DOUBLE_MAT4;
			case GL_DOUBLE_MAT2x3: return eAttributeType::DOUBLE_MAT2x3;
			case GL_DOUBLE_MAT2x4: return eAttributeType::DOUBLE_MAT2x4;
			case GL_DOUBLE_MAT3x2: return eAttributeType::DOUBLE_MAT3x2;
			case GL_DOUBLE_MAT3x4: return eAttributeType::DOUBLE_MAT3x4;
			case GL_DOUBLE_MAT4x2: return eAttributeType::DOUBLE_MAT4x2;
			case GL_DOUBLE_MAT4x3: return eAttributeType::DOUBLE_MAT4x3;

			default:
				throw ShaderException("Unsupported attribute type");
			}
		}

		std::ostream& operator << (std::ostream& os, const ShaderAttribute& attribute) {
			os
				<< attribute.getName()
				<< " (" << attribute.getType() << ")"
				<< " => " << attribute.getLocation();

			return os;
		}

		std::ostream& operator << (std::ostream& os, const eAttributeType& type) {

			switch (type) {
			case eAttributeType::FLOAT: os << "FLOAT"; break;
			case eAttributeType::FLOAT_VEC2: os << "FLOAT_VEC2"; break;
			case eAttributeType::FLOAT_VEC3: os << "FLOAT_VEC3"; break;
			case eAttributeType::FLOAT_VEC4: os << "FLOAT_VEC4"; break;

			case eAttributeType::FLOAT_MAT2: os << "FLOAT_MAT2"; break;
			case eAttributeType::FLOAT_MAT3: os << "FLOAT_MAT3"; break;
			case eAttributeType::FLOAT_MAT4: os << "FLOAT_MAT4"; break;
			case eAttributeType::FLOAT_MAT2x3: os << "FLOAT_MAT2x3"; break;
			case eAttributeType::FLOAT_MAT2x4: os << "FLOAT_MAT2x4"; break;
			case eAttributeType::FLOAT_MAT3x2: os << "FLOAT_MAT3x2"; break;
			case eAttributeType::FLOAT_MAT3x4: os << "FLOAT_MAT3x4"; break;
			case eAttributeType::FLOAT_MAT4x2: os << "FLOAT_MAT4x2"; break;
			case eAttributeType::FLOAT_MAT4x3: os << "FLOAT_MAT4x3"; break;

			case eAttributeType::INT: os << "INT"; break;
			case eAttributeType::INT_VEC2: os << "INT_VEC2"; break;
			case eAttributeType::INT_VEC3: os << "INT_VEC3"; break;
			case eAttributeType::INT_VEC4: os << "INT_VEC4"; break;

			case eAttributeType::UNSIGNED_INT: os << "UNSIGNED_INT"; break;
			case eAttributeType::UNSIGNED_INT_VEC2: os << "UNSIGNED_INT_VEC2"; break;
			case eAttributeType::UNSIGNED_INT_VEC3: os << "UNSIGNED_INT_VEC3"; break;
			case eAttributeType::UNSIGNED_INT_VEC4: os << "UNSIGNED_INT_VEC4"; break;

			case eAttributeType::DOUBLE: os << "DOUBLE"; break;
			case eAttributeType::DOUBLE_VEC2: os << "DOUBLE_VEC2"; break;
			case eAttributeType::DOUBLE_VEC3: os << "DOUBLE_VEC3"; break;
			case eAttributeType::DOUBLE_VEC4: os << "DOUBLE_VEC4"; break;

			case eAttributeType::DOUBLE_MAT2: os << "DOUBLE_MAT2"; break;
			case eAttributeType::DOUBLE_MAT3: os << "DOUBLE_MAT3"; break;
			case eAttributeType::DOUBLE_MAT4: os << "DOUBLE_MAT4"; break;
			case eAttributeType::DOUBLE_MAT2x3: os << "DOUBLE_MAT2x3"; break;
			case eAttributeType::DOUBLE_MAT2x4: os << "DOUBLE_MAT2x4"; break;
			case eAttributeType::DOUBLE_MAT3x2: os << "DOUBLE_MAT3x2"; break;
			case eAttributeType::DOUBLE_MAT3x4: os << "DOUBLE_MAT3x4"; break;
			case eAttributeType::DOUBLE_MAT4x2: os << "DOUBLE_MAT4x2"; break;
			case eAttributeType::DOUBLE_MAT4x3: os << "DOUBLE_MAT4x3"; break;

			default:
				os << "Unknown attribute type: " << static_cast<std::underlying_type<eAttributeType>::type>(type);
			}

			return os;
		}
	}
}