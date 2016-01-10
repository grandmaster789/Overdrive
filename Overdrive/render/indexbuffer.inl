#pragma once

#include "indexbuffer.h"

namespace overdrive {
	namespace render {
		template <typename T>
		IndexBuffer<T>::IndexBuffer(
			size_t numElements, 
			eBufferUsage usage
		):
			Buffer<T>(eBufferTarget::ELEMENT_ARRAY, numElements, usage)
		{
		}

		template <typename T>
		typename IndexBuffer<T>::Data IndexBuffer<T>::map() {
			return Base::map(
				eBufferAccess::WRITE, 
				eBufferAccess::INVALIDATE_BUFFER
			);
		}		
	}
}
