#include "Decay.h"

using namespace std;

Track::Direction changeAxis(Track::Direction p, Track::Direction a)
{
    double phi1 = fmod(p.phi + 5 * M_PI_2 - a.phi, 2 * M_PI);
    double theta1 = p.theta;

    double ss =
        sin(phi1) * sin(theta1) * cos(a.theta) - cos(theta1) * sin(a.theta);
    double c =
        sin(phi1) * sin(theta1) * sin(a.theta) + cos(theta1) * cos(a.theta);
    double phi2 = (phi1 < M_PI_2 or phi1 > 3 * M_PI_2) ?
                      asin(ss / sqrt(1 - c * c)) :
                      M_PI - asin(ss / sqrt(1 - c * c));
    double theta2 = acos(c);

    Track::Direction pf;
    pf.phi = fmod(phi2 + 3 * M_PI_2 + a.phi, 2 * M_PI);
    pf.theta = theta2;

    return pf;
}

Track::Direction reflect(Track::Direction p)
{
    Track::Direction r;
    r.theta = p.theta;
    r.phi = fmod(p.phi + M_PI, 2 * M_PI);

    return r;
}

Track::Direction rotate(Track::Direction p, Track::Direction a, double ang)
{
	using Dir = Track::Direction;
	
	Dir pNewAx = changeAxis(p, a);
	
	p.phi += ang;
	
	return changeAxis(p, reflect(a));
}

unique_ptr<Track> Decay::decay(unique_ptr<Track>&& cosmic, Position3D pos)
{
	double g = 2.;
	double muB = 1.;
	std::exponential_distribution<double> slow(1./slowDec);
	std::exponential_distribution<double> fast(1./fastDec);
	
	double t = slow(gen());
	if (cosmic->f == Track::Flavour::muN)
	{
		double tf = fast(gen());
		if (tf < t)
			t = tf;
	}
	
	B b = bg(pos);
	
	Track::Direction normal = {atan2(b.y,b.x), M_PI_2};
	
	double rotationAngle = -b.mod()*g*muB/2;
	if (cosmic->s == Track::Spin::d)
		rotationAngle = -rotationAngle;
	
	Track::Direction dir = rotate(Track::Direction{0, (cosmic->s == Track::Spin::u)?0:M_PI}, normal, rotationAngle);
	
	auto elec = unique_ptr<Track>(new Track(pos, dir, Track::Spin::u, Track::Flavour(int(cosmic->f)+2), t));
	
	return move(elec);
}
