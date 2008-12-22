#ifndef _POLYGON_H_
#define _POLYGON_H_

#include <stdint.h>
#include <cstdlib>

struct Colour {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

class Polygon {
	public:
		Polygon();
		Polygon(size_t num, const int16_t* x, const int16_t* y, struct Colour colour);
		Polygon(const Polygon&);

		~Polygon();

		Polygon& operator=(const Polygon&);

		size_t num() const;
		const int16_t* x() const;
		const int16_t* y() const;
		const struct Colour& colour() const;

	private:
		size_t _cap;
		size_t _num;
		int16_t* _x;
		int16_t* _y;
		struct Colour _colour;
};

#endif
