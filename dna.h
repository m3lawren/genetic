#ifndef _DNA_H_
#define _DNA_H_

#include <cstdlib>
#include <vector>

#include <polygon.h>

class DNA {
	public:
		DNA();
		DNA(const std::vector<Polygon>&);
		~DNA();

		size_t num() const;
		const Polygon& operator[](size_t) const;
		const std::vector<Polygon>& polygons() const;
		uint64_t score() const;

		void setScore(uint64_t);

	private:
		std::vector<Polygon> _polys;
		uint64_t _score;
};

#endif
