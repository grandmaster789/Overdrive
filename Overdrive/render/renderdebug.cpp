#include "stdafx.h"
#include "renderdebug.h"
#include "../core/logger.h"

namespace overdrive {
	namespace render {
		namespace {
			RenderDebug::eSource fromSource(GLenum source) {
				switch (source) {
				case GL_DEBUG_SOURCE_API:				return RenderDebug::eSource::API;
				case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return RenderDebug::eSource::WINDOW_SYSTEM;
				case GL_DEBUG_SOURCE_SHADER_COMPILER:	return RenderDebug::eSource::SHADER_COMPILER;
				case GL_DEBUG_SOURCE_THIRD_PARTY:		return RenderDebug::eSource::THIRD_PARTY;
				case GL_DEBUG_SOURCE_APPLICATION:		return RenderDebug::eSource::APPLICATION;
				case GL_DEBUG_SOURCE_OTHER:				return RenderDebug::eSource::OTHER;
				case GL_DONT_CARE:						return RenderDebug::eSource::DONTCARE;
				default:
					throw std::runtime_error("unexpected source from openGL debug callback");
				}
			}

			RenderDebug::eType fromType(GLenum type) {
				switch (type) {
				case GL_DEBUG_TYPE_ERROR:				return RenderDebug::eType::ERROR_;
				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return RenderDebug::eType::DEPRECATED;
				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return RenderDebug::eType::UNDEFINED_BEHAVIOR;
				case GL_DEBUG_TYPE_PORTABILITY:			return RenderDebug::eType::PORABILITY;
				case GL_DEBUG_TYPE_PERFORMANCE:			return RenderDebug::eType::PERFORMANCE;
				case GL_DEBUG_TYPE_MARKER:				return RenderDebug::eType::MARKER;
				case GL_DEBUG_TYPE_PUSH_GROUP:			return RenderDebug::eType::PUSH_GROUP;
				case GL_DEBUG_TYPE_POP_GROUP:			return RenderDebug::eType::POP_GROUP;
				case GL_DEBUG_TYPE_OTHER:				return RenderDebug::eType::OTHER;
				case GL_DONT_CARE:						return RenderDebug::eType::DONTCARE;
				default:
					throw std::runtime_error("unexpected type from openGL debug callback");
				}
			}

			RenderDebug::eSeverity fromSeverity(GLenum severity) {
				switch (severity) {
				case GL_DEBUG_SEVERITY_LOW:		return RenderDebug::eSeverity::LOW;
				case GL_DEBUG_SEVERITY_MEDIUM:	return RenderDebug::eSeverity::MEDIUM;
				case GL_DEBUG_SEVERITY_HIGH:	return RenderDebug::eSeverity::HIGH;
				case GL_DONT_CARE:				return RenderDebug::eSeverity::DONTCARE;
				default:
					throw std::runtime_error("unexpected severity from openGL debug callback");
				}
			}
		}

		void* RenderDebug::mUserParam = nullptr;

		RenderDebug::RenderDebug() {
		}

		void RenderDebug::init() {
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(&RenderDebug::callback, nullptr);
		}

		void RenderDebug::enable() {
			glDebugMessageCallback(&RenderDebug::callback, mUserParam);
		}

		void RenderDebug::disable() {
			glDebugMessageCallback(nullptr, nullptr);
		}

		void RenderDebug::enableLowSeverityMessages(bool toggle) {
			glDebugMessageControl(
				GL_DONT_CARE,					// source
				GL_DONT_CARE,					// type
				GL_DEBUG_SEVERITY_LOW,			// severity
				0,								// array count
				nullptr,						// array ptr
				(toggle ? GL_TRUE : GL_FALSE)	// enabled flag
			);
		}

		void RenderDebug::setUserParam(void* p) {
			mUserParam = p;
			glDebugMessageCallback(&RenderDebug::callback, p);
		}

		void RenderDebug::clear() {
			int totalMessages;

			glGetIntegerv(GL_DEBUG_LOGGED_MESSAGES, &totalMessages);
			glGetDebugMessageLog(
				totalMessages, // count
				0, // buffer size
				nullptr, //sources
				nullptr, //types
				nullptr, // ids
				nullptr, // severities
				nullptr, // lengths
				nullptr // message log
			);
		}		

		void RenderDebug::callback(
			GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei, // length,
			const GLchar* message,
			const void* // userParam
		) {
			gLogDebug
				<< " Type: " << fromType(type)
				<< "; Source: " << fromSource(source)
				<< "; ID: " << id
				<< "; Severity: " << fromSeverity(severity)
				<< "\n" << message;
		}

		std::ostream& operator << (std::ostream& os, const RenderDebug::eSource& source) {
			switch (source) {
			case RenderDebug::eSource::API:				os << "API"; break;
			case RenderDebug::eSource::WINDOW_SYSTEM:	os << "Window system"; break;
			case RenderDebug::eSource::SHADER_COMPILER: os << "Shader compiler"; break;
			case RenderDebug::eSource::THIRD_PARTY:		os << "Third party"; break;
			case RenderDebug::eSource::APPLICATION:		os << "Application"; break;
			case RenderDebug::eSource::OTHER:			os << "Other"; break;
			case RenderDebug::eSource::DONTCARE:		os << "Don't care"; break;
			default:
				os << "Unknown source: " << static_cast<std::underlying_type<RenderDebug::eSource>::type>(source);
			}

			return os;
		}

		std::ostream& operator << (std::ostream& os, const RenderDebug::eType& type) {
			switch (type) {
			case RenderDebug::eType::ERROR_:				os << "Error"; break;
			case RenderDebug::eType::DEPRECATED:			os << "Deprecated behavior"; break;
			case RenderDebug::eType::UNDEFINED_BEHAVIOR:	os << "Undefined behavior"; break;
			case RenderDebug::eType::PORABILITY:			os << "Porability issue"; break;
			case RenderDebug::eType::PERFORMANCE:			os << "Performance issue"; break;
			case RenderDebug::eType::MARKER:				os << "Marker"; break;
			case RenderDebug::eType::PUSH_GROUP:			os << "Push group"; break;
			case RenderDebug::eType::POP_GROUP:				os << "Pop group"; break;
			case RenderDebug::eType::OTHER:					os << "Other"; break;
			case RenderDebug::eType::DONTCARE:				os << "Don't care"; break;
			default:
				os << "Unknown type: " << static_cast<std::underlying_type<RenderDebug::eType>::type>(type);
			}

			return os;
		}

		std::ostream& operator << (std::ostream& os, const RenderDebug::eSeverity& severity) {
			switch (severity) {
			case RenderDebug::eSeverity::LOW:		os << "Low"; break;
			case RenderDebug::eSeverity::MEDIUM:	os << "Medium"; break;
			case RenderDebug::eSeverity::HIGH:		os << "High"; break;
			case RenderDebug::eSeverity::DONTCARE:	os << "Don't care"; break;
			default:
				os << "Unknown severity: " << static_cast<std::underlying_type<RenderDebug::eType>::type>(severity);
			}
			return os;
		}
	}
}