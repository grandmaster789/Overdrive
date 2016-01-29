namespace gli
{
	template <typename genType>
	inline genType texel_fetch
	(
		texture2D const & Texture,
		texture2D::dim_type const & TexCoord,
		texture2D::size_type const & Level
	)
	{
		assert(!Texture.empty());
		assert(!is_compressed(Texture.format()));
		assert(block_size(Texture.format()) == sizeof(genType));

		image::dim_type Dimensions = Texture[Level].dimensions();
		genType const * const Data = reinterpret_cast<genType const * const >(Texture[Level].data());

		return reinterpret_cast<genType const * const>(Data)[TexCoord.x + TexCoord.y * Dimensions.x];
	}

	template <typename genType>
	void texel_write
	(
		texture2D & Texture,
		texture2D::dim_type const & Texcoord,
		texture2D::size_type const & Level,
		genType const & Color
	)
	{
		assert(!Texture.empty());
		assert(!is_compressed(Texture.format()));
		assert(block_size(Texture.format()) == sizeof(genType));

		genType * Data = Texture[Level].data<genType>();
		std::size_t Index = Texcoord.x + Texcoord.y * Texture[Level].dimensions().x;
		
		std::size_t Capacity = Texture[Level].size();
		assert(Index < Capacity);

		*(Data + Index) = Color;
	}

	template <typename genType>
	inline genType texture_lod
	(
		texture2D const & Texture,
		texture2D::samplecoord_type const & SampleCoord,
		texture2D::size_type const & Level
	)
	{
		//assert(Texture.format() == R8U || Texture.format() == RG8U || Texture.format() == RGB8U || Texture.format() == RGBA8U);

		image::dim_type Dimensions = Texture[Level].dimensions();
		genType const * const Data = reinterpret_cast<genType const * const>(Texture[Level].data());

		std::size_t s_below = std::size_t(glm::floor(SampleCoord.s * float(Dimensions.x - 1)));
		std::size_t s_above = std::size_t(glm::ceil( SampleCoord.s * float(Dimensions.x - 1)));
		std::size_t t_below = std::size_t(glm::floor(SampleCoord.t * float(Dimensions.y - 1)));
		std::size_t t_above = std::size_t(glm::ceil( SampleCoord.t * float(Dimensions.y - 1)));

		float s_below_normalized = s_below / float(Dimensions.x);
		float t_below_normalized = t_below / float(Dimensions.y);

		genType Value1 = reinterpret_cast<genType const * const>(Data)[s_below + t_below * Dimensions.x];
		genType Value2 = reinterpret_cast<genType const * const>(Data)[s_above + t_below * Dimensions.x];
		genType Value3 = reinterpret_cast<genType const * const>(Data)[s_above + t_above * Dimensions.x];
		genType Value4 = reinterpret_cast<genType const * const>(Data)[s_below + t_above * Dimensions.x];

		float BlendA = float(SampleCoord.s - s_below_normalized) * float(Dimensions.x - 1);
		float BlendB = float(SampleCoord.s - s_below_normalized) * float(Dimensions.x - 1);
		float BlendC = float(SampleCoord.t - t_below_normalized) * float(Dimensions.y - 1);

		genType ValueA(glm::mix(Value1, Value2, BlendA));
		genType ValueB(glm::mix(Value4, Value3, BlendB));

		return genType(glm::mix(ValueA, ValueB, BlendC));
	}

}//namespace gli
