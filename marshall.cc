#include <fstream>

#include <config.h>
#include <history.h>

#define TIXML_USE_STL
#include <tinyxml.h>

TiXmlElement* saveColour(struct Colour c) {
	TiXmlElement* e = new TiXmlElement("Colour");
	e->SetAttribute("R", c.r);
	e->SetAttribute("G", c.g);
	e->SetAttribute("B", c.b);
	e->SetAttribute("A", c.a);
	return e;
}

TiXmlElement* saveVertex(const Polygon& p, uint32_t vidx) {
	TiXmlElement* e = new TiXmlElement("Vertex");
	e->SetAttribute("Order", vidx);
	e->SetAttribute("X", p.x()[vidx]);
	e->SetAttribute("Y", p.y()[vidx]);
	return e;
}

TiXmlElement* savePolygon(const Polygon& p, uint32_t pidx) {
	TiXmlElement* e = new TiXmlElement("Polygon");
	e->SetAttribute("Order", pidx);
	e->SetAttribute("NumVertices", p.num());
	e->LinkEndChild(saveColour(p.colour()));
	for (uint32_t idx = 0; idx < p.num(); idx++) {
		e->LinkEndChild(saveVertex(p, idx));
	}
	return e;
}

TiXmlElement* saveDNA(const DNA& d) {
	TiXmlElement* e = new TiXmlElement("DNA");
	e->SetAttribute("NumPolygons", d.num());
	{
		std::ostringstream s;
		s << d.score();
		e->SetAttribute("Score", s.str());
	}
	for (uint32_t idx = 0; idx < d.num(); idx++) {
		e->LinkEndChild(savePolygon(d[idx], idx));
	}
	return e;
}

TiXmlElement* saveEntry(const History& h, uint32_t eidx) {
	TiXmlElement* e = new TiXmlElement("Entry");
	e->SetAttribute("Order", eidx);
	e->LinkEndChild(saveDNA(h[eidx]));
	return e;
}

TiXmlElement* saveHistory(const History& h) {
	TiXmlElement* e = new TiXmlElement("History");
	e->SetAttribute("NumEntries", h.num());
	for (uint32_t idx = 0; idx < h.num(); idx++) {
		e->LinkEndChild(saveEntry(h, idx));
	}
	return e;
}

TiXmlElement* saveConfig(const Config& c) {
	TiXmlElement* e = new TiXmlElement("Config");
	e->SetAttribute("WhiteBackground", c.whiteBG() ? 1 : 0);
	e->SetAttribute("Width", c.width());
	e->SetAttribute("Height", c.height());
	e->SetAttribute("MaxPolygons", c.maxPolygons());
	e->SetAttribute("MaxPolySize", c.maxPolySize());
	e->SetAttribute("DeltaCoord", c.deltaCoord());
	e->SetAttribute("MaxDegree", c.maxDegree());
	e->SetAttribute("MaxAlpha", c.maxAlpha());
	e->SetAttribute("MinAlpha", c.minAlpha());
	e->SetAttribute("DeltaColour", c.deltaColour());
	return e;
}

void saveState(const char* f, const History& h, const Config& c) {
	TiXmlDocument d;
	TiXmlElement* r = new TiXmlElement("GeneticState");
	d.LinkEndChild(r);
	r->LinkEndChild(saveHistory(h));
	r->LinkEndChild(saveConfig(c));
	d.SaveFile(f);
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
	return 0;
}

void loadEntry(const TiXmlElement* e, std::vector<DNA>& entries) {
	int order;
	if (TIXML_SUCCESS != e->QueryIntAttribute("Order", &order) || order < 0 || order >= (int)entries.size()) {
		return;
	}

	const TiXmlElement* child;
	for (child = e->FirstChildElement(); child; child = child->NextSiblingElement()) {
		if (child->ValueStr() == "DNA") {
			DNA d;
			if (!loadDNA(child, d)) {
				entries[order] = d;
			}
		}
	}
}

void loadHistory(const TiXmlElement* e, History& h) {
	std::vector<DNA> entries;
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
		h.update(entries[idx]);
	}
}

void loadConfig(const TiXmlElement* e, Config& c) {
	int t;
	if (TIXML_SUCCESS == e->QueryIntAttribute("WhiteBackground", &t)) {
		c.setWhiteBG(t != 0);
	}
	if (TIXML_SUCCESS == e->QueryIntAttribute("Width", &t) && t > 0) {
		c.setWidth(t);
	}
	if (TIXML_SUCCESS == e->QueryIntAttribute("Height", &t) && t > 0) {
		c.setHeight(t);
	}
	if (TIXML_SUCCESS == e->QueryIntAttribute("MaxPolygons", &t) && t > 0) {
		c.setMaxPolygons(t);
	}
	if (TIXML_SUCCESS == e->QueryIntAttribute("MaxPolySize", &t) && t > 0) {
		c.setMaxPolySize(t);
	}
	if (TIXML_SUCCESS == e->QueryIntAttribute("DeltaCoord", &t) && t > 0) {
		c.setDeltaCoord(t);
	}
	if (TIXML_SUCCESS == e->QueryIntAttribute("MaxDegree", &t) && t > 0) {
		c.setMaxDegree(t);
	}
	if (TIXML_SUCCESS == e->QueryIntAttribute("MaxAlpha", &t) && t >= 0 && t <= 255) {
		c.setMaxAlpha(t);
	}
	if (TIXML_SUCCESS == e->QueryIntAttribute("MinAlpha", &t) && t >= 0 && t <= 255) {
		c.setMinAlpha(t);
	}
	if (TIXML_SUCCESS == e->QueryIntAttribute("DeltaColour", &t) && t >= 0 && t <= 255) {
		c.setDeltaColour(t);
	}
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
}
