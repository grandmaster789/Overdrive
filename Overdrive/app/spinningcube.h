#ifndef OVERDRIVE_APP_SPINNINGCUBE_H
#define OVERDRIVE_APP_SPINNINGCUBE_H

#include "app/application.h"
#include "render/shaderprogram.h"
#include "input/keyboard.h"
#include "opengl.h"

namespace overdrive {
	namespace app {
		/*
		Really basic application, showing a spinning cube and shutting down when escape is pressed
		Minimal requirements:
		- System features (init/update/shutdown)
		- Channel features (MessageHandler, onKeyPress)
		- ShaderProgram (diffuse shaders)

		Does not use/require camera's, models, lights, etc.
		*/
		class SpinningCube :
			public Application,
			public core::MessageHandler<input::Keyboard::OnKeyPress>
		{
		public:
			SpinningCube();

			bool initialize();
			void update();
			void shutdown();

			void operator()(const input::Keyboard::OnKeyPress& kp);

		private:
			void createCube(float size = 1.0f);

			static void cleanupVertexArray(GLuint handle);
			static void cleanupBuffer(GLuint handle);

			glm::mat4 mModel;
			glm::mat4 mView;
			glm::mat4 mProjection;

			float mAngle;

			render::ShaderProgram mShaderProgram;

			GLsizei mNumElements;

			UniqueResource<GLuint, void(*)(GLuint)> mVertexArray;

			UniqueResource<GLuint, void(*)(GLuint)> mVertices;
			UniqueResource<GLuint, void(*)(GLuint)> mIndices;
			UniqueResource<GLuint, void(*)(GLuint)> mNormals;
			UniqueResource<GLuint, void(*)(GLuint)> mTexCoords;
		};
	}
}

#endif
