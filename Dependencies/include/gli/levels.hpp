/// @brief Include to compute the number of mipmaps levels necessary to create a mipmap complete texture.
/// @file gli/levels.hpp

#pragma once

#include "type.hpp"

namespace gli
{
	/// Compute the number of mipmaps levels necessary to create a mipmap complete texture
	/// 
	/// @param Dimensions Dimensions of the texture base level mipmap
	/// @tparam dimType Vector type used to express the dimentions of a texture of any kind.
	/// @code
	/// #include <gli/gli.hpp>
	/// #include <gli/levels.hpp>
	/// ...
	/// gli::texture2D::texelcoord_type Dimensions(32, 10);
	/// gli::texture2D Texture(gli::levels(Dimensions));
	/// @endcode
	template <typename T, precision P, template <typename, precision> class vecType>
	T levels(vecType<T, P> const & Dimensions);

	/// Compute the number of mipmaps levels necessary to create a mipmap complete texture
	/// 
	/// @param Dimension Dimension of the texture base level mipmap
	/// @tparam dimType Vector type used to express the dimentions of a texture of any kind.
	/// @code
	/// #include <gli/gli.hpp>
	/// #include <gli/levels.hpp>
	/// ...
	/// gli::texture2D Texture(32);
	/// @endcode
	size_t levels(size_t Dimension);

	/// Compute the number of mipmaps levels necessary to create a mipmap complete texture
	/// 
	/// @param Dimension Dimension of the texture base level mipmap
	/// @tparam dimType Vector type used to express the dimentions of a texture of any kind.
	/// @code
	/// #include <gli/gli.hpp>
	/// #include <gli/levels.hpp>
	/// ...
	/// gli::texture2D Texture(32);
	/// @endcode
	int levels(int Dimension);
}//namespace gli

#include "./core/levels.inl"
