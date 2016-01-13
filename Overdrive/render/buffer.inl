#pragma once

#include "buffer.h"
#include "../util/enum_bitfield.h"
#include <exception>

namespace overdrive {
	namespace render {
		// ----- Buffer::Data -----
		template <typename T>
		Buffer<T>::Data::Data(Buffer* buffer, GLbitfield access):
			mBuffer(buffer),
			mNumItems(buffer->mNumItems),
			mHostMemory(nullptr)
		{
			assert(buffer);

			GLenum target = static_cast<GLenum>(buffer->getTarget());
			GLsizeiptr numBytes = mNumItems * sizeof(T);

			glBindBuffer(target, buffer->getHandle());

			mHostMemory = reinterpret_cast<T*>(glMapBufferRange(
				target,		// Specifies the name of the buffer object
				0,			// Specifies the starting offset within the buffer of the range to be mapped
				numBytes,	// Specifies the length of the range to be mapped
				access		// Specifies a combination of access flags indicating the desired access to the mapped range
			));

			glBindBuffer(target, 0);

			if (!mHostMemory)
				throw std::runtime_error("Could not map buffer");

			buffer->mIsMapped = true;
		}

		template <typename T>
		Buffer<T>::Data::~Data() {
			GLenum target = static_cast<GLenum>(mBuffer->getTarget());
			
			glBindBuffer(target, mBuffer->getHandle());
			glUnmapBuffer(target);
			glBindBuffer(target, 0);

			mBuffer->mIsMapped = false;
		}

		template <typename T>
		Buffer<T>* Buffer<T>::Data::getBuffer() const {
			return mBuffer;
		}

		template <typename T>
		T* Buffer<T>::Data::getMemory() const {
			return mHostMemory;
		}

		template <typename T>
		size_t Buffer<T>::Data::size() const {
			return mNumItems;
		}

		template <typename T>
		T* Buffer<T>::Data::begin() {
			return mHostMemory;
		}

		template <typename T>
		T* Buffer<T>::Data::end() {
			return mHostMemory + mNumItems;
		}

		template <typename T>
		T& Buffer<T>::Data::operator[](size_t index) {
			return mHostMemory[index];
		}

		template <typename T>
		const T* Buffer<T>::Data::begin() const {
			return mHostMemory;
		}

		template <typename T>
		const T* Buffer<T>::Data::end() const {
			return mHostMemory + mNumItems;
		}
		
		template <typename T>
		const T& Buffer<T>::Data::operator[](size_t index) const {
			return mHostMemory[index];
		}

		// ----- Buffer -----
		template <typename T>
		Buffer<T>::Buffer(
			eBufferTarget target_, 
			size_t numItems, 
			eBufferUsage usage
		):
			mTarget(target_),
			mUsage(usage),
			mHandle(0),
			mNumItems(numItems),
			mIsMapped(false)
		{
			GLenum target = static_cast<GLenum>(target_);
			size_t numBytes = numItems * sizeof(T);

			glGenBuffers(1, &mHandle);
			glBindBuffer(target, mHandle);
			glBufferData(target, numBytes, nullptr, static_cast<GLenum>(usage));
			glBindBuffer(target, 0);
		}

		template <typename T>
		Buffer<T>::~Buffer() {
			if (mHandle)
				glDeleteBuffers(1, &mHandle);
		}

		template <typename T>
		GLuint Buffer<T>::getHandle() const {
			return mHandle;
		}

		template <typename T>
		eBufferTarget Buffer<T>::getTarget() const {
			return mTarget;
		}

		template <typename T>
		eBufferUsage Buffer<T>::getUsage() const {
			return mUsage;
		}

		template <typename T>
		size_t Buffer<T>::getSize() const {
			return mNumItems;
		}

