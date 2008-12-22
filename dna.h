#ifndef _DNA_H_
#define _DNA_H_

#include <cstdlib>
#include <vector>

#include <polygon.h>

class DNA {
	public:
		DNA();
		~DNA();

		size_t num() const;
		const Polygon& operator[](size_t) const;

	private:
		DNA(const DNA&);
		DNA& operator=(const DNA&);

		std::vector<Polygon> _polys;
};

#endif
