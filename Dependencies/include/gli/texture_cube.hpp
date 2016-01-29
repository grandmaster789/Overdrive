/// @brief Include to use cube map textures.
/// @file gli/texture_cube.hpp

#pragma once

#include "texture2d.hpp"

namespace gli
{
	/// Cube map texture
	class textureCube : public texture
	{
	public:
		typedef ivec2 texelcoord_type;

	public:
		/// Create an empty texture cube
		textureCube();

		/// Create a textureCube and allocate a new storage
		explicit textureCube(
			format_type Format,
			texelcoord_type const & Dimensions,
			size_type Levels);

		/// Create a textureCube and allocate a new storage with a complete mipmap chain
		explicit textureCube(
			format_type Format,
			texelcoord_type const & Dimensions);

		/// Create a textureCube view with an existing storage
		explicit textureCube(
			texture const & Texture);

		/// Create a textureCube view with an existing storage
		explicit textureCube(
			texture const & Texture,
			format_type Format,
			size_type BaseLayer, size_type MaxLayer,
			size_type BaseFace, size_type MaxFace,
			size_type BaseLevel, size_type MaxLevel);

		/// Create a textureCube view, reference a subset of an existing textureCube instance
		explicit textureCube(
			textureCube const & Texture,
			size_type BaseFace, size_type MaxFace,
			size_type BaseLevel, size_type MaxLevel);

		/// Create a view of the texture identified by Face in the texture cube
		texture2D operator[](size_type Face) const;

		/// Return the dimensions of a texture instance: width and height where both should be equal.
		texelcoord_type dimensions(size_type Level = 0) const;

		/// Fetch a texel from a texture. The texture format must be uncompressed.
		template <typename genType>
		genType load(texelcoord_type const & TexelCoord, size_type Face, size_type Level) const;

		/// Write a texel to a texture. The texture format must be uncompressed.
		template <typename genType>
		void store(texelcoord_type const & TexelCoord, size_type Face, size_type Level, genType const & Texel);

		/// Clear the entire texture storage with zeros
		void clear();

		/// Clear the entire texture storage with Texel which type must match the texture storage format block size
		/// If the type of genType doesn't match the type of the texture format, no conversion is performed and the data will be reinterpreted as if is was of the texture format. 
		template <typename genType>
		void clear(genType const & Texel);

		/// Clear a specific image of a texture.
		template <typename genType>
		void clear(size_type Face, size_type Level, genType const & Texel);

	private:
		struct cache
		{
			std::uint8_t* Data;
			texelcoord_type Dim;
#			ifndef NDEBUG
				size_type Size;
#			endif
		};

		void build_cache();
		size_type index_cache(size_type Face, size_type Level) const;

		std::vector<cache> Caches;
	};
}//namespace gli

#include "./core/texture_cube.inl"
