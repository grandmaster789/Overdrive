#include "render/enumerations.h"

std::ostream& operator << (std::ostream& os, const overdrive::render::eBufferUsage& usage) {
	using overdrive::render::eBufferUsage;

	os << "BufferUsage: ";

	switch (usage) {
	case eBufferUsage::STREAM_DRAW: os << "stream draw"; break;
	case eBufferUsage::STREAM_READ: os << "stream read"; break;
	case eBufferUsage::STREAM_COPY: os << "stream copy"; break;

	case eBufferUsage::STATIC_DRAW: os << "static draw"; break;
	case eBufferUsage::STATIC_READ: os << "static read"; break;
	case eBufferUsage::STATIC_COPY: os << "static copy"; break;

	case eBufferUsage::DYNAMIC_DRAW: os << "dynamic draw"; break;
	case eBufferUsage::DYNAMIC_READ: os << "dynamic read"; break;
	case eBufferUsage::DYNAMIC_COPY: os << "dynamic copy"; break;
	default:
		os << "unknown";
	}

	return os;
}