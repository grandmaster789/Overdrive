#include "../levels.hpp"

namespace gli
{
	inline texture2D::texture2D()
	{}

	inline texture2D::texture2D(format_type Format, texelcoord_type const & Dimensions)
		: texture(TARGET_2D, Format, texture::texelcoord_type(Dimensions, 1), 1, 1, gli::levels(Dimensions))
	{
		this->build_cache();
	}

	inline texture2D::texture2D(format_type Format, texelcoord_type const & Dimensions, size_type Levels)
		: texture(TARGET_2D, Format, texture::texelcoord_type(Dimensions, 1), 1, 1, Levels)
	{
		this->build_cache();
	}

	inline texture2D::texture2D(texture const & Texture)
		: texture(Texture, TARGET_2D, Texture.format())
	{
		this->build_cache();
	}

	inline texture2D::texture2D
	(
		texture const & Texture,
		format_type Format,
		size_type BaseLayer, size_type MaxLayer,
		size_type BaseFace, size_type MaxFace,
		size_type BaseLevel, size_type MaxLevel
	)
		: texture(
			Texture, TARGET_2D, Format,
			BaseLayer, MaxLayer,
			BaseFace, MaxFace,
			BaseLevel, MaxLevel)
	{
		this->build_cache();
	}

	inline texture2D::texture2D
	(
		texture2D const & Texture,
		size_type BaseLevel, size_type MaxLevel
	)
		: texture(
			Texture, TARGET_2D, Texture.format(),
			Texture.base_layer(), Texture.max_layer(),
			Texture.base_face(), Texture.max_face(),
			Texture.base_level() + BaseLevel, Texture.base_level() + MaxLevel)
	{
		this->build_cache();
	}

	inline image texture2D::operator[](size_type Level) const
	{
		GLI_ASSERT(Level < this->levels());

		return image(
			this->Storage,
			this->format(),
			this->base_layer(),
			this->base_face(),
			this->base_level() + Level);
	}

	inline texture2D::texelcoord_type texture2D::dimensions(size_type Level) const
	{
		GLI_ASSERT(!this->empty());

		return this->Caches[this->index_cache(Level)].Dim;
	}

	template <typename genType>
	inline genType texture2D::load(texelcoord_type const & TexelCoord, size_type Level) const
	{
		GLI_ASSERT(!this->empty());
		GLI_ASSERT(!is_compressed(this->format()));
		GLI_ASSERT(block_size(this->format()) == sizeof(genType));

		cache const & Cache = this->Caches[this->index_cache(Level)];

		std::size_t const Index = linear_index(TexelCoord, Cache.Dim);
		GLI_ASSERT(Index < Cache.Size / sizeof(genType));

		return reinterpret_cast<genType const * const>(Cache.Data)[Index];
	}

	template <typename genType>
	inline void texture2D::store(texelcoord_type const & TexelCoord, size_type Level, genType const & Texel)
	{
		GLI_ASSERT(!this->empty());
		GLI_ASSERT(!is_compressed(this->format()));
		GLI_ASSERT(block_size(this->format()) == sizeof(genType));

		cache const & Cache = this->Caches[this->index_cache(Level)];
		GLI_ASSERT(glm::all(glm::lessThan(TexelCoord, Cache.Dim)));

		size_type const Index = linear_index(TexelCoord, Cache.Dim);
		GLI_ASSERT(Index < Cache.Size / sizeof(genType));

		reinterpret_cast<genType*>(Cache.Data)[Index] = Texel;
	}

	inline void texture2D::clear()
	{
		this->texture::clear();
	}

	template <typename genType>
	inline void texture2D::clear(genType const & Texel)
	{
		this->texture::clear<genType>(Texel);
	}

	template <typename genType>
	inline void texture2D::clear(size_type Level, genType const & Texel)
	{
		this->texture::clear<genType>(0, 0, Level, Texel);
	}

	inline texture2D::size_type texture2D::index_cache(size_type Level) const
	{
		return Level;
	}

	inline void texture2D::build_cache()
	{
		this->Caches.resize(this->levels());

		for(size_type Level = 0, Levels = this->levels(); Level < Levels; ++Level)
		{
			cache& Cache = this->Caches[this->index_cache(Level)];
			Cache.Data = this->data<std::uint8_t>(0, 0, Level);
			Cache.Dim = glm::max(texelcoord_type(this->texture::dimensions(Level)), texelcoord_type(1));
#			ifndef NDEBUG
				Cache.Size = this->size(Level);
#			endif
		}
	}
}//namespace gli
