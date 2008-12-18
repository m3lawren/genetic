#ifndef _MARSHALL_H_
#define _MARSHALL_H_

#include <string>

class ParseError {
	public:
		ParseError(const std::string& desc, uint32_t loc) : _desc(desc), _loc(loc) {}
		~ParseError() {}

		const std::string& description() const { return _desc; }
		uint32_t position() const { return _loc; }

	private:
		std::string _desc;
		uint32_t    _loc;
};

#endif
