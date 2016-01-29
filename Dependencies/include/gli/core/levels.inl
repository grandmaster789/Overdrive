#include <glm/gtc/integer.hpp>
#include <glm/gtx/component_wise.hpp>

namespace gli
{
	/// Compute the number of texture levels necessary to build a complete mipmap chain.
	template <typename T, precision P, template <typename, precision> class vecType>
	inline T levels(vecType<T, P> const & Dimensions)
	{
		return glm::log2(compMax(Dimensions)) + static_cast<T>(1);
	}

	/// Compute the number of texture levels necessary to build a complete mipmap chain.
	inline size_t levels(size_t Dimensions)
	{
		return glm::log2(Dimensions) + static_cast<size_t>(1);
	}

	/// Compute the number of texture levels necessary to build a complete mipmap chain.
	inline int levels(int Dimensions)
	{
		return glm::log2(Dimensions) + static_cast<int>(1);
	}
}//namespace gli
