#include "app/spinningcube.h"
#include "core/engine.h"
#include "video/video.h"
#include "math/conversions.h"
#include "opengl.h"
#include "util/checkGL.h"

namespace overdrive {
	namespace app {
		SpinningCube::SpinningCube():
			Application("Spinning Cube"),
			
			mVertexArray(0, &cleanupVertexArray, false),

			mVertices(0, &cleanupBuffer, false),
			mIndices(0, &cleanupBuffer, false),
			mNormals(0, &cleanupBuffer, false),
			mTexCoords(0, &cleanupBuffer, false)
		{
		}

		bool SpinningCube::initialize() {
			System::initialize();

			glClearColor(0.1f, 0.0f, 0.0f, 0.0f);
			glEnable(GL_DEPTH_TEST);

			mModel = glm::mat4(1.0f);
			mView = glm::lookAt(
				glm::vec3(0.0f, 0.0f, 2.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f)
			);
			mProjection = glm::mat4(1.0f); // identity projection

			auto video = mEngine->get<video::Video>();

			assert(!video->getWindowList().empty());

			int primaryWidth = video->getWindowList()[0]->getWidth();
			int primaryHeight = video->getWindowList()[0]->getHeight();

			mProjection = glm::perspective(
				math::toRadians(70.0f),
				static_cast<float>(primaryWidth) / static_cast<float>(primaryHeight),
				0.1f,
				100.0f
			); // 70'fov perspective projection

			glViewport(0, 0, primaryWidth, primaryHeight);

			mAngle = 0.01f;

			mShaderProgram.addShaderFile("shaders/diffuse.vs", render::VERTEX);
			mShaderProgram.addShaderFile("shaders/diffuse.fs", render::FRAGMENT);
			mShaderProgram.link();
			mShaderProgram.use();

			mShaderProgram.set("LightDiffuse", 1.0f, 1.0f, 1.0f);
			mShaderProgram.set("MaterialDiffuse", 0.9f, 0.5f, 0.3f);
			mShaderProgram.set("LightPosition", mView * glm::vec4(5.0f, 5.0f, 2.0f, 1.0f));

			createCube();

			mEngine->updateSystem(this, true, false);

			return true;
		}

		void SpinningCube::update() {
			mModel *= glm::rotate(mAngle, glm::vec3(0.1f, 1.0f, 0.0f));

			glm::mat4 modelView = mView * mModel;

			mShaderProgram.set("ModelViewMatrix", modelView);
			mShaderProgram.set("NormalMatrix", glm::mat3(
				glm::vec3(modelView[0]),
				glm::vec3(modelView[1]),
				glm::vec3(modelView[2])
			));
			mShaderProgram.set("ModelViewProjection", mProjection * modelView);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glBindVertexArray(mVertexArray);
			glDrawElements(GL_TRIANGLES, mNumElements, GL_UNSIGNED_INT, nullptr);
		}

		void SpinningCube::shutdown() {
			System::shutdown();
		}

		void SpinningCube::operator()(const input::Keyboard::OnKeyPress& kp) {
			switch (kp.mKey) {
			case GLFW_KEY_ESCAPE:
				mChannel.broadcast(core::Engine::OnStop());
				break;

			case GLFW_KEY_F1:
				mShaderProgram.printAttributes();
				break;

			case GLFW_KEY_F2:
				mShaderProgram.printUniforms();
				break;
			}
		}

