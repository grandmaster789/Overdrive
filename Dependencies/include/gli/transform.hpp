/// @brief Include to perform arithmetic per texel between two textures.
/// @file gli/transform.hpp

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
	template <typename vec_type>
	struct transform_func
	{
		typedef vec_type(*type)(vec_type const & A, vec_type const & B);
	};

	/// Compute per-texel operations using a user defined function.
	///
	/// @param Out Output texture.
	/// @param In0 First input texture.
	/// @param In1 Second input texture.
	/// @param TexelFunc Pointer to a binary function.
	template <typename vec_type>
	void transform(texture1D & Out, texture1D const & In0, texture1D const & In1, typename transform_func<vec_type>::type TexelFunc);

	/// Compute per-texel operations using a user defined function.
	///
	/// @param Out Output texture.
	/// @param In0 First input texture.
	/// @param In1 Second input texture.
	/// @param TexelFunc Pointer to a binary function.
	template <typename vec_type>
	void transform(texture1DArray & Out, texture1DArray const & In0, texture1DArray const & In1, typename transform_func<vec_type>::type TexelFunc);

	/// Compute per-texel operations using a user defined function.
	///
	/// @param Out Output texture.
	/// @param In0 First input texture.
	/// @param In1 Second input texture.
	/// @param TexelFunc Pointer to a binary function.
	template <typename vec_type>
	void transform(texture2D & Out, texture2D const & In0, texture2D const & In1, typename transform_func<vec_type>::type TexelFunc);

	/// Compute per-texel operations using a user defined function.
	///
	/// @param Out Output texture.
	/// @param In0 First input texture.
	/// @param In1 Second input texture.
	/// @param TexelFunc Pointer to a binary function.
	template <typename vec_type>
	void transform(texture2DArray & Out, texture2DArray const & In0, texture2DArray const & In1, typename transform_func<vec_type>::type TexelFunc);

	/// Compute per-texel operations using a user defined function.
	///
	/// @param Out Output texture.
	/// @param In0 First input texture.
	/// @param In1 Second input texture.
	/// @param TexelFunc Pointer to a binary function.
	template <typename vec_type>
	void transform(texture3D & Out, texture3D const & In0, texture3D const & In1, typename transform_func<vec_type>::type TexelFunc);

	/// Compute per-texel operations using a user defined function.
	///
	/// @param Out Output texture.
	/// @param In0 First input texture.
	/// @param In1 Second input texture.
	/// @param TexelFunc Pointer to a binary function.
	template <typename vec_type>
	void transform(textureCube & Out, textureCube const & In0, textureCube const & In1, typename transform_func<vec_type>::type TexelFunc);

	/// Compute per-texel operations using a user defined function.
	///
	/// @param Out Output texture.
	/// @param In0 First input texture.
	/// @param In1 Second input texture.
	/// @param TexelFunc Pointer to a binary function.
	template <typename vec_type>
	void transform(textureCubeArray & Out, textureCubeArray const & In0, textureCubeArray const & In1, typename transform_func<vec_type>::type TexelFunc);
	
}//namespace gli

#include "./core/transform.inl"
