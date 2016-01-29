namespace gli{
namespace detail
{
	inline void flip(image ImageDst, image ImageSrc, size_t BlockSize)
	{
		size_t const LineSize = BlockSize * ImageDst.dimensions().x;

		for(int y = 0; y < ImageDst.dimensions().y; ++y)
		{
			size_t OffsetDst = LineSize * y;
			size_t OffsetSrc = ImageSrc.size() - (LineSize * (y + 1));

			memcpy(
				ImageDst.data<glm::byte>() + OffsetDst,
				ImageSrc.data<glm::byte>() + OffsetSrc,
				LineSize);
		}
	}

}//namespace detail

/*
template <>
inline image flip(image const & Image)
{

}
*/

template <>
inline texture2D flip(texture2D const & Texture)
{
	assert(!gli::is_compressed(Texture.format()));

	texture2D Flip(Texture.format(), Texture.dimensions(), Texture.levels());

	texture2D::size_type const BlockSize = block_size(Texture.format());

	for(texture2D::size_type Level = 0; Level < Flip.levels(); ++Level)
		detail::flip(Flip[Level], Texture[Level], BlockSize);

	return Flip;
}

template <>
inline texture2DArray flip(texture2DArray const & Texture)
{
	assert(!gli::is_compressed(Texture.format()));

	texture2DArray Flip(Texture.format(), Texture.dimensions(), Texture.layers(), Texture.levels());

	texture2DArray::size_type const BlockSize = block_size(Texture.format());

	for(texture2DArray::size_type Layer = 0; Layer < Flip.layers(); ++Layer)
	for(texture2DArray::size_type Level = 0; Level < Flip.levels(); ++Level)
		detail::flip(Flip[Layer][Level], Texture[Layer][Level], BlockSize);

	return Flip;
}

}//namespace gli
