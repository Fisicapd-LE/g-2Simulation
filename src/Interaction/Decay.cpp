#include "Decay.h"

#include <iostream>

using namespace std;

Direction changeAxis(const Direction& point, const Direction& axis)
{
    double phi1 = fmod(point.phi + 5 * M_PI_2 - axis.phi, 2 * M_PI);
    double theta1 = point.theta;

    double ss =
        sin(phi1) * sin(theta1) * cos(axis.theta) - cos(theta1) * sin(axis.theta);
    double c =
        sin(phi1) * sin(theta1) * sin(axis.theta) + cos(theta1) * cos(axis.theta);
    double phi2 = (phi1 < M_PI_2 or phi1 > 3 * M_PI_2) ?
                      asin(ss / sqrt(1 - c * c)) :
                      M_PI - asin(ss / sqrt(1 - c * c));
    double theta2 = acos(c);

    Direction pf;
    pf.phi = fmod(phi2 + 3 * M_PI_2 + axis.phi, 2 * M_PI);
    pf.theta = theta2;

    return pf;
}

Direction reflect(const Direction& p)
{
    Direction r;
    r.theta = p.theta;
    r.phi = fmod(p.phi + M_PI, 2 * M_PI);

    return r;
}

Direction rotate(const Direction& point, const Direction& axis, double ang)
{
	
	Direction pNewAx = changeAxis(point, axis);
	
	pNewAx.phi += ang;
	
	return changeAxis(pNewAx, reflect(axis));
}

unique_ptr<Track> Decay::decay(const Track& cosmic, Position3D pos)
{
	static const double g = 2.;
	static const double muB = 1.;
	
	if(cosmic.f >= Track::Flavour::eP)
	{
		return unique_ptr<Track>(nullptr);
	}
	
	double t = generateDecayTime(cosmic.f);
	
	B b = bg()(pos);
	
	Direction normal = {atan2(b.y,b.x), M_PI_2};
	
	double rotationAngle = -sqrt(b.x*b.x + b.y*b.y)*t*1e-3*g*muB/2;
	if (cosmic.s == Track::Spin::d)
		rotationAngle = -rotationAngle;
	//clog << rotationAngle << endl;
	
	Direction spinDir = rotate(Direction{0, (cosmic.s == Track::Spin::u)?0:M_PI}, normal, rotationAngle);
	
	Direction dir = generateElecDir();		// generate electron dir as if spin was on vertical
	
	dir = changeAxis(dir, reflect(spinDir));		// adjust it to spin direction
	
	auto elec = unique_ptr<Track>(new Track(pos, dir, Track::Spin::u, Track::Flavour(int(cosmic.f)+2), 0, t));
	
	//clog << "Decay2: " << elec->t << endl;
	
	return elec;
}

double Decay::generateDecayTime(Track::Flavour f)
{
	std::exponential_distribution<double> slow(1./slowDec);
	std::exponential_distribution<double> fast(1./fastDec);
	
	double t = slow(gen());
	if (f == Track::Flavour::muN)
	{
		double tf = fast(gen());
		if (tf < t)
			t = tf;
	}
	
	//clog << "Decay: " << int(t) << endl;;
	
	return t;
}

Direction Decay::generateElecDir()
{
	std::uniform_real_distribution<double> dis(0, 2*M_PI);	// genera phi, uniforme

	Direction dir;
	dir.phi = dis(gen());
	double cosNum;
	double uniNum;
	do
	{
		cosNum = dis(gen())/2;
		uniNum = generate_canonical<double, 16>(gen());
	}while(uniNum > (1 + a*cos(cosNum))/(1 + a));	// genera theta, con algoritmo hit or miss
	
	dir.theta = cosNum;

	return dir;
}
