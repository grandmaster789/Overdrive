#pragma once

#include "../opengl.h"

namespace overdrive {
	namespace render {
		// http://docs.gl/gl4/glMapBufferRange
		enum class eBufferAccess: GLenum {
			READ				= GL_MAP_READ_BIT,
			WRITE				= GL_MAP_WRITE_BIT,
			PERSISTENT			= GL_MAP_PERSISTENT_BIT,
			COHERENT			= GL_MAP_COHERENT_BIT,
			INVALIDATE_BUFFER	= GL_MAP_INVALIDATE_BUFFER_BIT,
			INVALIDATE_RANGE	= GL_MAP_INVALIDATE_RANGE_BIT,
			FLUSH_EXPLICIT		= GL_MAP_FLUSH_EXPLICIT_BIT,
			UNSYNCHRONIZED		= GL_MAP_UNSYNCHRONIZED_BIT
		};

		// http://docs.gl/gl4/glBufferData
		enum class eBufferUsage : GLenum {
			STREAM_DRAW = GL_STREAM_DRAW,
			STREAM_READ = GL_STREAM_READ,
			STREAM_COPY = GL_STREAM_COPY,

			STATIC_DRAW = GL_STATIC_DRAW,
			STATIC_READ = GL_STATIC_READ,
			STATIC_COPY = GL_STATIC_COPY,

			DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
			DYNAMIC_READ = GL_DYNAMIC_READ,
			DYNAMIC_COPY = GL_DYNAMIC_COPY
		};
		
		// http://docs.gl/gl4/glBindBuffer
		enum class eBufferTarget: GLenum {
			ARRAY				= GL_ARRAY_BUFFER,				// vertex attributes
			ATOMIC_COUNTER		= GL_ATOMIC_COUNTER_BUFFER,		// atomic counter storage
			COPY_READ			= GL_COPY_READ_BUFFER,			// buffer copy source
			COPY_WRITE			= GL_COPY_WRITE_BUFFER,			// buffer copy destination
			DISPATCH_INDIRECT	= GL_DISPATCH_INDIRECT_BUFFER,	// indirect compute dispatch commands
			DRAW_INDIRECT		= GL_DRAW_INDIRECT_BUFFER,		// indirect draw commands
			ELEMENT_ARRAY		= GL_ELEMENT_ARRAY_BUFFER,		// vertex array indices
			PIXEL_PACK			= GL_PIXEL_PACK_BUFFER,			// pixel read target
			PIXEL_UNPACK		= GL_PIXEL_UNPACK_BUFFER,		// texture data source
			QUERY				= GL_QUERY_BUFFER,				// query result
			SHADER_STORAGE		= GL_SHADER_STORAGE_BUFFER,		// read/write storage for shaders
			TEXTURE				= GL_TEXTURE_BUFFER,			// texture data
			TRANSFORM_FEEDBACK	= GL_TRANSFORM_FEEDBACK_BUFFER,	// transform feedback buffer
			UNIFORM				= GL_UNIFORM_BUFFER				// uniform block storage
		};

		// [NOTE] bind/unbind looks like it could be RAII material as well
		// [NOTE] maybe include things like cbegin() and cend() in the Data class?
		// [NOTE] perhaps cache the bind/unbind status and provide an interface to query it?
		template <typename T>
		class Buffer {
		private:
			class Data {
			public:
				Data(Buffer* buffer, GLbitfield access);
				~Data();

				Buffer* getBuffer() const;

				T* getMemory() const;
				size_t size() const;

				T* begin();
				T* end();
				T& operator[](size_t index); // [NOTE] doesn't do any bounds checking!

				const T* begin() const;
				const T* end() const;
				const T& operator[](size_t index) const;  // [NOTE] doesn't do any bounds checking!

			private:
				Buffer* mBuffer;
				size_t mNumItems;
				T* mHostMemory;
			};

		public:
			Buffer(eBufferTarget target, size_t numItems, eBufferUsage usage);
			virtual ~Buffer();

			Buffer(const Buffer&) = delete;
			Buffer(Buffer&& b) = delete;
			Buffer& operator = (const Buffer&) = delete;
			Buffer& operator = (Buffer&& b) = delete;

			GLuint getHandle() const;
			eBufferTarget getTarget() const;
			eBufferUsage getUsage() const;

			template <typename... Pack>
			Data map(eBufferAccess access, Pack... pack); // ex: auto data = map(eBufferAccess::WRITE, eBufferAccess::INVALIDATE_BUFFER);
			bool isMapped() const;

			void bind();
			void unbind();

		protected:
			eBufferTarget mTarget;
			eBufferUsage mUsage;
			GLuint mHandle;
			size_t mNumItems;
			bool mIsMapped;
		};
	}
}

#include "buffer.inl"