#pragma once

#include "common_vertex_formats.h"

namespace overdrive {
	namespace render {
		/*
			Given a common vertex format, provide a very simple default (fragment) shader

			Default layout locations:
			0 - Position
			1 - Normal
			2 - TexCoord
			3 - Color
		*/
		template <typename TVertexFormat>
		struct DefaultShader {
			static const char* getVertexShader();
			static const char* getFragmentShader();			
		};
	}
}

#include "defaultShaders.inl"