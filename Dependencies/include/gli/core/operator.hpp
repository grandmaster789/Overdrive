#pragma once

#include "texture2d.hpp"

namespace gli{
namespace detail
{

}//namespace detail

	/// Add two textures, each texel
	texture2D operator+(texture2D const & TextureA, texture2D const & TextureB);

	/// Substract two textures, each texel
	texture2D operator-(texture2D const & TextureA, texture2D const & TextureB);

}//namespace gli

#include "operator.inl"
