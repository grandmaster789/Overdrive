namespace gli
{
	inline image view(image const & Image)
	{
		return Image;
	}

	inline texture view(texture const & Texture)
	{
		return Texture;
	}

	template <typename texType>
	inline texture view(texType const & Texture)
	{
		return Texture;
	}

	inline texture view
	(
		texture const & Texture,
		texture::size_type BaseLayer, texture::size_type MaxLayer,
		texture::size_type BaseFace, texture::size_type MaxFace,
		texture::size_type BaseLevel, texture::size_type MaxLevel
	)
	{
		GLI_ASSERT(!Texture.empty());
		GLI_ASSERT(BaseLevel >= 0 && BaseLevel < Texture.levels() && MaxLevel >= 0 && MaxLevel < Texture.levels() && BaseLevel <= MaxLevel);
		GLI_ASSERT(BaseFace >= 0 && BaseFace < Texture.faces() && MaxFace >= 0 && MaxFace < Texture.faces() && BaseFace <= MaxFace);
		GLI_ASSERT(BaseLayer >= 0 && BaseLayer < Texture.layers() && MaxLayer >= 0 && MaxLayer < Texture.layers() && BaseLayer <= MaxLayer);

		return texture(
			Texture, Texture.target(), Texture.format(),
			Texture.base_layer() + BaseLayer, Texture.base_layer() + MaxLayer,
			Texture.base_face() + BaseFace, Texture.base_face() + MaxFace,
			Texture.base_level() + BaseLevel, Texture.base_level() + MaxLevel);
	}

	template <typename texType>
	inline texture view(texType const & Texture, format Format)
	{
		if(block_size(Texture.format()) != block_size(Format))
			return texture();

		return texture(Texture, Texture.target(), Format);
	}

	inline texture view
	(
		texture1D const & Texture,
		texture1D::size_type BaseLevel, texture1D::size_type MaxLevel
	)
	{
		GLI_ASSERT(!Texture.empty());
		GLI_ASSERT(BaseLevel >= 0 && BaseLevel < Texture.levels() && MaxLevel >= 0 && MaxLevel < Texture.levels() && BaseLevel <= MaxLevel);

		return texture(
			Texture, TARGET_1D, Texture.format(),
			Texture.base_layer(), Texture.max_layer(),
			Texture.base_face(), Texture.max_face(),
			Texture.base_level() + BaseLevel, Texture.base_level() + MaxLevel);
	}

	inline texture view
	(
		texture1DArray const & Texture,
		texture1DArray::size_type BaseLayer, texture1DArray::size_type MaxLayer,
		texture1DArray::size_type BaseLevel, texture1DArray::size_type MaxLevel
	)
	{
		GLI_ASSERT(!Texture.empty());
		GLI_ASSERT(BaseLevel >= 0 && BaseLevel < Texture.levels() && MaxLevel >= 0 && MaxLevel < Texture.levels() && BaseLevel <= MaxLevel);
		GLI_ASSERT(BaseLayer >= 0 && BaseLayer < Texture.layers() && MaxLayer >= 0 && MaxLayer < Texture.layers() && BaseLayer <= MaxLayer);

		return texture(
			Texture, TARGET_1D_ARRAY, Texture.format(),
			Texture.base_layer() + BaseLayer, Texture.base_layer() + MaxLayer,
			Texture.base_face(), Texture.max_face(),
			Texture.base_level() + BaseLevel, Texture.base_level() + MaxLevel);
	}

	inline texture view
	(
		texture2D const & Texture,
		texture2D::size_type BaseLevel, texture2D::size_type MaxLevel
	)
	{
		GLI_ASSERT(!Texture.empty());
		GLI_ASSERT(BaseLevel >= 0 && BaseLevel < Texture.levels() && MaxLevel >= 0 && MaxLevel < Texture.levels() && BaseLevel <= MaxLevel);

		return texture(
			Texture, TARGET_2D, Texture.format(),
			Texture.base_layer(), Texture.max_layer(),
			Texture.base_face(), Texture.max_face(),
			Texture.base_level() + BaseLevel, Texture.base_level() + MaxLevel);
	}

	inline texture view
	(
		texture2DArray const & Texture,
		texture2DArray::size_type BaseLayer, texture2DArray::size_type MaxLayer,
		texture2DArray::size_type BaseLevel, texture2DArray::size_type MaxLevel
	)
	{
		GLI_ASSERT(!Texture.empty());
		GLI_ASSERT(BaseLevel >= 0 && BaseLevel < Texture.levels() && MaxLevel >= 0 && MaxLevel < Texture.levels() && BaseLevel <= MaxLevel);
		GLI_ASSERT(BaseLayer >= 0 && BaseLayer < Texture.layers() && MaxLayer >= 0 && MaxLayer < Texture.layers() && BaseLayer <= MaxLayer);

		return texture(
			Texture, TARGET_2D_ARRAY, Texture.format(),
			Texture.base_layer() + BaseLayer, Texture.base_layer() + MaxLayer,
			Texture.base_face(), Texture.max_face(),
			Texture.base_level() + BaseLevel, Texture.base_level() + MaxLevel);
	}

	inline texture view
	(
		texture3D const & Texture,
		texture3D::size_type BaseLevel, texture3D::size_type MaxLevel
	)
	{
		GLI_ASSERT(!Texture.empty());
		GLI_ASSERT(BaseLevel >= 0 && BaseLevel < Texture.levels() && MaxLevel >= 0 && MaxLevel < Texture.levels() && BaseLevel <= MaxLevel);

		return texture(
			Texture, TARGET_3D, Texture.format(),
			Texture.base_layer(), Texture.max_layer(),
			Texture.base_face(), Texture.max_face(),
			Texture.base_level() + BaseLevel, Texture.base_level() + MaxLevel);
	}

	inline texture view
	(
		textureCube const & Texture,
		textureCube::size_type BaseFace, textureCube::size_type MaxFace,
		textureCube::size_type BaseLevel, textureCube::size_type MaxLevel
	)
	{
		GLI_ASSERT(!Texture.empty());
		GLI_ASSERT(BaseLevel >= 0 && BaseLevel < Texture.levels() && MaxLevel >= 0 && MaxLevel < Texture.levels() && BaseLevel <= MaxLevel);
		GLI_ASSERT(BaseFace >= 0 && BaseFace < Texture.faces() && MaxFace >= 0 && MaxFace < Texture.faces() && BaseFace <= MaxFace);

		return texture(
			Texture, TARGET_CUBE, Texture.format(),
			Texture.base_layer(), Texture.max_layer(),
			Texture.base_face(), Texture.base_face() + MaxFace,
			Texture.base_level() + BaseLevel, Texture.base_level() + MaxLevel);
	}

	inline texture view
	(
		textureCubeArray const & Texture,
		textureCubeArray::size_type BaseLayer, textureCubeArray::size_type MaxLayer,
		textureCubeArray::size_type BaseFace, textureCubeArray::size_type MaxFace,
		textureCubeArray::size_type BaseLevel, textureCubeArray::size_type MaxLevel
	)
	{
		GLI_ASSERT(!Texture.empty());
		GLI_ASSERT(BaseLevel >= 0 && BaseLevel < Texture.levels() && MaxLevel >= 0 && MaxLevel < Texture.levels() && BaseLevel <= MaxLevel);
		GLI_ASSERT(BaseFace >= 0 && BaseFace < Texture.faces() && MaxFace >= 0 && MaxFace < Texture.faces() && BaseFace <= MaxFace);
		GLI_ASSERT(BaseLayer >= 0 && BaseLayer < Texture.layers() && MaxLayer >= 0 && MaxLayer < Texture.layers() && BaseLayer <= MaxLayer);

		return texture(
			Texture, TARGET_CUBE_ARRAY, Texture.format(),
			Texture.base_layer() + BaseLayer, Texture.base_layer() + MaxLayer,
			Texture.base_face() + BaseFace, Texture.base_face() + MaxFace,
			Texture.base_level() + BaseLevel, Texture.base_level() + MaxLevel);
	}
}//namespace gli
