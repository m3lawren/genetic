#include <iostream>
#include <locale>

#include <config.h>
#include <history.h>
#include <marshall.h>
#include <mutations.h>

void init_locale(void) {
	std::locale loc("");
	std::locale::global(loc);
	std::cout.imbue(loc);
	std::cerr.imbue(loc);
	std::cin.imbue(loc);
}

int main(void) { 
//	init_locale();

	Config c;
	History h;
	DNA d;

	c.setWidth(100);
	c.setHeight(100);

	d = mutationAddPolygon(d, c);
	h.update(d);

	d = mutationAddPolygon(d, c);
	h.update(d);
	
	d = mutationAddPolygon(d, c);
	h.update(d);

	saveState(h, c);

	return 0;
}
