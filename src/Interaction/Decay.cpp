#include "Decay.h"

#include <iostream>
#include <cmath>

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

unique_ptr<Track> Decay::decay(const Track& cosmic, Position3D pos, const BGen& bg)
{
	static const double g = 2.;
	static const double e = 1.602e-19;
	static const double mmu = 1.86e-28;
	static const double gyr = g*e/(2*mmu);
	
	double t = generateDecayTime(cosmic.f);
	
	if (cosmic.f >= Track::Flavour::eP)
	{
		return unique_ptr<Track>(nullptr);
	}
	
	Direction dir; 
	
	if (cosmic.s == Track::Spin::n)
	{
		dir = generateUniformDir();
	}
	else
	{
		B b = bg(pos);
		
		//clog << b.x << endl;
	
		//clog << pos.x << " " <<pos.y << " " << pos.z << endl;
	
		double rotationAngle = fmod(-norm(b)*gyr*t*1e-9, 2*M_PI);
		rotationAngle *= (cosmic.f == Track::Flavour::muP)?1:-1; 
		//clog << norm(b)*gyr*1e-9 << endl;
		//-sqrt(b.x*b.x + b.y*b.y)*t*1e7*g*muB/2;
		
		
		//cout << fmod(cosmic.dir.theta+rotationAngle, 2*M_PI) << " ";
		
		Direction spinDir;
		if (cosmic.s == Track::Spin::f)
		{
			spinDir.phi = cosmic.dir.phi;
			spinDir.theta = cosmic.dir.theta;
		}
		else
		{
			spinDir.phi = fmod(cosmic.dir.phi + M_PI, 2*M_PI);
			spinDir.theta = M_PI - cosmic.dir.theta;
		}
		
		//cout << spinDir.theta << " ";
	
		spinDir = rotate(spinDir, versor(b), rotationAngle);
	
		dir = generateElecDir();		// generate electron dir as if spin was on vertical
	
		//cout << dir.theta << " ";
		//cout << dir.phi << " ";

		dir = changeAxis(dir, reflect(spinDir));		// adjust it to spin direction
		
		//cout << acos(sin(dir.theta)*cos(dir.phi)*sin(spinDir.theta)*cos(spinDir.phi) + sin(dir.theta)*sin(dir.phi)*sin(spinDir.theta)*sin(spinDir.phi) + cos(dir.theta)*cos(spinDir.theta)) << " ";
		
		//cout << rotationAngle << " ";
		//cout << spinDir.theta << " ";
		//cout << dir.theta << " ";
		//cout << endl;
	}
	
	auto elec = unique_ptr<Track>(new Track(pos, dir, Track::Spin::f, Track::Flavour(int(cosmic.f)+2), 0, t));
	
	//clog << "Decay2: " << pos.x << " " << pos.y << " " << pos.z << endl;
	
	return elec;
}

Time Decay::generateDecayTime(Track::Flavour f)
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

Direction Decay::generateUniformDir()
{
	return {generate_canonical<double, 16>(gen())*2*M_PI, acos(1 - 2 * generate_canonical<double, 16>(gen()))};
}
