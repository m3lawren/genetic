#ifndef _HISTORY_H_
#define _HISTORY_H_

#include <dna.h>

class History {
	public:
		History();
		~History();

		size_t num() const;
		const DNA& operator[](size_t) const;

		size_t update(const DNA&);

	private:
		std::vector<DNA> _g;
};

#endif
