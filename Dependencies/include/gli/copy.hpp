/// @brief Include to copy textures, images or a subset of either textures or an images. These operations will cause memory allocations.
/// @file gli/copy.hpp

#pragma once

#include "image.hpp"
#include "texture1d.hpp"
#include "texture1d_array.hpp"
#include "texture2d.hpp"
#include "texture2d_array.hpp"
#include "texture3d.hpp"
#include "texture_cube.hpp"
#include "texture_cube_array.hpp"

namespace gli
{
	/// Copy an image and create a new image with a new storage allocation.
	image copy(image const & Image);

	/// Copy a texture and create a new texture with a new storage allocation.
	template <typename texType>
	texture copy(texType const & Texture);

	/// Copy a texture and create a new texture with a new storage allocation but a different format.
	/// The format must be a compatible format, a format which block size match the original format. 
	template <typename texType>
	texture copy(texType const & Texture, format Format);

	/// Copy a subset of a texture and create a new texture with a new storage allocation.
	texture copy(
		texture1D const & Texture,
		texture1D::size_type BaseLevel, texture1D::size_type MaxLevel);

	/// Copy a subset of a texture and create a new texture with a new storage allocation.
	texture copy(
		texture1DArray const & Texture,
		texture1DArray::size_type BaseLayer, texture1DArray::size_type MaxLayer,
		texture1DArray::size_type BaseLevel, texture1DArray::size_type MaxLevel);

	/// Copy a subset of a texture and create a new texture with a new storage allocation.
	texture copy(
		texture2D const & Texture,
		texture2D::size_type BaseLevel, texture2D::size_type MaxLevel);

	/// Copy a subset of a texture and create a new texture with a new storage allocation.
	texture copy(
		texture2DArray const & Texture,
		texture2DArray::size_type BaseLayer, texture2DArray::size_type MaxLayer,
		texture2DArray::size_type BaseLevel, texture2DArray::size_type MaxLevel);

	/// Copy a subset of a texture and create a new texture with a new storage allocation.
	texture copy(
		texture3D const & Texture,
		texture3D::size_type BaseLevel, texture3D::size_type MaxLevel);

	/// Copy a subset of a texture and create a new texture with a new storage allocation.
	texture copy(
		textureCube const & Texture,
		textureCube::size_type BaseFace, textureCube::size_type MaxFace,
		textureCube::size_type BaseLevel, textureCube::size_type MaxLevel);

	/// Copy a subset of a texture and create a new texture with a new storage allocation.
	texture copy(
		textureCubeArray const & Texture,
		textureCubeArray::size_type BaseLayer, textureCubeArray::size_type MaxLayer,
		textureCubeArray::size_type BaseFace, textureCubeArray::size_type MaxFace,
		textureCubeArray::size_type BaseLevel, textureCubeArray::size_type MaxLevel);
}//namespace gli

#include "./core/copy.inl"
