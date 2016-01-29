/// @brief Include create views of textures, either to isolate a subset or to reinterpret data without memory copy.
/// @file gli/view.hpp

#pragma once

#include "image.hpp"
#include "texture.hpp"
#include "texture1d.hpp"
#include "texture1d_array.hpp"
#include "texture2d.hpp"
#include "texture2d_array.hpp"
#include "texture3d.hpp"
#include "texture_cube.hpp"
#include "texture_cube_array.hpp"

namespace gli
{
	/// Create an image view of an existing image, sharing the same memory storage.
	image view(image const & Image);

	/// Create a texture view of an existing texture, sharing the same memory storage.
	texture view(texture const & Texture);

	/// Create a texture view of an existing texture, sharing the same memory storage but giving access only to a subset of layers, levels and faces.
	texture view(
		texture const & Texture,
		texture::size_type BaseLayer, texture::size_type MaxLayer,
		texture::size_type BaseFace, texture::size_type MaxFace,
		texture::size_type BaseLevel, texture::size_type MaxLevel);

	/// Create a texture view of an existing texture, sharing the same memory storage.
	template <typename texType>
	texture view(texType const & Texture);

	/// Create a texture view of an existing texture, sharing the same memory storage but a different format.
	/// The format must be a compatible format, a format which block size match the original format. 
	template <typename texType>
	texture view(texType const & Texture, format Format);

	/// Create a texture view of an existing texture, sharing the same memory storage but giving access only to a subset of levels.
	texture view(
		texture1D const & Texture,
		texture1D::size_type BaseLevel, texture1D::size_type MaxLevel);

	/// Create a texture view of an existing texture, sharing the same memory storage but giving access only to a subset of levels and layers.
	texture view(
		texture1DArray const & Texture,
		texture1DArray::size_type BaseLayer, texture1DArray::size_type MaxLayer,
		texture1DArray::size_type BaseLevel, texture1DArray::size_type MaxLevel);

	/// Create a texture view of an existing texture, sharing the same memory storage but giving access only to a subset of levels.
	texture view(
		texture2D const & Texture,
		texture2D::size_type BaseLevel, texture2D::size_type MaxLevel);

	/// Create a texture view of an existing texture, sharing the same memory storage but giving access only to a subset of levels and layers.
	texture view(
		texture2DArray const & Texture,
		texture2DArray::size_type BaseLayer, texture2DArray::size_type MaxLayer,
		texture2DArray::size_type BaseLevel, texture2DArray::size_type MaxLevel);

	/// Create a texture view of an existing texture, sharing the same memory storage but giving access only to a subset of levels.
	texture view(
		texture3D const & Texture,
		texture3D::size_type BaseLevel, texture3D::size_type MaxLevel);

	/// Create a texture view of an existing texture, sharing the same memory storage but giving access only to a subset of levels and faces.
	texture view(
		textureCube const & Texture,
		textureCube::size_type BaseFace, textureCube::size_type MaxFace,
		textureCube::size_type BaseLevel, textureCube::size_type MaxLevel);

	/// Create a texture view of an existing texture, sharing the same memory storage but giving access only to a subset of layers, levels and faces.
	texture view(
		textureCubeArray const & Texture,
		textureCubeArray::size_type BaseLayer, textureCubeArray::size_type MaxLayer,
		textureCubeArray::size_type BaseFace, textureCubeArray::size_type MaxFace,
		textureCubeArray::size_type BaseLevel, textureCubeArray::size_type MaxLevel);
}//namespace gli

#include "./core/view.inl"
