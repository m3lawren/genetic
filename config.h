#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>

class Config {
	public:
		Config();
		~Config();

		bool     whiteBG() const;
		uint32_t width() const;
		uint32_t height() const;

		uint32_t maxPolygons() const;
		uint32_t maxPolySize() const;
		uint32_t deltaCoord() const;
		uint32_t maxDegree() const;

		uint8_t  maxAlpha() const;
		uint8_t  minAlpha() const;
		uint8_t  deltaColour() const;

		void setWhiteBG(bool);
		void setWidth(uint32_t);
		void setHeight(uint32_t);

		void setMaxPolygons(uint32_t);
		void setMaxPolySize(uint32_t);
		void setDeltaCoord(uint32_t);
		void setMaxDegree(uint32_t);

		void setMaxAlpha(uint8_t);
		void setMinAlpha(uint8_t);
		void setDeltaColour(uint8_t);

	private:
		Config(const Config&);
		Config& operator=(const Config&);

		bool _whiteBG;
		uint32_t _w;
		uint32_t _h;

		uint32_t _mp;
		uint32_t _mpsz;
		uint32_t _dcr;
		uint32_t _md;

		uint8_t _mxa;
		uint8_t _mna;
		uint8_t _dcl;
};

#endif
