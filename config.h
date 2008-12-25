#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>

class Config {
	public:
		~Config();

		static Config& instance();

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

		uint32_t mutPolyAddFreq() const;
		uint32_t mutPolyDelFreq() const;
		uint32_t mutColourFreq() const;
		uint32_t mutPointHugeFreq() const;
		uint32_t mutPointMedFreq() const;
		uint32_t mutPointSmallFreq() const;
		uint32_t mutPointAddFreq() const;
		uint32_t mutPointDelFreq() const;

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

		void setMutPolyAddFreq(uint32_t);
		void setMutPolyDelFreq(uint32_t);
		void setMutColourFreq(uint32_t);
		void setMutPointHugeFreq(uint32_t);
		void setMutPointMedFreq(uint32_t);
		void setMutPointSmallFreq(uint32_t);
		void setMutPointAddFreq(uint32_t);
		void setMutPointDelFreq(uint32_t);

	private:
		Config();
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

		uint32_t _mplaf;
		uint32_t _mpldf;
		uint32_t _mcf;
		uint32_t _mpthf;
		uint32_t _mptmf;
		uint32_t _mptsf;
		uint32_t _mptaf;
		uint32_t _mptdf;
};

#endif
