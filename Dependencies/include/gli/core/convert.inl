#include "../core/convert_func.hpp"

namespace gli
{
	inline texture1D convert(texture1D const& Texture, format Format)
	{
		typedef float T;
		typedef typename texture1D::texelcoord_type texelcoord_type;
		typedef typename texture1D::size_type size_type;
		typedef typename texelcoord_type::value_type component_type;
		typedef typename detail::convert<texture1D, T, defaultp>::fetchFunc fetch_type;
		typedef typename detail::convert<texture1D, T, defaultp>::writeFunc write_type;

		GLI_ASSERT(!Texture.empty());
		GLI_ASSERT(!is_compressed(Texture.format()) && !is_compressed(Format));

		fetch_type Fetch = detail::convert<texture1D, T, defaultp>::call(Texture.format()).Fetch;
		write_type Write = detail::convert<texture1D, T, defaultp>::call(Format).Write;

		texture1D Copy(Format, Texture.dimensions(), Texture.levels());

		for(size_type Level = 0; Level < Texture.levels(); ++Level)
		{
			texelcoord_type const& Dimensions = Texture.dimensions(Level);

			for(component_type i = 0; i < Dimensions.x; ++i)
			{
				Write(
					Copy, texelcoord_type(i), 0, 0, Level,
					Fetch(Texture, texelcoord_type(i), 0, 0, Level));
			}
		}

		return Copy;
	}

	inline texture1DArray convert(texture1DArray const& Texture, format Format)
	{
		typedef float T;
		typedef typename texture1DArray::texelcoord_type texelcoord_type;
		typedef typename texture1DArray::size_type size_type;
		typedef typename texelcoord_type::value_type component_type;
		typedef typename detail::convert<texture1DArray, T, defaultp>::fetchFunc fetch_type;
		typedef typename detail::convert<texture1DArray, T, defaultp>::writeFunc write_type;

		GLI_ASSERT(!Texture.empty());
		GLI_ASSERT(!is_compressed(Texture.format()) && !is_compressed(Format));

		fetch_type Fetch = detail::convert<texture1DArray, T, defaultp>::call(Texture.format()).Fetch;
		write_type Write = detail::convert<texture1DArray, T, defaultp>::call(Format).Write;

		texture1DArray Copy(Format, Texture.dimensions(), Texture.layers(), Texture.levels());

		for(size_type Layer = 0; Layer < Texture.layers(); ++Layer)
		for(size_type Level = 0; Level < Texture.levels(); ++Level)
		{
			texelcoord_type const& Dimensions = Texture.dimensions(Level);

			for(component_type i = 0; i < Dimensions.x; ++i)
			{
				Write(
					Copy, texelcoord_type(i), Layer, 0, Level,
					Fetch(Texture, texelcoord_type(i), Layer, 0, Level));
			}
		}

		return Copy;
	}

	inline texture2D convert(texture2D const& Texture, format Format)
	{
		typedef float T;
		typedef typename texture2D::texelcoord_type texelcoord_type;
		typedef typename texture2D::size_type size_type;
		typedef typename texelcoord_type::value_type component_type;
		typedef typename detail::convert<texture2D, T, defaultp>::fetchFunc fetch_type;
		typedef typename detail::convert<texture2D, T, defaultp>::writeFunc write_type;

		GLI_ASSERT(!Texture.empty());
		GLI_ASSERT(!is_compressed(Texture.format()) && !is_compressed(Format));

		fetch_type Fetch = detail::convert<texture2D, T, defaultp>::call(Texture.format()).Fetch;
		write_type Write = detail::convert<texture2D, T, defaultp>::call(Format).Write;

		texture2D Copy(Format, Texture.dimensions(), Texture.levels());

		for(size_type Level = 0; Level < Texture.levels(); ++Level)
		{
			texelcoord_type const& Dimensions = Texture.dimensions(Level);

			for(component_type j = 0; j < Dimensions.y; ++j)
			for(component_type i = 0; i < Dimensions.x; ++i)
			{
				Write(
					Copy, texelcoord_type(i, j), 0, 0, Level,
					Fetch(Texture, texelcoord_type(i, j), 0, 0, Level));
			}
		}

		return Copy;
	}

	inline texture2DArray convert(texture2DArray const& Texture, format Format)
	{
		typedef float T;
		typedef typename texture2DArray::texelcoord_type texelcoord_type;
		typedef typename texture2DArray::size_type size_type;
		typedef typename texelcoord_type::value_type component_type;
		typedef typename detail::convert<texture2DArray, T, defaultp>::fetchFunc fetch_type;
		typedef typename detail::convert<texture2DArray, T, defaultp>::writeFunc write_type;

		GLI_ASSERT(!Texture.empty());
		GLI_ASSERT(!is_compressed(Texture.format()) && !is_compressed(Format));

		fetch_type Fetch = detail::convert<texture2DArray, T, defaultp>::call(Texture.format()).Fetch;
		write_type Write = detail::convert<texture2DArray, T, defaultp>::call(Format).Write;

		texture2DArray Copy(Format, Texture.dimensions(), Texture.layers(), Texture.levels());

		for(size_type Layer = 0; Layer < Texture.layers(); ++Layer)
		for(size_type Level = 0; Level < Texture.levels(); ++Level)
		{
			texelcoord_type const& Dimensions = Texture.dimensions(Level);

			for(component_type j = 0; j < Dimensions.y; ++j)
			for(component_type i = 0; i < Dimensions.x; ++i)
			{
				Write(
					Copy, texelcoord_type(i, j), Layer, 0, Level,
					Fetch(Texture, texelcoord_type(i, j), Layer, 0, Level));
			}
		}

		return Copy;
	}

