#include <utils.h>

#include <cassert>
#include <cstdlib>
#include <mtwist.h>

namespace Utils {

	int64_t randRange(int64_t a, int64_t b) {
		assert(a <= b);
		uint64_t v = mt_llrand() % (uint64_t)(b - a + 1);
		return a + (int64_t)v;
	}
}
