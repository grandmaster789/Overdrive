#ifndef OVERDRIVE_APP_RENDERTEST_H
#define OVERDRIVE_APP_RENDERTEST_H

#include "app/application.h"
#include "render/shaderprogram.h"
#include "render/vertexarray.h"
#include "render/vertexbuffer.h"
#include "input/keyboard.h"
#include "opengl.h"

namespace overdrive {
	namespace app {
		class RenderTest :
			public Application,
			public core::MessageHandler<input::Keyboard::OnKeyPress>
		{
		public:
			RenderTest();

			bool initialize();
			void update();
			void shutdown();

			void operator()(const input::Keyboard::OnKeyPress& kp);

		private:
			void createCube(float size = 1.0f);

			glm::mat4 mModel;
			glm::mat4 mView;
			glm::mat4 mProjection;

			float mAngle;

			render::ShaderProgram mShaderProgram;

			// openGL resources cannot be created until after engine initialization is completed!
			render::VertexBuffer<float, 3> mVertices;
			render::VertexBuffer<float, 3> mNormals;
			render::VertexBuffer<float, 2> mTexCoords;
			render::VertexBuffer<GLuint> mIndices;
			GLsizei mNumElements;

			render::VertexArray mVertexArray;
		};
	}
}

#endif
