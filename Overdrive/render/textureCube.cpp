#include "stdafx.h"
#include "textureCube.h"
#include "texture2D.h"
#include "../util/deleters.h"

namespace overdrive {
	namespace render {
		TextureCube::TextureCube():
			mHandle(0),
			mFormat(gli::FORMAT_UNDEFINED)
		{
		}

		TextureCube::TextureCube(eTextureFormat fmt, int width, int height):
			mHandle(0),
			mFormat(fmt)
		{
			glGenTextures(1, &mHandle);
			if (mHandle == 0)
				throw std::runtime_error("Unable to allocate a new texture handle");

			glBindTexture(GL_TEXTURE_CUBE_MAP, mHandle);

			auto format = detail::gFormatConverter.translate(fmt);

			// submit texture data (all empty faces)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format.Internal, width, height, 0, format.External, format.Type, nullptr);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format.Internal, width, height, 0, format.External, format.Type, nullptr);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format.Internal, width, height, 0, format.External, format.Type, nullptr);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format.Internal, width, height, 0, format.External, format.Type, nullptr);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format.Internal, width, height, 0, format.External, format.Type, nullptr);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format.Internal, width, height, 0, format.External, format.Type, nullptr);

			// set various flags
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

		TextureCube::TextureCube(
			eTextureFormat fmt,
			int width,
			int height,
			const unsigned char* rawPositiveX,
			const unsigned char* rawPositiveY,
			const unsigned char* rawPositiveZ,
			const unsigned char* rawNegativeX,
			const unsigned char* rawNegativeY,
			const unsigned char* rawNegativeZ
		):
			mHandle(0),
			mFormat(fmt)
		{
			glGenTextures(1, &mHandle);
			if (mHandle == 0)
				throw std::runtime_error("Unable to allocate a new texture handle");

			glBindTexture(GL_TEXTURE_CUBE_MAP, mHandle);

			auto format = detail::gFormatConverter.translate(fmt);

			// submit texture data
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format.Internal, width, height, 0, format.External, format.Type, rawPositiveX);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format.Internal, width, height, 0, format.External, format.Type, rawPositiveY);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format.Internal, width, height, 0, format.External, format.Type, rawPositiveZ);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format.Internal, width, height, 0, format.External, format.Type, rawNegativeX);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format.Internal, width, height, 0, format.External, format.Type, rawNegativeY);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format.Internal, width, height, 0, format.External, format.Type, rawNegativeZ);

			// set various flags
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

		TextureCube::TextureCube(const gli::texture& tex) {
			glGenTextures(1, &mHandle);
			if (mHandle == 0)
				throw std::runtime_error("Unable to allocate a new texture handle");

			glBindTexture(GL_TEXTURE_CUBE_MAP, mHandle);

			auto format = detail::gFormatConverter.translate(tex.format());
			auto dims = tex.dimensions();

			switch (tex.target()) {
			case gli::TARGET_2D: {
					glTexStorage2D(
						GL_TEXTURE_CUBE_MAP, 
						static_cast<GLsizei>(tex.levels()), 
						format.Internal, 
						dims.x,
						dims.y
					);

					// the order used in gli is:
					// POSITIVE_X
					// NEGATIVE_X
					// POSITIVE_Y
					// NEGATIVE_Y
					// POSITIVE_Z
					// NEGATIVE_Z

					for (size_t layer = 0; layer < tex.layers(); ++layer) {
						for (size_t face = 0; face < 6; ++face) {
							GLenum currentFace = static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face);

							for (size_t level = 0; level < tex.levels(); ++level) {
								if (gli::is_compressed(tex.format()))
									glCompressedTexSubImage2D(
										currentFace,
										static_cast<GLint>(level),
										0, // x offset
										0, // y offset
										dims.x,
										dims.y,
										format.External,
										static_cast<GLsizei>(tex.size(level)),
										tex.data(layer, 0, level)
									);
								else
									glTexSubImage2D(
										currentFace,
										static_cast<GLint>(level),
										0,
										0,
										dims.x,
										dims.y,
										format.External,
										format.Type,
										tex.data(layer, 0, level)
									);
							}
						}
					}
				}
				break;

			case gli::TARGET_CUBE: {
					glTexStorage2D(
						GL_TEXTURE_CUBE_MAP, 
						static_cast<GLsizei>(tex.levels()), 
						format.Internal, 
						dims.x, 
						dims.y
					);

					for (size_t layer = 0; layer < tex.layers(); ++layer) {
						for (size_t face = 0; face < tex.faces(); ++face) {
							GLenum currentFace = static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face);

							for (size_t level = 0; level < tex.levels(); ++level) {
								if (gli::is_compressed(tex.format()))
									glCompressedTexSubImage2D(
										currentFace,
										static_cast<GLint>(level),
										0,
										0,
										dims.x,
										dims.y,
										format.External,
										static_cast<GLsizei>(tex.size(level)),
										tex.data(layer, face, level)
									);
								else
									glTexSubImage2D(
										currentFace,
										static_cast<GLint>(level),
										0,
										0,
										dims.x,
										dims.y,
										format.External,
										format.Type,
										tex.data(layer, face, level)
									);
							}
						}
					}
				}
				break;

			default:
				throw std::runtime_error("Unsupported gli texture target");
			}
					
			// set various flags
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

		TextureCube::~TextureCube() {
			if (mHandle)
				glDeleteTextures(1, &mHandle);
		}

		TextureCube::TextureCube(TextureCube&& t):
			mHandle(t.mHandle),
			mFormat(t.mFormat)
		{
			t.mHandle = 0;
		}

		TextureCube& TextureCube::operator = (TextureCube&& t) {
			if (mHandle)
				glDeleteTextures(1, &mHandle);

			mHandle = t.mHandle;
			mFormat = t.mFormat;

			t.mHandle = 0;

			return *this;
		}

		GLuint TextureCube::getHandle() const {
			return mHandle;
		}

		eTextureFormat TextureCube::getFormat() const {
			return mFormat;
		}
				
		void TextureCube::bind() {
			glBindTexture(GL_TEXTURE_CUBE_MAP, mHandle);
		}

		void TextureCube::unbind() {
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}


		void TextureCube::bind(int textureUnit) {
			glBindTextureUnit(textureUnit, mHandle);
		}

		void TextureCube::unbind(int textureUnit) {
			glBindTextureUnit(textureUnit, 0);
		}

		namespace {
			std::vector<char> readFile(const std::string& filepath) {
				std::unique_ptr<FILE, util::FCloseHelper> file(
					std::fopen(filepath.c_str(), "rb")
				);

				// figure out how big the file is
				auto begin_of_file = std::ftell(file.get());
				std::fseek(file.get(), 0, SEEK_END);
				auto end_of_file = std::ftell(file.get());
				std::fseek(file.get(), 0, SEEK_SET);

				std::vector<char> rawData(static_cast<size_t>(end_of_file - begin_of_file));

				std::fread(&rawData[0], 1, rawData.size(), file.get());

				return rawData;
			}

			struct ImageInfo {
				int mImageWidth;
				int mImageHeight;
				eTextureFormat mFormat;
				std::unique_ptr<stbi_uc[], util::FreeHelper> mUncompressedData;
			};

			ImageInfo loadRaw(const std::vector<char>& rawData) {
				ImageInfo result;

				int numChannels = 0;

				result.mUncompressedData.reset(
					stbi_load_from_memory(
						reinterpret_cast<const stbi_uc*>(rawData.data()),
						static_cast<int>(rawData.size()),
						&result.mImageWidth,
						&result.mImageHeight,
						&numChannels, 
						0
					)
				);

				switch (numChannels) {
				case 1: result.mFormat = gli::FORMAT_R8_UNORM_PACK8; break;
				case 2: result.mFormat = gli::FORMAT_RG8_UNORM_PACK8; break;
				case 3: result.mFormat = gli::FORMAT_RGB8_UNORM_PACK8; break;
				case 4: result.mFormat = gli::FORMAT_RGBA8_UNORM_PACK8; break;

				default:
					return ImageInfo();
				}

				return result;
			}
		}

		TextureCube loadTextureCube(const std::string& filepath) {
			auto rawData = readFile(filepath);

			// try for gli first
			{
				auto tex = gli::load(&rawData[0], rawData.size());

				if (!tex.empty())
					return TextureCube(tex);
			}

			// load using stbi
			auto img = loadRaw(rawData);

			if (img.mUncompressedData) {
				return TextureCube(
					img.mFormat, 
					img.mImageWidth, 
					img.mImageHeight,
					img.mUncompressedData.get(),
					img.mUncompressedData.get(),
					img.mUncompressedData.get(),
					img.mUncompressedData.get(),
					img.mUncompressedData.get(),
					img.mUncompressedData.get()
				);
			}

			throw std::runtime_error("Failed to load texture cube");
		}

		TextureCube loadTextureCube(
			const std::string& positiveX,
			const std::string& negativeX,
			const std::string& positiveY,
			const std::string& negativeY,
			const std::string& positiveZ,
			const std::string& negativeZ
		) {
			// assume stbi... [TODO] add gli basis
			std::vector<char> file_data[] = {
				readFile(positiveX),
				readFile(negativeX),
				readFile(positiveY),
				readFile(negativeY),
				readFile(positiveZ),
				readFile(negativeZ)
			};

			ImageInfo uncompressed[] = {	
				loadRaw(file_data[0]),
				loadRaw(file_data[1]),
				loadRaw(file_data[2]),
				loadRaw(file_data[3]),
				loadRaw(file_data[4]),
				loadRaw(file_data[5])
			};	

			// [TODO] add some checking (textures must be square, and perhaps power of two?)
			// [TODO] verify that all of the I/O actually succeeded
			return TextureCube(
				uncompressed[0].mFormat,
				uncompressed[0].mImageWidth,
				uncompressed[0].mImageHeight,

				uncompressed[0].mUncompressedData.get(),
				uncompressed[1].mUncompressedData.get(),
				uncompressed[2].mUncompressedData.get(),
				uncompressed[3].mUncompressedData.get(),
				uncompressed[4].mUncompressedData.get(),
				uncompressed[5].mUncompressedData.get()
			);
		}
	}
}
