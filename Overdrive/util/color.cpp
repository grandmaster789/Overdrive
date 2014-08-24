#include "util/color.h"
#include <algorithm>

namespace overdrive {
	namespace util {
		const Color Color::WHITE				= Color(1.0f, 1.0f, 1.0f, 1.0f);
		const Color Color::BLACK				= Color(0.0f, 0.0f, 0.0f, 1.0f);
		
		const Color Color::RED					= Color(1.0f, 0.0f, 0.0f, 1.0f);
		const Color Color::GREEN				= Color(0.0f, 1.0f, 0.0f, 1.0f);
		const Color Color::BLUE					= Color(0.0f, 0.0f, 1.0f, 1.0f);
		const Color Color::YELLOW				= Color(1.0f, 1.0f, 0.0f, 1.0f);
		const Color Color::MAGENTA				= Color(1.0f, 0.0f, 1.0f, 1.0f);
		const Color Color::CYAN					= Color(0.0f, 1.0f, 1.0f, 1.0f);

		const Color Color::DEFAULT_AMBIENT		= Color(0.2f, 0.2f, 0.2f, 1.0f);
		const Color Color::DEFAULT_DIFFUSE		= Color(0.8f, 0.8f, 0.8f, 1.0f);
		const Color Color::DEFAULT_SPECULAR		= Color(0.9f, 0.9f, 0.9f, 1.0f);
		const Color Color::DEFAULT_EMISSIVE		= Color(0.0f, 0.0f, 0.0f, 1.0f);
		const Color Color::DEFAULT_REFLECTION	= Color(0.0f, 0.0f, 0.0f, 1.0f);
		const Color Color::DEFAULT_REFRACTION	= Color(0.0f, 0.0f, 0.0f, 1.0f);

		Color::Color() {
			mRGBA[0] = 0.0f;
			mRGBA[1] = 0.0f;
			mRGBA[2] = 0.0f;
			mRGBA[3] = 0.0f;
		}

		Color::Color(float r, float g, float b) {
			mRGBA[0] = r;
			mRGBA[1] = g;
			mRGBA[2] = b;
			mRGBA[3] = 1.0f;
		}

		Color::Color(float r, float g, float b, float a) {
			mRGBA[0] = r;
			mRGBA[1] = g;
			mRGBA[2] = b;
			mRGBA[3] = a;
		}

		Color::Color(const float rgba[4]) {
			mRGBA[0] = rgba[0];
			mRGBA[1] = rgba[1];
			mRGBA[2] = rgba[2];
			mRGBA[3] = rgba[3];
		}

		Color::Color(float luminance) {
			mRGBA[0] = luminance;
			mRGBA[1] = luminance;
			mRGBA[2] = luminance;
			mRGBA[3] = 1.0f;
		}

		Color::Color(unsigned char r, unsigned char g, unsigned char b) {
			mRGBA[0] = r / 255.0f;
			mRGBA[1] = g / 255.0f;
			mRGBA[2] = b / 255.0f;
			mRGBA[3] = 1.0f;
		}

		Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
			mRGBA[0] = r / 255.0f;
			mRGBA[1] = g / 255.0f;
			mRGBA[2] = b / 255.0f;
			mRGBA[3] = a / 255.0f;
		}

		Color::Color(const unsigned char rgba[4]) {
			mRGBA[0] = rgba[0] / 255.0f;
			mRGBA[1] = rgba[1] / 255.0f;
			mRGBA[2] = rgba[2] / 255.0f;
			mRGBA[3] = rgba[3] / 255.0f;
		}

		Color::Color(unsigned char luminance) {
			float value = luminance / 255.0f;

			mRGBA[0] = value;
			mRGBA[1] = value;
			mRGBA[2] = value;
			mRGBA[3] = 1.0f;
		}

		void Color::setRed(float red) {
			mRGBA[0] = red;
		}

		void Color::setGreen(float green) {
			mRGBA[1] = green;
		}

		void Color::setBlue(float blue) {
			mRGBA[2] = blue;
		}

		void Color::setAlpha(float alpha) {
			mRGBA[3] = alpha;
		}

		float Color::getRed() const {
			return mRGBA[0];
		}

		float Color::getGreen() const {
			return mRGBA[1];
		}

		float Color::getBlue() const {
			return mRGBA[2];
		}

		float Color::getAlpha() const {
			return mRGBA[3];
		}

		void Color::setLuminance(float value) {
			mRGBA[0] = value;
			mRGBA[1] = value;
			mRGBA[2] = value;
		}

		void Color::setLuminance(unsigned char value) {
			setLuminance(value / 255.0f);
		}

		void Color::setRGB(float r, float g, float b) {
			mRGBA[0] = r;
			mRGBA[1] = g;
			mRGBA[2] = b;
		}

		void Color::setRGB(unsigned char r, unsigned char g, unsigned char b) {
			mRGBA[0] = r / 255.0f;
			mRGBA[1] = g / 255.0f;
			mRGBA[2] = b / 255.0f;
		}

		void Color::setRGB(const float rgb[3]) {
			mRGBA[0] = rgb[0];
			mRGBA[1] = rgb[1];
			mRGBA[2] = rgb[2];
		}

		void Color::setRGB(const unsigned char rgb[3]) {
			mRGBA[0] = rgb[0] / 255.0f;
			mRGBA[1] = rgb[1] / 255.0f;
			mRGBA[2] = rgb[2] / 255.0f;
		}

		void Color::setRGBA(float r, float g, float b, float a) {
			mRGBA[0] = r;
			mRGBA[1] = g;
			mRGBA[2] = b;
			mRGBA[3] = a;
		}

		void Color::setRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
			mRGBA[0] = r / 255.0f;
			mRGBA[1] = g / 255.0f;
			mRGBA[2] = b / 255.0f;
			mRGBA[3] = a / 255.0f;
		}

		void Color::setRGBA(const float rgba[4]) {
			mRGBA[0] = rgba[0];
			mRGBA[1] = rgba[1];
			mRGBA[2] = rgba[2];
			mRGBA[3] = rgba[3];
		}

		void Color::setRGBA(const unsigned char rgba[4]) {
			mRGBA[0] = rgba[0] / 255.0f;
			mRGBA[1] = rgba[1] / 255.0f;
			mRGBA[2] = rgba[2] / 255.0f;
			mRGBA[3] = rgba[3] / 255.0f;
		}

		void Color::setElement(int index, float value) {
			mRGBA[index] = value;
		}

		float Color::getElement(int index) const {
			return mRGBA[index];
		}

		const float* Color::getRGBA() const {
			return mRGBA;
		}
	}
}
