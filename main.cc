#include <iostream>
#include <locale>

void init_locale(void) {
	std::locale loc("");
	std::locale::global(loc);
	std::cout.imbue(loc);
	std::cerr.imbue(loc);
	std::cin.imbue(loc);
}

int main(void) { 
	init_locale();

	return 0;
}