	inline texture3D convert(texture3D const& Texture, format Format)
	{
		typedef float T;
		typedef typename texture3D::texelcoord_type texelcoord_type;
		typedef typename texture3D::size_type size_type;
		typedef typename texelcoord_type::value_type component_type;
		typedef typename detail::convert<texture3D, T, defaultp>::fetchFunc fetch_type;
		typedef typename detail::convert<texture3D, T, defaultp>::writeFunc write_type;

		GLI_ASSERT(!Texture.empty());
		GLI_ASSERT(!is_compressed(Texture.format()) && !is_compressed(Format));

		fetch_type Fetch = detail::convert<texture3D, T, defaultp>::call(Texture.format()).Fetch;
		write_type Write = detail::convert<texture3D, T, defaultp>::call(Format).Write;

		texture3D Copy(Format, Texture.dimensions(), Texture.levels());

		for(size_type Level = 0; Level < Texture.levels(); ++Level)
		{
			texelcoord_type const& Dimensions = Texture.dimensions(Level);

			for(component_type k = 0; k < Dimensions.z; ++k)
			for(component_type j = 0; j < Dimensions.y; ++j)
			for(component_type i = 0; i < Dimensions.x; ++i)
			{
				Write(
					Copy, texelcoord_type(i, j, k), 0, 0, Level,
					Fetch(Texture, texelcoord_type(i, j, k), 0, 0, Level));
			}
		}

		return Copy;
	}

	inline textureCube convert(textureCube const& Texture, format Format)
	{
		typedef float T;
		typedef typename textureCube::texelcoord_type texelcoord_type;
		typedef typename textureCube::size_type size_type;
		typedef typename texelcoord_type::value_type component_type;
		typedef typename detail::convert<textureCube, T, defaultp>::fetchFunc fetch_type;
		typedef typename detail::convert<textureCube, T, defaultp>::writeFunc write_type;

		GLI_ASSERT(!Texture.empty());
		GLI_ASSERT(!is_compressed(Texture.format()) && !is_compressed(Format));

		fetch_type Fetch = detail::convert<textureCube, T, defaultp>::call(Texture.format()).Fetch;
		write_type Write = detail::convert<textureCube, T, defaultp>::call(Format).Write;

		textureCube Copy(Format, Texture.dimensions(), Texture.levels());

		for(size_type Face = 0; Face < Texture.faces(); ++Face)
		for(size_type Level = 0; Level < Texture.levels(); ++Level)
		{
			texelcoord_type const& Dimensions = Texture.dimensions(Level);

			for(component_type j = 0; j < Dimensions.y; ++j)
			for(component_type i = 0; i < Dimensions.x; ++i)
			{
				Write(
					Copy, texelcoord_type(i, j), 0, Face, Level,
					Fetch(Texture, texelcoord_type(i, j), 0, Face, Level));
			}
		}

		return Copy;
	}

	inline textureCubeArray convert(textureCubeArray const& Texture, format Format)
	{
		typedef float T;
		typedef typename textureCubeArray::texelcoord_type texelcoord_type;
		typedef typename textureCubeArray::size_type size_type;
		typedef typename texelcoord_type::value_type component_type;
		typedef typename detail::convert<textureCubeArray, T, defaultp>::fetchFunc fetch_type;
		typedef typename detail::convert<textureCubeArray, T, defaultp>::writeFunc write_type;

		GLI_ASSERT(!Texture.empty());
		GLI_ASSERT(!is_compressed(Texture.format()) && !is_compressed(Format));

		fetch_type Fetch = detail::convert<textureCubeArray, T, defaultp>::call(Texture.format()).Fetch;
		write_type Write = detail::convert<textureCubeArray, T, defaultp>::call(Format).Write;

		textureCubeArray Copy(Format, Texture.dimensions(), Texture.layers(), Texture.levels());

		for(size_type Layer = 0; Layer < Texture.layers(); ++Layer)
		for(size_type Face = 0; Face < Texture.faces(); ++Face)
		for(size_type Level = 0; Level < Texture.levels(); ++Level)
		{
			texelcoord_type const& Dimensions = Texture.dimensions(Level);

			for(component_type j = 0; j < Dimensions.y; ++j)
			for(component_type i = 0; i < Dimensions.x; ++i)
			{
				Write(
					Copy, texelcoord_type(i, j), Layer, Face, Level,
					Fetch(Texture, texelcoord_type(i, j), Layer, Face, Level));
			}
		}

		return Copy;
	}
}//namespace gli

