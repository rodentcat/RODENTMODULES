#include "RODENTMODULES.hpp"


Plugin *plugin;

void init(rack::Plugin *p) {
	plugin = p;
	p->slug = TOSTRING(SLUG);
	p->version = TOSTRING(VERSION);
	
	p->addModel(modelMessedUpOsc);
}