#include "app/rendertest.h"
#include "core/engine.h"
#include "math/conversions.h"
#include "video/video.h"
#include "util/checkGL.h"
#include "opengl.h"


namespace overdrive {
	namespace app {
		RenderTest::RenderTest():
			Application("RenderTest")
		{
		}

		bool RenderTest::initialize() {
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

		void RenderTest::update() {
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

			glBindVertexArray(mVertexArray.getHandle());
			glDrawElements(GL_TRIANGLES, mIndices.getNumElements(), GL_UNSIGNED_INT, nullptr);
		}

		void RenderTest::shutdown() {
			System::shutdown();
		}

		void RenderTest::operator()(const input::Keyboard::OnKeyPress& kp) {
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

		void RenderTest::createCube(float size) {
			using namespace overdrive::render;

			float halfSide = size * 0.5f;

			std::vector<float> vertices;
			vertices.reserve(6 * 4 * 3); // 6 quads with 4 vertices each that have 3 values
			
			//front
			push(vertices, -halfSide, -halfSide, halfSide);
			push(vertices, halfSide, -halfSide, halfSide);
			push(vertices, halfSide, halfSide, halfSide);
			push(vertices, -halfSide, halfSide, halfSide);
			
			// Right
			push(vertices, halfSide, -halfSide, halfSide);
			push(vertices, halfSide, -halfSide, -halfSide);
			push(vertices, halfSide, halfSide, -halfSide);
			push(vertices, halfSide, halfSide, halfSide);

			// Back
			push(vertices, -halfSide, -halfSide, -halfSide);
			push(vertices, -halfSide, halfSide, -halfSide);
			push(vertices, halfSide, halfSide, -halfSide);
			push(vertices, halfSide, -halfSide, -halfSide);

			// Left
			push(vertices, -halfSide, -halfSide, halfSide);
			push(vertices, -halfSide, halfSide, halfSide);
			push(vertices, -halfSide, halfSide, -halfSide);
			push(vertices, -halfSide, -halfSide, -halfSide);

			// Bottom
			push(vertices, -halfSide, -halfSide, halfSide);
			push(vertices, -halfSide, -halfSide, -halfSide);
			push(vertices, halfSide, -halfSide, -halfSide);
			push(vertices, halfSide, -halfSide, halfSide);

			// Top
			push(vertices, -halfSide, halfSide, halfSide);
			push(vertices, halfSide, halfSide, halfSide);
			push(vertices, halfSide, halfSide, -halfSide);
			push(vertices, -halfSide, halfSide, -halfSide);

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

			mVertices.setData(vertices);
			mNormals.setData(normals, sizeof(normals));
			mTexCoords.setData(texCoords, sizeof(texCoords));
			mIndices.setData(indices, sizeof(indices));

			mVertexArray.bindAttribute(0, mVertices);
			mVertexArray.bindAttribute(1, mNormals);
			mVertexArray.bindAttribute(2, mTexCoords);
			mVertexArray.bindElements(mIndices);
		}
	}
}