#pragma once

#include "vertexbuffer.h"
#include "gltypes.h"
#include <type_traits>

namespace overdrive {
	namespace render {
		template <typename T>
		VertexBuffer<T>::VertexBuffer(
			size_t numElements,
			eBufferUsage usage
		):
			Buffer(eBufferTarget::ARRAY, numElements, usage)
		{
		}

		template <typename T>
		typename VertexBuffer<T>::Data VertexBuffer<T>::map() {
			return Base::map({
				eBufferAccess::WRITE,
				eBufferAccess::INVALIDATE_BUFFER
			});
		}

		namespace detail {
			template <typename T> 
			struct CountComponents {
				static const GLint result = T::components; // this will work for glm::vec
			};

			// http://docs.gl/gl4/glVertexAttribPointer
			struct ApplyAttribPointer {
				char* mSequenceStart;
				GLsizei mSequenceSize;
				int* mCount; // some fudging here so that the operator() can be const

				template <typename tSequence>
				ApplyAttribPointer(tSequence& sample, int* count):
					mSequenceSize(sizeof(tSequence)),
					mCount(count)
				{
					mSequenceStart = reinterpret_cast<char*>(&sample);
				}

				template <typename tSequenceElement>
				void operator()(tSequenceElement& value) const {
					typedef typename tSequenceElement::value_type component_type;
					
					GLint numComponents = CountComponents<tSequenceElement>::result;
					GLenum value_constant = ToValue<component_type>::value;

					char* offset = (char*)(reinterpret_cast<char*>(&value) - mSequenceStart); // figure out how far this element is from the start of the sequence
					
					if (std::is_integral<component_type>::value)
						glVertexAttribIPointer(
							(*mCount)++,
							numComponents,
							value_constant,
							mSequenceSize,	// use the entire sequence size as the stride for this attribute
							offset
						);
					else
						glVertexAttribPointer(
							(*mCount)++,
							numComponents,
							value_constant,
							GL_FALSE, // normalized
							mSequenceSize,
							offset
						);
				}
			};
		}

		template <typename T>
		void VertexBuffer<T>::bind() {
			Base::bind();

			for (GLuint i = 0; i < getNumAttributes(); ++i)
				glEnableVertexAttribArray(i);

			T sequence;
			int count = 0;
			detail::ApplyAttribPointer helper(sequence, &count);
			boost::fusion::for_each(sequence, helper);
		}

		template <typename T>
		void VertexBuffer<T>::unbind() {
			Base::unbind();

			for (GLuint i = 0; i < getNumAttributes(); ++i)
				glDisableVertexAttribArray(i);
		}

		template <typename T>
		GLuint VertexBuffer<T>::getNumAttributes() const {
			return boost::fusion::result_of::size<T>::type::value;
		}
	}
}
