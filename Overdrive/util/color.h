#ifndef OVERDRIVE_UTIL_COLOR_H
#define OVERDRIVE_UTIL_COLOR_H

namespace overdrive {
	namespace util {
		class Color {
		public:
			Color();
			
			// unsigned char values range [0 - 255]
			// floats *should* range [0.0f - 1.0f] (no checking tho)
			// set methods that don't specify an alpha value will leave it at the previous value

			Color(float r, float g, float b);
			Color(float r, float g, float b, float a);
			Color(const float rgba[4]);
			Color(float luminance);
			
			Color(unsigned char r, unsigned char g, unsigned char b);
			Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
			Color(const unsigned char rgba[4]);
			Color(unsigned char luminance);

			void setRed(float red);
			void setGreen(float green);
			void setBlue(float blue);
			void setAlpha(float alpha);

			float getRed() const;
			float getGreen() const;
			float getBlue() const;
			float getAlpha() const;

			void setLuminance(float value);
			void setLuminance(unsigned char value);

			void setRGB(float r, float g, float b);
			void setRGB(unsigned char r, unsigned char g, unsigned char b);
			void setRGB(const float rgb[3]);
			void setRGB(const unsigned char rgb[3]);

			void setRGBA(float r, float g, float b, float a);
			void setRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
			void setRGBA(const float rgba[4]);
			void setRGBA(const unsigned char rgba[4]);

			void setElement(int index, float value);
			float getElement(int index) const;

			const float* getRGBA() const;

			static const Color WHITE;
			static const Color BLACK;
			static const Color RED;
			static const Color GREEN;
			static const Color BLUE;
			static const Color YELLOW;
			static const Color MAGENTA;
			static const Color CYAN;

			static const Color DEFAULT_AMBIENT;
			static const Color DEFAULT_DIFFUSE;
			static const Color DEFAULT_SPECULAR;
			static const Color DEFAULT_EMISSIVE;
			static const Color DEFAULT_REFLECTION;
			static const Color DEFAULT_REFRACTION;

		private:
			float mRGBA[4];
		};
	}
}

#endif
