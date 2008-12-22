#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>

class Config {
	public:
		Config();
		~Config();

		bool     whiteBG() const;
		int32_t  width() const;
		int32_t  height() const;

		uint32_t maxPolygons() const;
		int32_t  maxPolySize() const;
		int32_t  deltaCoord() const;
		uint32_t maxDegree() const;

		uint8_t  maxAlpha() const;
		uint8_t  minAlpha() const;
		uint8_t  deltaColour() const;

		void setWhiteBG(bool);
		void setWidth(int32_t);
		void setHeight(int32_t);

		void setMaxPolygons(uint32_t);
		void setMaxPolySize(int32_t);
		void setDeltaCoord(int32_t);
		void setMaxDegree(uint32_t);

		void setMaxAlpha(uint8_t);
		void setMinAlpha(uint8_t);
		void setDeltaColour(uint8_t);

	private:
		Config(const Config&);
		Config& operator=(const Config&);

		bool _whiteBG;
		int32_t _w;
		int32_t _h;

		uint32_t _mp;
		int32_t _mpsz;
		int32_t _dcr;
		uint32_t _md;

		uint8_t _mxa;
		uint8_t _mna;
		uint8_t _dcl;
};

#endif
