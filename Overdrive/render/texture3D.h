#ifndef OVERDRIVE_RENDER_TEXTURE3D_H
#define OVERDRIVE_RENDER_TEXTURE3D_H

#include "render/texture.h"

namespace overdrive {
	namespace render {
		class Texture3D :
			public Texture
		{
		public:
			explicit Texture3D();

			Texture3D(
				const void* data,
				unsigned int imageWidth,
				unsigned int imageHeight,
				unsigned int imageDepth,
				eFormat format = eFormat::RGBA,
				eDataType dataType = eDataType::UNSIGNED_BYTE,
				eInternalFormat internalFormat = eInternalFormat::RGBA
			);

			void bind();
			void setWrapping(eWrapping s, eWrapping t, eWrapping r); // defaults to CLAMP_TO_EDGE
			void setFilters(eFilter mini, eFilter mag);	// defaults to LINEAR_MIPMAP_LINEAR + LINEAR
			void setBorder(const util::Color& c);		// defaults to black

			void generateMipmaps();

		private:
			// helper class that stores the currently bound texture handle and restores it upon destruction
			class TexGuard {
			public:
				TexGuard();
				TexGuard(const TexGuard&) = delete;
				~TexGuard();

			private:
				GLint mCurrentTextureBinding;
			};
		};
	}
}

#endif