		/*
		template <typename T>
		template <typename... Pack>
		typename Buffer<T>::Data Buffer<T>::map(eBufferAccess access, Pack... pack) {
			if (mIsMapped)
				throw std::runtime_error("Buffer is already mapped");

			return Data(this, util::toBitfield(access, pack...));
		}
		*/
		template <typename T>
		typename Buffer<T>::Data Buffer<T>::map(std::initializer_list<eBufferAccess> access) {
			if (mIsMapped)
				throw std::runtime_error("Buffer is already mapped");

			util::Bitfield<eBufferAccess> bits(access);

			return Data(this, bits.value());
		}

		template <typename T>
		bool Buffer<T>::isMapped() const {
			return mIsMapped;
		}

		template <typename T>
		void Buffer<T>::bind() {
			if (mIsMapped)
				throw std::runtime_error("Cannot bind a mapped buffer");

			GLenum target = static_cast<GLenum>(mTarget);
			glBindBuffer(target, mHandle);
		}

		template <typename T>
		void Buffer<T>::unbind() {
			GLenum target = static_cast<GLenum>(mTarget);
			glBindBuffer(target, 0);
		}

		// ----- ostream << -----
		std::ostream& operator << (std::ostream& os, const eBufferAccess& value) {
			switch (value) {
			case eBufferAccess::READ:				os << "read"; break;
			case eBufferAccess::WRITE:				os << "write"; break;
			case eBufferAccess::PERSISTENT:			os << "persistent"; break;
			case eBufferAccess::COHERENT:			os << "coherent"; break;
			case eBufferAccess::INVALIDATE_BUFFER:	os << "invalidate buffer"; break;
			case eBufferAccess::INVALIDATE_RANGE:	os << "invalidate range"; break;
			case eBufferAccess::FLUSH_EXPLICIT:		os << "flush explicit"; break;
			case eBufferAccess::UNSYNCHRONIZED:		os << "unsynchronized"; break;

			default:
				os << "Unknown buffer access type: " << static_cast<std::underlying_type<eBufferAccess>::type>(value);
			}

			return os;
		}

		std::ostream& operator << (std::ostream& os, const eBufferUsage& value) {
			switch (value) {
			case eBufferUsage::STREAM_DRAW: os << "stream draw"; break;
			case eBufferUsage::STREAM_READ: os << "stream draw"; break;
			case eBufferUsage::STREAM_COPY: os << "stream draw"; break;

			case eBufferUsage::STATIC_DRAW: os << "static draw"; break;
			case eBufferUsage::STATIC_READ: os << "static draw"; break;
			case eBufferUsage::STATIC_COPY: os << "static draw"; break;

			case eBufferUsage::DYNAMIC_DRAW: os << "dynamic draw"; break;
			case eBufferUsage::DYNAMIC_READ: os << "dynamic draw"; break;
			case eBufferUsage::DYNAMIC_COPY: os << "dynamic draw"; break;

			default:
				os << "Unknown buffer usage: " << static_cast<std::underlying_type<eBufferAccess>::type>(value);
			}

			return os;
		}

		std::ostream& operator << (std::ostream& os, const eBufferTarget& value) {
			switch (value) {
			case eBufferTarget::ARRAY:				os << "array"; break;
			case eBufferTarget::ATOMIC_COUNTER:		os << "atomic counter"; break;
			case eBufferTarget::COPY_READ:			os << "copy read"; break;
			case eBufferTarget::COPY_WRITE:			os << "copy write"; break;
			case eBufferTarget::DISPATCH_INDIRECT:	os << "dispatch indirect"; break;
			case eBufferTarget::DRAW_INDIRECT:		os << "draw indirect"; break;
			case eBufferTarget::ELEMENT_ARRAY:		os << "element array"; break;
			case eBufferTarget::PIXEL_PACK:			os << "pixel pack"; break;
			case eBufferTarget::PIXEL_UNPACK:		os << "pixel unpack"; break;
			case eBufferTarget::QUERY:				os << "query"; break;
			case eBufferTarget::SHADER_STORAGE:		os << "shader storage"; break;
			case eBufferTarget::TEXTURE:			os << "texture"; break;
			case eBufferTarget::TRANSFORM_FEEDBACK: os << "transform feedback"; break;
			case eBufferTarget::UNIFORM:			os << "uniform"; break;
			}
			return os;
		}
	}
}