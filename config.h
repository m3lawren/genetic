#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>

class Config {
	public:
		Config();
		~Config();

		bool    whiteBG() const;
		int32_t width() const;
		int32_t height() const;
		int32_t maxPolySize() const;

		void setWhiteBG(bool);
		void setWidth(int32_t);
		void setHeight(int32_t);
		void setMaxPolySize(int32_t);

	private:
		Config(const Config&);
		Config& operator=(const Config&);

		bool _whiteBG;
		int32_t _w;
		int32_t _h;
		int32_t _mpsz;
};

#endif
