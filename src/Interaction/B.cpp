#include "Decay.h"

B Decay::BGen::operator()(Position3D p) const
{
	return B{1,0,0};
}

