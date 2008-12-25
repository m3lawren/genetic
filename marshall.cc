#include <fstream>
#include <locale>

#include <config.h>
#include <history.h>

#define TIXML_USE_STL
#include <tinyxml.h>

#define _DUMP(n, v) " " << (n) << "=\"" << (v) << "\""
#define DUMPBOOL(n, v) _DUMP(n, (v) ? 1 : 0)
#define DUMPUINT(n, v) _DUMP(n, (uint64_t)(v))
#define DUMPINT(n, v) _DUMP(n, (int64_t)(v))

void saveColour(std::ostream& o, struct Colour c) {
	o << "<Colour"
	  << DUMPUINT("R", c.r)
	  << DUMPUINT("G", c.g)
	  << DUMPUINT("B", c.b)
	  << DUMPUINT("A", c.a)
	  << " />" << std::endl;
}

void saveVertex(std::ostream& o, const Polygon& p, uint32_t vidx) {
	o << "<Vertex"
	  << DUMPUINT("Order", vidx)
	  << DUMPINT("X", p.x()[vidx])
	  << DUMPINT("Y", p.y()[vidx])
	  << " />" << std::endl;
}

void savePolygon(std::ostream& o, const Polygon& p, uint32_t pidx) {
	o << "<Polygon" 
	  << DUMPUINT("Order", pidx)
	  << DUMPUINT("NumVertices", p.num())
	  << ">" << std::endl;
	saveColour(o, p.colour());
	for (uint32_t vidx = 0; vidx < p.num(); vidx++) {
		saveVertex(o, p, vidx);
	}
	o << "</Polygon>" << std::endl;
}

void saveDNA(std::ostream& o, const DNA& d) {
	o << "<DNA"
	  << DUMPUINT("NumPolygons", d.num())
	  << DUMPUINT("Score", d.score())
	  << ">" << std::endl;
	for (uint32_t pidx = 0; pidx < d.num(); pidx++) {
		savePolygon(o, d[pidx], pidx);
	}
	o << "</DNA>" << std::endl;
}

void saveEntry(std::ostream& o, const History& h, uint32_t eidx) {
	o << "<Entry"
	  << DUMPUINT("Order", eidx)
	  << DUMPUINT("Iteration", h.iter(eidx))
	  << ">" << std::endl;
	saveDNA(o, h.dna(eidx));
	o << "</Entry>" << std::endl;
}

void saveHistory(std::ostream& o, const History& h) {
	o << "<History"
	  << DUMPUINT("NumEntries", h.num())
	  << ">" << std::endl;
	for (uint32_t eidx = 0; eidx < h.num(); eidx++) {
		saveEntry(o, h, eidx);
	}
	o << "</History>" << std::endl;
}

void saveConfig(std::ostream& o, const Config& c) {
	o << "<Config"
	  << DUMPBOOL("WhiteBackground", c.whiteBG())
	  << DUMPUINT("Width", c.width())
	  << DUMPUINT("Height", c.height())
	  << DUMPUINT("MaxPolygons", c.maxPolygons())
	  << DUMPUINT("MaxPolySize", c.maxPolySize())
	  << DUMPUINT("MaxDegree", c.maxDegree())
	  << DUMPUINT("MaxAlpha", c.maxAlpha())
	  << DUMPUINT("MinAlpha", c.minAlpha())
	  << DUMPUINT("MutPolyAddFreq", c.mutPolyAddFreq())
	  << DUMPUINT("MutPolyDelFreq", c.mutPolyDelFreq())
	  << DUMPUINT("MutPolySwapFreq", c.mutPolySwapFreq())
	  << DUMPUINT("MutColourFreq", c.mutColourFreq())
	  << DUMPUINT("MutPointHugeFreq", c.mutPointHugeFreq())
	  << DUMPUINT("MutPointMedFreq", c.mutPointMedFreq())
	  << DUMPUINT("MutPointSmallFreq", c.mutPointSmallFreq())
	  << DUMPUINT("MutPointAddFreq", c.mutPointAddFreq())
	  << DUMPUINT("MutPointDelFreq", c.mutPointDelFreq())
	  << DUMPUINT("MutPointSwapFreq", c.mutPointSwapFreq())
	  << " />" << std::endl;
}

void saveState(const char* f, const History& h, const Config& c) {
	std::ofstream o(f);
	o << "<GeneticState>" << std::endl;
	saveHistory(o, h);
	saveConfig(o, c);
	o << "</GeneticState>" << std::endl;
}

void loadVertex(const TiXmlElement* e, int16_t* x, int16_t* y, int n) {
	int o, tx, ty;
	if (TIXML_SUCCESS != e->QueryIntAttribute("Order", &o) || o < 0 || o >= n) {
		return;
	}
	if (TIXML_SUCCESS != e->QueryIntAttribute("X", &tx)) {
		return;
	}
	if (TIXML_SUCCESS != e->QueryIntAttribute("Y", &ty)) {
		return;
	}
	x[o] = tx;
	y[o] = ty;
}

int loadColour(const TiXmlElement* e, struct Colour& c) {
	int t;
	if (TIXML_SUCCESS != e->QueryIntAttribute("R", &t) || t > 255 || t < 0) {
		return 1;
	}
	c.r = t;
	if (TIXML_SUCCESS != e->QueryIntAttribute("G", &t) || t > 255 || t < 0) {
		return 1;
	}
	c.g = t;
	if (TIXML_SUCCESS != e->QueryIntAttribute("B", &t) || t > 255 || t < 0) {
		return 1;
	}
	c.b = t;
	if (TIXML_SUCCESS != e->QueryIntAttribute("A", &t) || t > 255 || t < 0) {
		return 1;
	}
	c.a = t;
	return 0;
}

