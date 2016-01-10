#pragma once

#include "../opengl.h"

namespace overdrive {
	namespace render {
		// [NOTE] there are a couple that are sort-of missing here, but I feel it's reasonable to skip the ones with explicit bit-patterns
		//			e.g. GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV and GL_UNSIGNED_INT_10F_11F_11F_REV

		template <GLuint constant>
		struct ToType {};

		template <> struct ToType<GL_BYTE>				{ GLbyte type; };
		template <> struct ToType<GL_UNSIGNED_BYTE>		{ GLubyte type; };
		template <> struct ToType<GL_SHORT>				{ GLshort type; };
		template <> struct ToType<GL_UNSIGNED_SHORT>	{ GLushort type; };
		template <> struct ToType<GL_INT>				{ GLint type; };
		template <> struct ToType<GL_UNSIGNED_INT>		{ GLuint type; };
		template <> struct ToType<GL_FLOAT>				{ GLfloat type; };
		template <> struct ToType<GL_DOUBLE>			{ GLdouble type; };
		template <> struct ToType<GL_HALF_FLOAT>		{ GLhalf type; };
		template <> struct ToType<GL_FIXED>				{ GLfixed type; };

		template <typename T>
		struct ToValue {};

		template <> struct ToValue<GLbyte>		{ static const GLenum value = GL_BYTE; };
		template <> struct ToValue<GLubyte>		{ static const GLenum value = GL_UNSIGNED_BYTE; };
		template <> struct ToValue<GLshort>		{ static const GLenum value = GL_SHORT; };
		template <> struct ToValue<GLushort>	{ static const GLenum value = GL_UNSIGNED_SHORT; };
		template <> struct ToValue<GLint>		{ static const GLenum value = GL_INT; };
		template <> struct ToValue<GLuint>		{ static const GLenum value = GL_UNSIGNED_INT; };
		template <> struct ToValue<GLfloat>		{ static const GLenum value = GL_FLOAT; };
		template <> struct ToValue<GLdouble>	{ static const GLenum value = GL_DOUBLE; };
		//template <> struct ToValue<GLhalf>		{ static const GLenum value = GL_HALF_FLOAT; }; // GLhalf == GLshort so this can't work
		//template <> struct ToValue<GLfixed>		{ static const GLenum value = GL_FIXED; }; // GLfixed == GLint so this can't work
		
		size_t getTypeSize(GLenum value);

		GLenum asColorAttachment(GLuint index);
		GLenum asTextureUnit(GLuint index);
	}
}