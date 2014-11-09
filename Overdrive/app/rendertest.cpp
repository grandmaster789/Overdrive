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

			mVertices = std::make_unique<render::VertexBuffer>();
			mNormals = std::make_unique<render::VertexBuffer>();
			mTexCoords = std::make_unique<render::VertexBuffer>();
			mIndices = std::make_unique<render::VertexBuffer>();

			mVertexArray = std::make_unique<render::VertexArray>();

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

			glBindVertexArray(mVertexArray->getHandle());
			glDrawElements(GL_TRIANGLES, mNumElements, GL_UNSIGNED_INT, nullptr);
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

			using render::eBufferUsage;
			using render::eElementType;

			mNumElements = sizeof(indices) / sizeof(indices[0]);

			mVertices->setData(&vertices, sizeof(vertices), eBufferUsage::STATIC_DRAW);
			mNormals->setData(&normals, sizeof(normals), eBufferUsage::STATIC_DRAW);
			mTexCoords->setData(&texCoords, sizeof(texCoords), eBufferUsage::STATIC_DRAW);
			mIndices->setData(&indices, sizeof(indices), eBufferUsage::STATIC_DRAW);
			
			mVertexArray->bindAttribute(0, *mVertices, eElementType::FLOAT, 3);
			mVertexArray->bindAttribute(1, *mNormals, eElementType::FLOAT, 3);
			mVertexArray->bindAttribute(2, *mTexCoords, eElementType::FLOAT, 2);
			mVertexArray->bindElements(*mIndices);
		}
	}
}