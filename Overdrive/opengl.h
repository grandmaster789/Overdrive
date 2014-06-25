#ifndef OVERDRIVE_OPENGL_H
#define OVERDRIVE_OPENGL_H

//include openGL-related headers, in the prescribed order

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#pragma warning(push)
#pragma warning(disable:4244) // conversion from 'GLuint' to 'GLfloat', possible loss of precision
	#include <oglplus/all.hpp>
#pragma warning(pop)

#endif
