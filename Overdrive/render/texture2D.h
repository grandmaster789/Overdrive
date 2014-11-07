#ifndef OVERDRIVE_RENDER_TEXTURE2D_H
#define OVERDRIVE_RENDER_TEXTURE2D_H

#include "texture.h"

namespace overdrive {
	namespace render {
		class Texture2D :
			public Texture
		{
		public:
			Texture2D(
				const void* data,
				unsigned int imageWidth,
				unsigned int imageHeight,
				eFormat format = eFormat::RGBA,
				eDataType dataType = eDataType::UNSIGNED_BYTE,
				eInternalFormat internalFormat = eInternalFormat::RGBA
			);

			void setWrapping(eWrapping s, eWrapping t); // defaults to CLAMP_TO_EDGE
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
