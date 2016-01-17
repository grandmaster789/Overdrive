#pragma once

#include "indexbuffer.h"
#include "gltypes.h"
#include <cassert>

namespace overdrive {
	namespace render {
		template <typename T>
		IndexBuffer<T>::IndexBuffer(
			size_t numElements, 
			eBufferUsage usage
		):
			Buffer<T>(eBufferTarget::ELEMENT_ARRAY, numElements, usage)
		{
			assert(
				(ToValue<T>::value == GL_UNSIGNED_BYTE) ||
				(ToValue<T>::value == GL_UNSIGNED_SHORT) ||
				(ToValue<T>::value == GL_UNSIGNED_INT)
			);
		}

		template <typename T>
		IndexBuffer<T>::IndexBuffer(
			std::initializer_list<T> items,
			eBufferUsage usage
		):
			Buffer<T>(eBufferTarget::ELEMENT_ARRAY, std::forward<std::initializer_list<T>>(items), usage)
		{
			assert(
				(ToValue<T>::value == GL_UNSIGNED_BYTE) ||
				(ToValue<T>::value == GL_UNSIGNED_SHORT) ||
				(ToValue<T>::value == GL_UNSIGNED_INT)
			);
		}

		template <typename T>
		typename IndexBuffer<T>::Data IndexBuffer<T>::map() {
			return Base::map({
				eBufferAccess::WRITE,
				eBufferAccess::INVALIDATE_BUFFER
			});
		}		
	}
}
