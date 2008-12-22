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

TiXmlElement* saveDNA(const DNA& d, uint32_t didx) {
	TiXmlElement* e = new TiXmlElement("DNA");
	e->SetAttribute("Generation", didx);
	e->SetAttribute("NumPolygons", d.num());
	for (uint32_t idx = 0; idx < d.num(); idx++) {
		e->LinkEndChild(savePolygon(d[idx], idx));
	}
	return e;
}

TiXmlElement* saveHistory(const History& h) {
	TiXmlElement* e = new TiXmlElement("History");
	e->SetAttribute("NumDNAs", h.num());
	for (uint32_t idx = 0; idx < h.num(); idx++) {
		e->LinkEndChild(saveDNA(h[idx], idx));
	}
	return e;
}

TiXmlElement* saveConfig(const Config& c) {
	TiXmlElement* e = new TiXmlElement("Config");
	e->SetAttribute("WhiteBackground", c.whiteBG() ? "True" : "False");
	e->SetAttribute("Width", c.width());
	e->SetAttribute("Height", c.height());
	e->SetAttribute("MaxPolySize", c.maxPolySize());
	return e;
}

void saveState(const History& h, const Config& c) {
	TiXmlDocument d;
	TiXmlElement* r = new TiXmlElement("GeneticState");
	d.LinkEndChild(r);
	r->LinkEndChild(saveHistory(h));
	r->LinkEndChild(saveConfig(c));
	d.SaveFile("state.xml");
}
