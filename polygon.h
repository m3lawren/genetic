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
		Polygon(size_t num, const uint32_t* x, const uint32_t* y, struct Colour colour);
		Polygon(const Polygon&);

		~Polygon();

		size_t num() const;
		const uint32_t* x() const;
		const uint32_t* y() const;
		const struct Colour& colour() const;

	private:
		Polygon& operator=(const Polygon&);

		size_t _cap;
		size_t _num;
		uint32_t* _x;
		uint32_t* _y;
		struct Colour _colour;
};

#endif
