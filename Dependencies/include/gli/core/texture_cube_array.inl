namespace gli
{
	inline textureCubeArray::textureCubeArray()
	{}

	inline textureCubeArray::textureCubeArray(format_type Format, texelcoord_type const & Dimensions, size_type Layers)
		: texture(TARGET_CUBE_ARRAY, Format, texture::texelcoord_type(Dimensions, 1), Layers, 6, gli::levels(Dimensions))
	{
		this->build_cache();
	}

	inline textureCubeArray::textureCubeArray(format_type Format, texelcoord_type const & Dimensions, size_type Layers, size_type Levels)
		: texture(TARGET_CUBE_ARRAY, Format, texture::texelcoord_type(Dimensions, 1), Layers, 6, Levels)
	{
		this->build_cache();
	}

	inline textureCubeArray::textureCubeArray(texture const & Texture)
		: texture(Texture, gli::TARGET_CUBE_ARRAY, Texture.format())
	{
		this->build_cache();
	}

	inline textureCubeArray::textureCubeArray
	(
		texture const & Texture,
		format_type Format,
		size_type BaseLayer, size_type MaxLayer,
		size_type BaseFace, size_type MaxFace,
		size_type BaseLevel, size_type MaxLevel
	)
		: texture(
			Texture, gli::TARGET_CUBE_ARRAY,
			Format,
			BaseLayer, MaxLayer,
			BaseFace, MaxFace,
			BaseLevel, MaxLevel)
	{
		this->build_cache();
	}

	inline textureCubeArray::textureCubeArray
	(
		textureCubeArray const & Texture,
		size_type BaseLayer, size_type MaxLayer,
		size_type BaseFace, size_type MaxFace,
		size_type BaseLevel, size_type MaxLevel
	)
		: texture(
			Texture, gli::TARGET_CUBE_ARRAY, Texture.format(),
			Texture.base_layer() + BaseLayer, Texture.base_layer() + MaxLayer,
			Texture.base_face() + BaseFace, Texture.base_face() + MaxFace,
			Texture.base_level() + BaseLevel, Texture.base_level() + MaxLevel)
	{
		this->build_cache();
	}

	inline textureCube textureCubeArray::operator[](size_type Layer) const
	{
		GLI_ASSERT(Layer < this->layers());

		return textureCube(
			*this, this->format(),
			this->base_layer() + Layer, this->base_layer() + Layer,
			this->base_face(), this->max_face(),
			this->base_level(), this->max_level());
	}

	inline textureCubeArray::texelcoord_type textureCubeArray::dimensions(size_type Level) const
	{
		GLI_ASSERT(!this->empty());

		return this->Caches[this->index_cache(0, 0, Level)].Dim;
	}

	template <typename genType>
	inline genType textureCubeArray::load(texelcoord_type const & TexelCoord, size_type Layer,  size_type Face, size_type Level) const
	{
		GLI_ASSERT(!this->empty());
		GLI_ASSERT(!is_compressed(this->format()));
		GLI_ASSERT(block_size(this->format()) == sizeof(genType));

		cache const & Cache = this->Caches[this->index_cache(Layer, Face, Level)];

		std::size_t const Index = linear_index(TexelCoord, Cache.Dim);
		GLI_ASSERT(Index < Cache.Size / sizeof(genType));

		return reinterpret_cast<genType const * const>(Cache.Data)[Index];
	}

	template <typename genType>
	inline void textureCubeArray::store(texelcoord_type const & TexelCoord, size_type Layer,  size_type Face, size_type Level, genType const & Texel)
	{
		GLI_ASSERT(!this->empty());
		GLI_ASSERT(!is_compressed(this->format()));
		GLI_ASSERT(block_size(this->format()) == sizeof(genType));

		cache const & Cache = this->Caches[this->index_cache(Layer, Face, Level)];
		GLI_ASSERT(glm::all(glm::lessThan(TexelCoord, Cache.Dim)));

		std::size_t const Index = linear_index(TexelCoord, Cache.Dim);
		GLI_ASSERT(Index < Cache.Size / sizeof(genType));

		reinterpret_cast<genType*>(Cache.Data)[Index] = Texel;
	}

	inline void textureCubeArray::clear()
	{
		this->texture::clear();
	}

	template <typename genType>
	inline void textureCubeArray::clear(genType const & Texel)
	{
		this->texture::clear<genType>(Texel);
	}

	template <typename genType>
	inline void textureCubeArray::clear(size_type Layer, size_type Face, size_type Level, genType const & Texel)
	{
		this->texture::clear<genType>(Layer, Face, Level, Texel);
	}

	inline textureCubeArray::size_type textureCubeArray::index_cache(size_type Layer, size_type Face, size_type Level) const
	{
		return (Layer * this->levels() * this->faces()) + (Face * this->levels()) + Level;
	}

	inline void textureCubeArray::build_cache()
	{
		this->Caches.resize(this->layers() * this->faces() * this->levels());

		for(size_type Layer = 0; Layer < this->layers(); ++Layer)
		for(size_type Face = 0; Face < this->faces(); ++Face)
		for(size_type Level = 0, Levels = this->levels(); Level < Levels; ++Level)
		{
			cache& Cache = this->Caches[this->index_cache(Layer, Face, Level)];
			Cache.Data = this->data<std::uint8_t>(Layer, Face, Level);
			Cache.Dim = glm::max(texture2D::texelcoord_type(this->texture::dimensions(Level)), texture2D::texelcoord_type(1));
#			ifndef NDEBUG
				Cache.Size = this->size(Level);
#			endif
		}
	}
}//namespace gli
