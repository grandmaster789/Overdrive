#ifndef OVERDRIVE_OPENGL_H
#define OVERDRIVE_OPENGL_H

//include openGL-related headers, in the prescribed order

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtx/transform2.hpp>
#undef GLM_FORCE_RADIANS

#endif
