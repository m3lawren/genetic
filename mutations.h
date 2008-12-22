#ifndef _MUTATIONS_H_
#define _MUTATIONS_H_

#include <dna.h>

struct Config;

DNA mutate(const DNA&, const Config&);
DNA mutationAddPolygon(const DNA&, const Config&);

#endif
