#include "Decay.h"

using namespace std;

unique_ptr<Track> Decay::decay(unique_ptr<Track>&& cosmic, Position3D pos)
{
	std::exponential_distribution slow(1./slowDec);
	std::exponential_distribution fast(1./fastDec);
	
	double t = slow(gen());
	if (cosmic->f == Track::Flavour::muN)
	{
		double tf = fast(gen());
		if (tf < t)
			t = tf;
	}
	
	B b = bg(pos);
}