		void SpinningCube::createCube(float size) {
			float halfSide = size * 0.5f;

			float vertices[] = {
				// Front
				-halfSide, -halfSide, halfSide,
				halfSide, -halfSide, halfSide,
				halfSide, halfSide, halfSide,
				-halfSide, halfSide, halfSide,

				// Right
				halfSide, -halfSide, halfSide,
				halfSide, -halfSide, -halfSide,
				halfSide, halfSide, -halfSide,
				halfSide, halfSide, halfSide,

				// Back
				-halfSide, -halfSide, -halfSide,
				-halfSide, halfSide, -halfSide,
				halfSide, halfSide, -halfSide,
				halfSide, -halfSide, -halfSide,

				// Left
				-halfSide, -halfSide, halfSide,
				-halfSide, halfSide, halfSide,
				-halfSide, halfSide, -halfSide,
				-halfSide, -halfSide, -halfSide,

				// Bottom
				-halfSide, -halfSide, halfSide,
				-halfSide, -halfSide, -halfSide,
				halfSide, -halfSide, -halfSide,
				halfSide, -halfSide, halfSide,

				// Top
				-halfSide, halfSide, halfSide,
				halfSide, halfSide, halfSide,
				halfSide, halfSide, -halfSide,
				-halfSide, halfSide, -halfSide
			};

			float normals[] = {
				// Front
				0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 1.0f,

				// Right
				1.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f,

				// Back
				0.0f, 0.0f, -1.0f,
				0.0f, 0.0f, -1.0f,
				0.0f, 0.0f, -1.0f,
				0.0f, 0.0f, -1.0f,

				// Left
				-1.0f, 0.0f, 0.0f,
				-1.0f, 0.0f, 0.0f,
				-1.0f, 0.0f, 0.0f,
				-1.0f, 0.0f, 0.0f,

				// Bottom
				0.0f, -1.0f, 0.0f,
				0.0f, -1.0f, 0.0f,
				0.0f, -1.0f, 0.0f,
				0.0f, -1.0f, 0.0f,

				// Top
				0.0f, 1.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 1.0f, 0.0f
			};

			float texCoords[] = {
				// Front
				0.0f, 0.0f,
				1.0f, 0.0f,
				1.0f, 1.0f,
				0.0f, 1.0f,

				// Right
				0.0f, 0.0f,
				1.0f, 0.0f,
				1.0f, 1.0f,
				0.0f, 1.0f,

				// Back
				0.0f, 0.0f,
				1.0f, 0.0f,
				1.0f, 1.0f,
				0.0f, 1.0f,

				// Left
				0.0f, 0.0f,
				1.0f, 0.0f,
				1.0f, 1.0f,
				0.0f, 1.0f,

				// Bottom
				0.0f, 0.0f,
				1.0f, 0.0f,
				1.0f, 1.0f,
				0.0f, 1.0f,

				// Top
				0.0f, 0.0f,
				1.0f, 0.0f,
				1.0f, 1.0f,
				0.0f, 1.0f
			};

			GLuint indices[] = {
				0, 1, 2,
				0, 2, 3,

				4, 5, 6,
				4, 6, 7,

				8, 9, 10,
				8, 10, 11,

				12, 13, 14,
				12, 14, 15,

				16, 17, 18,
				16, 18, 19,

				20, 21, 22,
				20, 22, 23
			};

			mNumElements = sizeof(indices) / sizeof(indices[0]);

			GLuint vao = 0;
			glGenVertexArrays(1, &vao);
			mVertexArray = std::move(vao);

			glBindVertexArray(mVertexArray);

			{
				GLuint buffers[4];
				glGenBuffers(4, buffers);
				mVertices = std::move(buffers[0]);
				mNormals = std::move(buffers[1]);
				mTexCoords = std::move(buffers[2]);
				mIndices = std::move(buffers[3]);

				glBindBuffer(GL_ARRAY_BUFFER, mVertices);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
				glVertexAttribPointer(
					0,			// location 0
					3,			// 3 elements
					GL_FLOAT,	// of type float
					GL_FALSE,	// not normalized
					0,			// no stride
					nullptr		// no corresponding buffer (data is bound)
				);
				glEnableVertexAttribArray(0); //enable vertex attribute 0 -- position

				glBindBuffer(GL_ARRAY_BUFFER, mNormals);
				glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
				glVertexAttribPointer(
					1,			// location 1
					3,			// 3 elements
					GL_FLOAT,	// of type float
					GL_FALSE,	// not normalized
					0,			// no stride
					nullptr		// no corresponding buffer (data is bound)
				);
				glEnableVertexAttribArray(1); //enable vertex attribute 1 -- normal

				glBindBuffer(GL_ARRAY_BUFFER, mTexCoords);
				glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
				glVertexAttribPointer(
					2,			// location 2
					2,			// 2 elements
					GL_FLOAT,	// of type float 
					GL_FALSE,	// not normalized
					0,			// no stride
					nullptr		// no corresponding buffer (data is bound)
				);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			}

			glBindVertexArray(0);
		}

		void SpinningCube::cleanupVertexArray(GLuint handle) {
			glDeleteVertexArrays(1, &handle);
		}

		void SpinningCube::cleanupBuffer(GLuint handle) {
			glDeleteBuffers(1, &handle);
		}
	}
}