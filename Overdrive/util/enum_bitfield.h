#pragma once

#include <type_traits>

namespace overdrive {
	namespace util {
		/*
			This converts an enum class to its underlying type and combines all 
			of them in a single bitfield. Doesn't do any real checking, use at
			your own discretion.
		*/
		//template <typename T, typename... Pack>
		//std::underlying_type_t<T> toBitfield(T value, Pack... pack);

		/*
			Wraps an enum class

			[TODO] iostream operators?
			[TODO] constexpr/noexcept
			[TODO] ordering (less) operator?
		*/
		template <typename T>
		class Bitfield {
		public:
			typedef typename std::underlying_type_t<T> type;

			Bitfield();
			Bitfield(T value);
			Bitfield(std::initializer_list<T> initializer);

			Bitfield(const Bitfield&) = default;
			Bitfield(Bitfield&&) = default;
			Bitfield& operator = (const Bitfield&) = default;
			Bitfield& operator = (Bitfield&&) = default;

			friend Bitfield operator | (const Bitfield& field, T flag);
			friend Bitfield operator | (T flag, const Bitfield& field);

			Bitfield operator !() const; // ~ kind of wonky

			Bitfield& operator |= (T flag);
			Bitfield& operator &= (T flag);
			Bitfield& operator ^= (T flag);
			Bitfield& operator |= (const Bitfield& field);
			Bitfield& operator &= (const Bitfield& field);
			Bitfield& operator ^= (const Bitfield& field);
			
			bool operator == (const Bitfield& field) const;
			bool operator != (const Bitfield& field) const;

			void set(T flag);
			bool isSet(T flag) const;
			void clear();
			void clear(T flag);

			type value() const;

		private:
			type mValue;
		};
	}
}

#include "enum_bitfield.inl"