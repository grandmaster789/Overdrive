/// @brief Include to copy textures, images or a subset of either textures or an images. These operations will cause memory allocations.
/// @file gli/convert.hpp

#pragma once

#include "texture1d.hpp"
#include "texture1d_array.hpp"
#include "texture2d.hpp"
#include "texture2d_array.hpp"
#include "texture3d.hpp"
#include "texture_cube.hpp"
#include "texture_cube_array.hpp"

namespace gli
{
	/// Convert texture data to a new format
	///
	/// @param Texture Source texture, the format must be uncompressed.
	/// @param Format Destination Texture format, it must be uncompressed.
	texture1D convert(texture1D const& Texture, format Format);

	/// Convert texture data to a new format
	///
	/// @param Texture Source texture, the format must be uncompressed.
	/// @param Format Destination Texture format, it must be uncompressed.
	texture1DArray convert(texture1DArray const& Texture, format Format);

	/// Convert texture data to a new format
	///
	/// @param Texture Source texture, the format must be uncompressed.
	/// @param Format Destination Texture format, it must be uncompressed.
	texture2D convert(texture2D const& Texture, format Format);

	/// Convert texture data to a new format
	///
	/// @param Texture Source texture, the format must be uncompressed.
	/// @param Format Destination Texture format, it must be uncompressed.
	texture2DArray convert(texture2DArray const& Texture, format Format);

	/// Convert texture data to a new format
	///
	/// @param Texture Source texture, the format must be uncompressed.
	/// @param Format Destination Texture format, it must be uncompressed.
	texture3D convert(texture3D const& Texture, format Format);

	/// Convert texture data to a new format
	///
	/// @param Texture Source texture, the format must be uncompressed.
	/// @param Format Destination Texture format, it must be uncompressed.
	textureCube convert(textureCube const& Texture, format Format);

	/// Convert texture data to a new format
	///
	/// @param Texture Source texture, the format must be uncompressed.
	/// @param Format Destination Texture format, it must be uncompressed.
	textureCubeArray convert(textureCubeArray const& Texture, format Format);
}//namespace gli

#include "./core/convert.inl"
