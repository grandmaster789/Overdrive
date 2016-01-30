#include "clear.hpp"
#include <glm/vector_relational.hpp>

namespace gli
{
	template <typename T, precision P>
	inline sampler2D<T, P>::sampler2D(texture_type const & Texture, wrap Wrap, filter Mip, filter Min, texel_type const & BorderColor)
		: sampler(Wrap, Texture.levels() > 1 ? Mip : FILTER_NEAREST, Min)
		, Texture(Texture)
		, Convert(detail::convert<texture_type, T, P>::call(this->Texture.format()))
		, BorderColor(BorderColor)
		, Filter(detail::get_filter<filter_type, detail::DIMENSION_2D, texture_type, interpolate_type, samplecoord_type, fetch_type, texel_type, T>(Mip, Min, is_border(Wrap)))
	{
		GLI_ASSERT(!Texture.empty());
		GLI_ASSERT(!is_compressed(Texture.format()));
		GLI_ASSERT((!std::numeric_limits<T>::is_iec559 && Mip == FILTER_NEAREST && Min == FILTER_NEAREST) || std::numeric_limits<T>::is_iec559);
	}

	template <typename T, precision P>
	inline typename sampler2D<T, P>::texture_type const & sampler2D<T, P>::operator()() const
	{
		return this->Texture;
	}

	template <typename T, precision P>
	inline typename sampler2D<T, P>::texel_type sampler2D<T, P>::texel_fetch(texelcoord_type const & TexelCoord, size_type const & Level) const
	{
		GLI_ASSERT(!this->Texture.empty());
		GLI_ASSERT(this->Convert.Fetch);

		return this->Convert.Fetch(this->Texture, TexelCoord, 0, 0, Level);
	}

	template <typename T, precision P>
	inline void sampler2D<T, P>::texel_write(texelcoord_type const & TexelCoord, size_type const & Level, texel_type const & Texel)
	{
		GLI_ASSERT(!this->Texture.empty());
		GLI_ASSERT(this->Convert.Write);

		this->Convert.Write(this->Texture, TexelCoord, 0, 0, Level, Texel);
	}

	template <typename T, precision P>
	inline void sampler2D<T, P>::clear(texel_type const & Color)
	{
		GLI_ASSERT(!this->Texture.empty());
		GLI_ASSERT(this->Convert.Write);

		detail::clear<texture_type, T, P>::call(this->Texture, this->Convert.Write, Color);
	}

	template <typename T, precision P>
	inline typename sampler2D<T, P>::texel_type sampler2D<T, P>::texture_lod(samplecoord_type const & SampleCoord, level_type Level) const
	{
		GLI_ASSERT(!this->Texture.empty());
		GLI_ASSERT(std::numeric_limits<T>::is_iec559);
		GLI_ASSERT(this->Filter && this->Convert.Fetch);

		samplecoord_type const SampleCoordWrap(this->Wrap(SampleCoord.x), this->Wrap(SampleCoord.y));
		return this->Filter(this->Texture, this->Convert.Fetch, SampleCoordWrap, size_type(0), size_type(0), Level, this->BorderColor);
	}

	template <typename T, precision P>
	inline void sampler2D<T, P>::generate_mipmaps(filter Minification)
	{
		this->generate_mipmaps(this->Texture.base_level(), this->Texture.max_level(), Minification);
	}

	template <typename T, precision P>
	inline void sampler2D<T, P>::generate_mipmaps(size_type BaseLevel, size_type MaxLevel, filter Minification)
	{
		GLI_ASSERT(!this->Texture.empty());
		GLI_ASSERT(!is_compressed(this->Texture.format()));
		GLI_ASSERT(this->Texture.base_level() <= BaseLevel && BaseLevel <= MaxLevel && MaxLevel <= this->Texture.max_level());
		GLI_ASSERT(this->Convert.Fetch && this->Convert.Write);
		GLI_ASSERT(Minification >= FILTER_FIRST && Minification <= FILTER_LAST);

		detail::generate_mipmaps_2d<texture_type, T, fetch_type, write_type, samplecoord_type, texel_type>(
			this->Texture, this->Convert.Fetch, this->Convert.Write, 0, 0, 0, 0, BaseLevel, MaxLevel, Minification);
	}
}//namespace gli
