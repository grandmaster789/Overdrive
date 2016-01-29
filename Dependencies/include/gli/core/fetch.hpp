#pragma once

#include "../gli.hpp"

namespace gli
{
	/// Fetch a texel from a texture
	/// The texture format must be uncompressed
	template <typename genType>
	genType texel_fetch(
		texture2D const & Texture,
		texture2D::dim_type const & TexelCoord,
		texture2D::size_type const & Level);

	/// Write a texel to a texture
	/// The texture format must be uncompressed
	template <typename genType>
	void texel_write(
		texture2D & Texture,
		texture2D::dim_type const & TexelCoord,
		texture2D::size_type const & Level,
		genType const & Color);

	/// Sample a pixel from a texture
	/// The texture format must be uncompressed
	template <typename genType>
	genType texture_lod(
		texture2D const & Texture,
		texture2D::samplecoord_type const & SampleCoord,
		texture2D::size_type const & Level);

}//namespace gli

#include "fetch.inl"