void loadPolygon(const TiXmlElement* e, std::vector<Polygon>& polys) {
	int idx;
	int n;
	int16_t* x;
	int16_t* y;
	struct Colour c;
	c.r = c.g = c.b = c.a = 0;

	if (TIXML_SUCCESS != e->QueryIntAttribute("Order", &idx) || idx < 0 || idx >= (int)polys.size()) {
		return;
	}
	if (TIXML_SUCCESS == e->QueryIntAttribute("NumVertices", &n) && n >= 3) {
		x = new int16_t[n];
		y = new int16_t[n];
	} else {
		return;
	}
	
	const TiXmlElement* child;
	for (child = e->FirstChildElement(); child; child = child->NextSiblingElement()) {
		if (child->ValueStr() == "Colour") {
			struct Colour c1;
			if (!loadColour(child, c1)) {
				c = c1;
			}
		} else if (child->ValueStr() == "Vertex") {
			loadVertex(child, x, y, n);
		}
	}

	polys[idx] = Polygon(n, x, y, c);
	delete[] x;
	delete[] y;
}

int loadDNA(const TiXmlElement* e, DNA& d) {
	int t;
	uint64_t sc;
	std::vector<Polygon> polys;
	if (TIXML_SUCCESS == e->QueryIntAttribute("NumPolygons", &t) && t >= 0) {
		polys.resize(t);
	} else {
		return 1;
	}

	const char* s = e->Attribute("Score");
	if (s) {
		std::istringstream is(s);
		is >> sc;
		d.setScore(sc);
	}

	const TiXmlElement* child;
	for (child = e->FirstChildElement(); child; child = child->NextSiblingElement()) {
		if (child->ValueStr() == "Polygon") {
			loadPolygon(child, polys);
		}
	}
	d = DNA(polys);
	d.setScore(sc);
	std::cout << "." << std::flush;
	return 0;
}

void loadEntry(const TiXmlElement* e, std::vector<std::pair<DNA, uint64_t> >& entries) {
	int order;
	if (TIXML_SUCCESS != e->QueryIntAttribute("Order", &order) || order < 0 || order >= (int)entries.size()) {
		return;
	}
	const char* siter = e->Attribute("Iteration");
	uint64_t iter;
	if (!siter) {
		return;
	} else {
		std::istringstream ss(siter);
		ss >> iter;
	}

	const TiXmlElement* child;
	for (child = e->FirstChildElement(); child; child = child->NextSiblingElement()) {
		if (child->ValueStr() == "DNA") {
			DNA d;
			if (!loadDNA(child, d)) {
				entries[order] = std::pair<DNA, uint64_t>(d, iter);
			}
		}
	}
}

void loadHistory(const TiXmlElement* e, History& h) {
	std::vector<std::pair<DNA, uint64_t> > entries;
	int t;
	if (TIXML_SUCCESS == e->QueryIntAttribute("NumEntries", &t) && t >= 0) {
		entries.resize(t);
	} else {
		return;
	}
	
	const TiXmlElement* child;
	for (child = e->FirstChildElement(); child; child = child->NextSiblingElement()) {
		if (child->ValueStr() == "Entry") {
			loadEntry(child, entries);
		}
	}

	for (uint32_t idx = 0; idx < entries.size(); idx++) {
		h.update(entries[idx].first, entries[idx].second);
	}
}

#define _LOAD_INT(n, type, cond) { if (TIXML_SUCCESS == e->QueryIntAttribute(#n, &t)) { type _t = t; if (cond) { c.set##n(_t); }}}
#define LOAD_UINT32(n) _LOAD_INT(n, uint32_t, true)
#define LOAD_UINT8(n) _LOAD_INT(n, uint8_t, (int32_t)_t == t);

void loadConfig(const TiXmlElement* e, Config& c) {
	int32_t t;
	if (TIXML_SUCCESS == e->QueryIntAttribute("WhiteBackground", &t)) {
		c.setWhiteBG(t != 0);
	}
	LOAD_UINT32(Width);
	LOAD_UINT32(Height);

	LOAD_UINT32(MaxPolygons);
	LOAD_UINT32(MaxPolySize);
	LOAD_UINT32(MaxDegree);

	LOAD_UINT8(MaxAlpha);
	LOAD_UINT8(MinAlpha);

	LOAD_UINT32(MutPolyAddFreq);
	LOAD_UINT32(MutPolyDelFreq);
	LOAD_UINT32(MutPolySwapFreq);
	LOAD_UINT32(MutColourFreq);
	LOAD_UINT32(MutPointHugeFreq);
	LOAD_UINT32(MutPointMedFreq);
	LOAD_UINT32(MutPointSmallFreq);
	LOAD_UINT32(MutPointAddFreq);
	LOAD_UINT32(MutPointDelFreq);
	LOAD_UINT32(MutPointSwapFreq);
}
	
void loadStateImpl(const TiXmlElement* e, History& h, Config& c) {
	const TiXmlElement* child;
	for (child = e->FirstChildElement(); child; child = child->NextSiblingElement()) {
		if (child->ValueStr() == "Config") {
			loadConfig(child, c);
		} else if (child->ValueStr() == "History") {
			loadHistory(child, h);
		}
	}
}

void loadState(const char* f, History& h, Config& c) {
	TiXmlDocument d;
	if (!d.LoadFile(f)) {
		return;
	}

	TiXmlElement* child;
	for (child = d.FirstChildElement(); child; child = child->NextSiblingElement()) {
		if (child->ValueStr() == "GeneticState") {
			loadStateImpl(child, h, c);
		}
	}
	std::cout << std::endl;
}
