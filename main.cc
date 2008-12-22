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

std::ostream& operator<<(std::ostream& s, const Config& c) {
	return s << "Config:" << std::endl
	         << "  WhiteBackground: " << c.whiteBG() << std::endl
	         << "  Width:           " << c.width() << std::endl
	         << "  Height:          " << c.height() << std::endl
	         << "  MaxPolySize:     " << c.maxPolySize() << std::endl;
}

int main(void) { 
//	init_locale();

	Config c, c1;
	History h, h1;
	DNA d;

	c.setWhiteBG(true);
	c.setWidth(100);
	c.setHeight(101);
	c.setMaxPolySize(1000);

	d = mutationAddPolygon(d, c);
	d.setScore(1000);
	h.update(d);

	d = mutationAddPolygon(d, c);
	d.setScore(998);
	h.update(d);
	
	d = mutationAddPolygon(d, c);
	d.setScore(0);
	h.update(d);

	saveState("state.xml", h, c);
	loadState("state.xml", h1, c1);
	saveState("state2.xml", h1, c1);

	return 0;
}
