#pragma once

#include <unordered_map>
#include <typeindex>
#include <typeinfo>

namespace overdrive {
	namespace util {
		template <typename T>
		class Type {
		private:
			Type(std::type_index t);

		public:
			static Type<T>& getType(std::type_index idx);

			template <typename U>
			static size_t getIndex();

		private:
			static size_t sNextTypeIndex;
			static std::unordered_map<std::type_index, Type<T>> sTypeMap;

			size_t mTypeIndex;
			std::type_index mType;
		};
	}
}

#include "typemap.inl"