#pragma once

#include <string>
#include "../opengl.h"
#include "../preprocessor.h"

namespace overdrive {
	namespace render {
		// [NOTE] If you want to use this, use the debug context hint when creating a window
		// http://docs.gl/gl4/glDebugMessageCallback
		// http://docs.gl/gl4/glDebugMessageControl
		class RenderDebug {
		private:
			RenderDebug();

		public:
			enum class eSource: GLenum {
				API				= GL_DEBUG_SOURCE_API,
				WINDOW_SYSTEM	= GL_DEBUG_SOURCE_WINDOW_SYSTEM, 
				SHADER_COMPILER = GL_DEBUG_SOURCE_SHADER_COMPILER, 
				THIRD_PARTY		= GL_DEBUG_SOURCE_THIRD_PARTY, 
				APPLICATION		= GL_DEBUG_SOURCE_APPLICATION, 
				OTHER			= GL_DEBUG_SOURCE_OTHER,
				DONTCARE		= GL_DONT_CARE
			};

			enum class eType: GLenum {
				ERROR_				= GL_DEBUG_TYPE_ERROR, 
				DEPRECATED			= GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, 
				UNDEFINED_BEHAVIOR	= GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, 
				PORABILITY			= GL_DEBUG_TYPE_PORTABILITY, 
				PERFORMANCE			= GL_DEBUG_TYPE_PERFORMANCE, 
				MARKER				= GL_DEBUG_TYPE_MARKER, 
				PUSH_GROUP			= GL_DEBUG_TYPE_PUSH_GROUP, 
				POP_GROUP			= GL_DEBUG_TYPE_POP_GROUP, 
				OTHER				= GL_DEBUG_TYPE_OTHER,
				DONTCARE			= GL_DONT_CARE
			};

			enum class eSeverity: GLenum {
				LOW			= GL_DEBUG_SEVERITY_LOW,
				MEDIUM		= GL_DEBUG_SEVERITY_MEDIUM, 
				HIGH		= GL_DEBUG_SEVERITY_HIGH,
				DONTCARE	= GL_DONT_CARE
			};

			static void init();

			static void enable();
			static void disable();

			static void enableLowSeverityMessages(bool toggle);
			static void setUserParam(void* p);
			static void clear();

		private:
			static void callback(
				GLenum source, 
				GLenum type, 
				GLuint id, 
				GLenum severity, 
				GLsizei length, 
				const GLchar* message, 
				const void* userParam
			);

			static void* mUserParam;
		};

		std::ostream& operator << (std::ostream& os, const RenderDebug::eSource& source);
		std::ostream& operator << (std::ostream& os, const RenderDebug::eType& type);
		std::ostream& operator << (std::ostream& os, const RenderDebug::eSeverity& severity);
	}
}
