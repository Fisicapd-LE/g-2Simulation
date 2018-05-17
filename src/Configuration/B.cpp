#include "B.h"

#include <string>
#include <fstream>
#include <iostream>

#include <TH3.h>
#include <TFile.h>
#include <TH2.h>

using namespace std;

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

B BGen::operator()(Position3D p) const
{
	if (zero)
	{
		return {0, 0, 0};
	}
	
	if (simple)
		return {0.0055, 0, 0};
		
	Position3D simm;
	simm.x = fabs(p.x);
	simm.y = fabs(p.y);
	simm.z = fabs(p.z + 52.5);
	
	B value;
	value.x = normalization*bX->Interpolate(simm.x, simm.y, simm.z);
	value.y = normalization*bY->Interpolate(simm.x, simm.y, simm.z)*sgn(p.y);
	value.z = normalization*bZ->Interpolate(simm.x, simm.y, simm.z)*sgn(p.z);
	
	return value;
}

BGen::BGen(const string& filename, double delta)
	:delta(delta), simple(filename == "#simple"), zero(filename == "#zero")
{
	
	if (simple or zero)
	{
		return; // ?
	}	

	fstream fs;
	fs.open(filename.c_str(), fstream::in);
	if (!fs)
	{
		cerr << "Unable to open file!\n";
		throw 1;
	}
	
	if ( fs.is_open() ) cout << "DAT File opened successfully" << endl;

	int xsteps, ysteps, zsteps;

	fs >> xsteps;
	fs >> ysteps;
	fs >> zsteps;
	
	cout << xsteps << " " << ysteps << " " << zsteps << endl;
	
	if (xsteps*ysteps*zsteps > 100000000)
	{
		cerr << "Grid too big!\n";
		throw 1;
	}
		
	bX.reset(new TH3D("hx", "MagneticField x", 
		xsteps + 2, -delta, (xsteps + 1)*delta, 
		ysteps + 2, -delta, (ysteps + 1)*delta, 
		zsteps + 2, -delta, (zsteps + 1)*delta
	));
	bY.reset(new TH3D("hy", "MagneticField y", 
		xsteps + 2, -delta, (xsteps + 1)*delta, 
		ysteps + 2, -delta, (ysteps + 1)*delta, 
		zsteps + 2, -delta, (zsteps + 1)*delta
	));
	bZ.reset(new TH3D("hz", "MagneticField z", 
		xsteps + 2, -delta, (xsteps + 1)*delta, 
		ysteps + 2, -delta, (ysteps + 1)*delta, 
		zsteps + 2, -delta, (zsteps + 1)*delta
	));
		
	double tempX, tempY, tempZ;
	Position3D pos;
	for (int i = 0; i < xsteps; i++)
	{
		for (int j = 0; j < ysteps; j++)
		{
			for (int k = 0; k < zsteps; k++)
			{
				fs >> tempX;
				fs >> tempY;
				fs >> tempZ;
				//clog << tempX << " " << tempY << " " << tempZ << endl;
				gridToPosition(&pos, i, j, k);
				bX->Fill(pos.x, pos.y, pos.z, tempX);
				bY->Fill(pos.x, pos.y, pos.z, tempY);
				bZ->Fill(pos.x, pos.y, pos.z, tempZ);
				if (i == 0)
				{
					pos.x = -pos.x;
					bX->Fill(pos.x, pos.y, pos.z, tempX);
					bY->Fill(pos.x, pos.y, pos.z, tempY);
					bZ->Fill(pos.x, pos.y, pos.z, tempZ);
					pos.x = -pos.x;
				}
				if (j == 0)
				{
					pos.y = -pos.y;
					bX->Fill(pos.x, pos.y, pos.z, tempX);
					bY->Fill(pos.x, pos.y, pos.z, tempY);
					bZ->Fill(pos.x, pos.y, pos.z, tempZ);
					pos.y = -pos.y;
				}
				if (k == 0)
				{
					pos.z = -pos.z;
					bX->Fill(pos.x, pos.y, pos.z, tempX);
					bY->Fill(pos.x, pos.y, pos.z, tempY);
					bZ->Fill(pos.x, pos.y, pos.z, tempZ);
					pos.z = -pos.z;
				}
				if (i == 0 and j == 0 and k == 0)
				{
					pos.x = -pos.x;
					pos.y = -pos.y;
					pos.z = -pos.z;
					bX->Fill(pos.x, pos.y, pos.z, tempX);
					bY->Fill(pos.x, pos.y, pos.z, tempY);
					bZ->Fill(pos.x, pos.y, pos.z, tempZ);
					pos.x = -pos.x;
					pos.y = -pos.y;
					pos.z = -pos.z;
				}
				if (i == 0 and j == 0)
				{
					pos.x = -pos.x;
					pos.y = -pos.y;
					bX->Fill(pos.x, pos.y, pos.z, tempX);
					bY->Fill(pos.x, pos.y, pos.z, tempY);
					bZ->Fill(pos.x, pos.y, pos.z, tempZ);
					pos.x = -pos.x;
					pos.y = -pos.y;
				}
				if (i == 0 and k == 0)
				{
					pos.x = -pos.x;
					pos.z = -pos.z;
					bX->Fill(pos.x, pos.y, pos.z, tempX);
					bY->Fill(pos.x, pos.y, pos.z, tempY);
					bZ->Fill(pos.x, pos.y, pos.z, tempZ);
					pos.x = -pos.x;
					pos.z = -pos.z;
				}
				if (j == 0 and k == 0)
				{
					pos.y = -pos.y;
					pos.z = -pos.z;
					bX->Fill(pos.x, pos.y, pos.z, tempX);
					bY->Fill(pos.x, pos.y, pos.z, tempY);
					bZ->Fill(pos.x, pos.y, pos.z, tempZ);
					pos.y = -pos.y;
					pos.z = -pos.z;
				}
			}
		}
	}
	
	normalization = 0.0055/bX->GetBinContent(bX->GetBin(delta, delta, delta));
		
	
	/*TDirectory* currentDir = gDirectory;
	auto f = TFile::Open("B.root", "RECREATE");
	
	bY->Write();
	
	delete f;
	currentDir->cd();*/	 
}

BGen::~BGen()
{
}


