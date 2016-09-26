#pragma once

#include "typemap.h"
#include <cassert>

namespace overdrive {
	namespace util {
		template <typename T>
		size_t Type<T>::sNextTypeIndex;

		template <typename T>
		std::unordered_map<std::type_index, Type<T>> Type<T>::sTypeMap;

		template <typename T>
		Type<T>::Type(std::type_index t):
			mTypeIndex(sNextTypeIndex++),
			mType(t)
		{
		}

		template <typename T>
		Type<T>& Type<T>::getType(std::type_index idx) {
			if (sTypeMap.count(idx) <= 0)
				sTypeMap.emplace(idx, Type<T>(idx));

			auto it = sTypeMap.find(idx);
			assert(it != sTypeMap.end());
			return it->second;
		}

		template <typename T>
		template <typename U>
		size_t Type<T>::getIndex() {
			return Type::getType(std::type_index(typeid(U))).mTypeIndex;
		}
	}
}