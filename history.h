#ifndef _HISTORY_H_
#define _HISTORY_H_

#include <dna.h>

#include <utility>

class History {
	public:
		History();
		~History();

		size_t num() const;
		const DNA& dna(size_t) const;
		uint64_t iter(size_t) const;

		size_t update(const DNA&, uint64_t iter);

	private:
		std::vector<std::pair<DNA, uint64_t> > _g;
};

#endif
