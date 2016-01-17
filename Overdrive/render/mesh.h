#pragma once

#include "shaderprogram.h"
#include "indexbuffer.h"
#include "vertexbuffer.h"

namespace overdrive {
	namespace render {
		class Mesh {
		public:
			using VertexFormat	= attributes::PositionNormalTexCoord;
			using VertexBuffer	= VertexBuffer<VertexFormat>;
			using IndexFormat	= GLuint;
			using IndexBuffer	= IndexBuffer<IndexFormat>;

			Mesh();
			
			void setVertices(const VertexBuffer& vtx);
			void setIndices(const IndexBuffer& idx);
			void setMaterial(ShaderProgram* material);

			// [NOTE] need some kind of interface to store and set material parameters for this specific instance
			
			void draw();

		private:
			ShaderProgram* mMaterial; // ~ should this perchance be an actually shared object?

			VertexBuffer mVertices;
			IndexBuffer mIndices;
		};
	}
}
