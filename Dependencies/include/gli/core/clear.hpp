#pragma once

#include "convert_func.hpp"

namespace gli{
namespace detail
{
	template <typename textureType, typename T, precision P>
	struct clear
	{
		static void call(textureType & Texture, typename convert<textureType, T, P>::writeFunc Write, tvec4<T, P> const & Color)
		{
			assert(Write);

			texture const ConvertTexel(Texture.target(), Texture.format(), texture::texelcoord_type(1), 1, 1, 1);
			textureType Texel(ConvertTexel);
			Write(Texel, typename textureType::texelcoord_type(0), 0, 0, 0, Color);

			size_t const BlockSize(block_size(Texture.format()));
			for(size_t BlockIndex = 0, BlockCount = Texture.size() / BlockSize; BlockIndex < BlockCount; ++BlockIndex)
				memcpy(static_cast<std::uint8_t*>(Texture.data()) + BlockSize * BlockIndex, Texel.data(), BlockSize);
		}
	};
}//namespace detail
}//namespace gli
