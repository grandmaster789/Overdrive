#pragma once

#include "enum_bitfield.h"

namespace overdrive {
	namespace util {
		/*
		template <typename T>
		std::underlying_type_t<T> toBitfield(T value) {
			return static_cast<std::underlying_type_t<T>>(value);
		}

		template <typename T, typename... Pack>
		std::underlying_type_t<T> toBitfield(T value, Pack... pack) {
			return
				static_cast<std::underlying_type_t<T>>(value) |
				toBitfield(pack...);
		}
		*/

		template <typename T>
		Bitfield<T>::Bitfield():
			mValue(0)
		{
		}

		template <typename T>
		Bitfield<T>::Bitfield(T value):
			mValue(0)
		{
			set(value);
		}

		template <typename T>
		Bitfield<T>::Bitfield(std::initializer_list<T> initializer):
			mValue(0)
		{
			for (T bit: initializer)
				set(bit);
		}

		template <typename T>
		Bitfield<T> operator | (const Bitfield<T>& field, T flag) {
			Bitfield<T> result = field;
			
			result.set(flag);
			
			return result;
		}

		template <typename T>
		Bitfield<T> operator | (T flag, const Bitfield<T>& field) {
			Bitfield<T> result = field;

			result.set(flag);

			return result;
		}

		template <typename T>
		Bitfield<T> Bitfield<T>::operator !() const {
			Bitfield<T> result = field;

			result.mValue = ~result.mValue;

			return result;
		}

		template <typename T>
		Bitfield<T>& Bitfield<T>::operator |= (T flag) {
			mValue |= static_cast<type>(flag);
			
			return *this;
		}

		template <typename T>
		Bitfield<T>& Bitfield<T>::operator &= (T flag) {
			mValue &= static_cast<type>(flag);

			return *this;
		}

		template <typename T>
		Bitfield<T>& Bitfield<T>::operator ^= (T flag) {
			mValue ^= static_cast<type>(flag);

			return *this;
		}

		template <typename T>
		Bitfield<T>& Bitfield<T>::operator |= (const Bitfield& field) {
			mValue |= field.mValue;

			return *this;
		}

		template <typename T>
		Bitfield<T>& Bitfield<T>::operator &= (const Bitfield& field) {
			mValue &= field.mValue;

			return *this;
		}

		template <typename T>
		Bitfield<T>& Bitfield<T>::operator ^= (const Bitfield& field) {
			mValue ^= field.mValue;

			return *this;
		}

		template <typename T>
		bool Bitfield<T>::operator == (const Bitfield& field) const {
			return (mValue == field.mValue);
		}

		template <typename T>
		bool Bitfield<T>::operator != (const Bitfield& field) const {
			return (mValue != field.mValue);
		}

		template <typename T>
		void Bitfield<T>::set(T flag) {
			mValue |= static_cast<type>(flag);
		}

		template <typename T>
		bool Bitfield<T>::isSet(T flag) const {
			return (mValue & static_cast<type>(flag)) != 0;
		}

		template <typename T>
		void Bitfield<T>::clear() {
			mValue = static_cast<type>(0);
		}

		template <typename T>
		void Bitfield<T>::clear(T flag) {
			mValue &= ~static_cast<type>(flag);
		}

		template <typename T>
		typename Bitfield<T>::type Bitfield<T>::value() const {
			return mValue;
		}
	}
}
