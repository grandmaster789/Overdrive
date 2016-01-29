/// @brief Include to use 2d array textures.
/// @file gli/texture2d_array.hpp

#pragma once

#include "texture2d.hpp"

namespace gli
{
	/// 2d array texture
	class texture2DArray : public texture
	{
	public:
		typedef ivec2 texelcoord_type;

	public:
		/// Create an empty texture 2D array
		texture2DArray();

		/// Create a texture2DArray and allocate a new storage
		explicit texture2DArray(
			format_type Format,
			texelcoord_type const & Dimensions,
			size_type Layers,
			size_type Levels);

		/// Create a texture2DArray and allocate a new storage with a complete mipmap chain
		explicit texture2DArray(
			format_type Format,
			texelcoord_type const & Dimensions,
			size_type Layers);

		/// Create a texture2DArray view with an existing storage
		explicit texture2DArray(
			texture const & Texture);

		/// Create a texture2DArray view with an existing storage
		explicit texture2DArray(
			texture const & Texture,
			format_type Format,
			size_type BaseLayer, size_type MaxLayer,
			size_type BaseFace, size_type MaxFace,
			size_type BaseLevel, size_type MaxLevel);

		/// Create a texture view, reference a subset of an exiting texture2DArray instance
		explicit texture2DArray(
			texture2DArray const & Texture,
			size_type BaseLayer, size_type MaxLayer,
			size_type BaseLevel, size_type MaxLevel);

		/// Create a view of the texture identified by Layer in the texture array
		texture2D operator[](size_type Layer) const;

		/// Return the dimensions of a texture instance: width and height 
		texelcoord_type dimensions(size_type Level = 0) const;

		/// Fetch a texel from a texture. The texture format must be uncompressed.
		template <typename genType>
		genType load(texelcoord_type const & TexelCoord, size_type Layer, size_type Level) const;

		/// Write a texel to a texture. The texture format must be uncompressed.
		template <typename genType>
		void store(texelcoord_type const & TexelCoord, size_type Layer, size_type Level, genType const & Texel);

		/// Clear the entire texture storage with zeros
		void clear();

		/// Clear the entire texture storage with Texel which type must match the texture storage format block size
		/// If the type of genType doesn't match the type of the texture format, no conversion is performed and the data will be reinterpreted as if is was of the texture format. 
		template <typename genType>
		void clear(genType const & Texel);

		/// Clear a specific image of a texture.
		template <typename genType>
		void clear(size_type Layer, size_type Level, genType const & Texel);

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
		size_type index_cache(size_type Layer, size_type Level) const;

		std::vector<cache> Caches;
	};
}//namespace gli

#include "./core/texture2d_array.inl"
