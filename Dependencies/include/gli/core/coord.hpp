#pragma once

#include "../type.hpp"

namespace gli{
namespace detail
{
	template <typename T, precision P, template <typename, precision> class vecType>
	inline vecType<bool, P> in_interval(vecType<T, P> const & Value, vecType<T, P> const & Min, vecType<T, P> const & Max)
	{
		return greaterThanEqual(Value, Min) && lessThanEqual(Value, Max);
	}

	template <typename texelcoord_type, typename samplecoord_type>
	struct coord_nearest
	{
		texelcoord_type Texel;
		typename texelcoord_type::bool_type UseTexel;
	};

	template <typename texelcoord_type, typename samplecoord_type>
	inline coord_nearest<texelcoord_type, samplecoord_type> make_coord_nearest(texelcoord_type const & TexelDim, samplecoord_type const & SampleCoord)
	{
		samplecoord_type const TexelLast(samplecoord_type(TexelDim) - samplecoord_type(1));

		coord_nearest<texelcoord_type, samplecoord_type> Coord;
		Coord.Texel = texelcoord_type(round(SampleCoord * TexelLast));
		Coord.UseTexel = in_interval(Coord.Texel, texelcoord_type(0), TexelDim - 1);
		return Coord;
	}

	template <typename texelcoord_type, typename samplecoord_type>
	struct coord_linear
	{
		texelcoord_type TexelFloor;
		texelcoord_type TexelCeil;
		samplecoord_type Blend;
	};

	template <typename texelcoord_type, typename samplecoord_type>
	struct coord_linear_border : public coord_linear<texelcoord_type, samplecoord_type>
	{
		typename texelcoord_type::bool_type UseTexelFloor;
		typename texelcoord_type::bool_type UseTexelCeil;
	};

	template <typename texelcoord_type, typename samplecoord_type>
	GLI_FORCE_INLINE coord_linear<texelcoord_type, samplecoord_type> make_coord_linear(texelcoord_type const & TexelDim, samplecoord_type const & SampleCoord)
	{
		coord_linear<texelcoord_type, samplecoord_type> Coord;

		samplecoord_type const TexelDimF(TexelDim);
		samplecoord_type const TexelLast = TexelDimF - samplecoord_type(1);
		samplecoord_type const ScaledCoord(SampleCoord * TexelLast);
		samplecoord_type const ScaledCoordFloor(floor(ScaledCoord));
		samplecoord_type const ScaledCoordCeil(ceil(ScaledCoord));

		Coord.Blend = ScaledCoord - ScaledCoordFloor;
		Coord.TexelFloor = texelcoord_type(ScaledCoordFloor);
		Coord.TexelCeil = texelcoord_type(ScaledCoordCeil);

		return Coord;
	}

	template <typename texelcoord_type, typename samplecoord_type>
	GLI_FORCE_INLINE coord_linear_border<texelcoord_type, samplecoord_type> make_coord_linear_border(texelcoord_type const & TexelDim, samplecoord_type const & SampleCoord)
	{
		coord_linear_border<texelcoord_type, samplecoord_type> Coord;

		samplecoord_type const TexelDimF(TexelDim);
		samplecoord_type const TexelLast = TexelDimF - samplecoord_type(1);
		samplecoord_type const ScaledCoord(SampleCoord * TexelLast);
		samplecoord_type const ScaledCoordFloor(floor(ScaledCoord));
		samplecoord_type const ScaledCoordCeil(ceil(ScaledCoord));

		Coord.Blend = ScaledCoord - ScaledCoordFloor;
		Coord.TexelFloor = texelcoord_type(ScaledCoordFloor);
		Coord.TexelCeil = texelcoord_type(ScaledCoordCeil);
		Coord.UseTexelFloor = in_interval(Coord.TexelFloor, texelcoord_type(0), TexelDim - 1);
		Coord.UseTexelCeil = in_interval(Coord.TexelCeil, texelcoord_type(0), TexelDim - 1);

		return Coord;
	}
}//namespace detail
}//namespace gli
