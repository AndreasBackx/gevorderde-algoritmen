
#include "Knoop.h"

Knoop::~Knoop() {
	delete volgende;
}

Knoop::Knoop(const Knoop & andereKnoop) {
	sl = andereKnoop.sl;
	
	if (andereKnoop.volgende) {
		volgende = new Knoop(*(andereKnoop.volgende));
	} else {
		volgende = nullptr;
	}
}
