#ifndef OVERDRIVE_RENDER_TEXTURE1D_H
#define OVERDRIVE_RENDER_TEXTURE1D_H

#include "texture.h"

namespace overdrive {
	namespace render {
		class Texture1D :
			public Texture
		{
		public:
			explicit Texture1D();
			Texture1D(
				const void* data,
				unsigned int imageSize,
				eFormat format = eFormat::RGBA,
				eDataType dataType = eDataType::UNSIGNED_BYTE,
				eInternalFormat internalFormat = eInternalFormat::RGBA
			);

			void bind();
			void setWrapping(eWrapping s);
			void setFilters(eFilter mini, eFilter mag);
			void setBorder(const util::Color& c);

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
