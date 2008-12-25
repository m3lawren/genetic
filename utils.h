#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>

namespace Utils {

	int64_t randRange(int64_t, int64_t);

	inline bool doMutate(int64_t freq) {
		return randRange(1, freq) == 1;
	}

	inline int64_t max(int64_t a, int64_t b) {
		if (a > b) return a;
		return b;
	}

	inline int64_t min(int64_t a, int64_t b) {
		if (a < b) return a;
		return b;
	}
}

#endif
