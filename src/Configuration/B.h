#ifndef B_H_
#define B_H_

#include "Utilities/Vector3D.h"

#include <memory>
#include <string>

class TH3;
class TH2;

class BGen
{
	public:
		BGen(const std::string& filename, double delta);
		virtual ~BGen();
		
		B operator()(Position3D p) const;
		
	private:
		std::unique_ptr<TH3> bX;
		std::unique_ptr<TH3> bY;
		std::unique_ptr<TH3> bZ;
		
		const double delta;
		
		double normalization;
		
		const bool simple;
		const bool zero;
		
		void gridToPosition(Position3D * position, const unsigned int & xindex, const unsigned int & yindex, const unsigned int & zindex)
		{
			position->x = xindex * delta + (0.5 * delta);
			position->y = yindex * delta + (0.5 * delta);
			position->z = zindex * delta + (0.5 * delta);
			return;
		}
};

#endif
