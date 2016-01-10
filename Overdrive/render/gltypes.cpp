#include "stdafx.h"
#include "gltypes.h"
#include <stdexcept>

namespace overdrive {
	namespace render {
		GLenum asColorAttachment(GLuint index) {
			switch (index) {
			case 0: return GL_COLOR_ATTACHMENT0;
			case 1: return GL_COLOR_ATTACHMENT1;
			case 2: return GL_COLOR_ATTACHMENT2;
			case 3: return GL_COLOR_ATTACHMENT3;
			case 4: return GL_COLOR_ATTACHMENT4;
			case 5: return GL_COLOR_ATTACHMENT5;
			case 6: return GL_COLOR_ATTACHMENT6;
			case 7: return GL_COLOR_ATTACHMENT7;

			case 8: return GL_COLOR_ATTACHMENT8;
			case 9: return GL_COLOR_ATTACHMENT9;
			case 10: return GL_COLOR_ATTACHMENT10;
			case 11: return GL_COLOR_ATTACHMENT11;
			case 12: return GL_COLOR_ATTACHMENT12;
			case 13: return GL_COLOR_ATTACHMENT13;
			case 14: return GL_COLOR_ATTACHMENT14;
			case 15: return GL_COLOR_ATTACHMENT15;

			default:
				throw std::runtime_error("Unsupported color attachment index");
			}
		}

		GLenum asTextureUnit(GLuint index) {
			switch (index) {
			case 0:  return GL_TEXTURE0;
			case 1:  return GL_TEXTURE1;
			case 2:  return GL_TEXTURE2;
			case 3:  return GL_TEXTURE3;
			case 4:  return GL_TEXTURE4;
			case 5:  return GL_TEXTURE5;
			case 6:  return GL_TEXTURE6;
			case 7:  return GL_TEXTURE7;
					 
			case 8:  return GL_TEXTURE8;
			case 9:  return GL_TEXTURE9;
			case 10: return GL_TEXTURE10;
			case 11: return GL_TEXTURE11;
			case 12: return GL_TEXTURE12;
			case 13: return GL_TEXTURE13;
			case 14: return GL_TEXTURE14;
			case 15: return GL_TEXTURE15;

			case 16: return GL_TEXTURE16;
			case 17: return GL_TEXTURE17;
			case 18: return GL_TEXTURE18;
			case 19: return GL_TEXTURE19;
			case 20: return GL_TEXTURE20;
			case 21: return GL_TEXTURE21;
			case 22: return GL_TEXTURE22;
			case 23: return GL_TEXTURE23;

			case 24: return GL_TEXTURE24;
			case 25: return GL_TEXTURE25;
			case 26: return GL_TEXTURE26;
			case 27: return GL_TEXTURE27;
			case 28: return GL_TEXTURE28;
			case 29: return GL_TEXTURE29;
			case 30: return GL_TEXTURE30;
			case 31: return GL_TEXTURE31;
				
			default:
				throw std::runtime_error("Unsupported texture unit index");
			}
		}
	}
}
