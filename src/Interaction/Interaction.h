#ifndef INTERACTION_H_
#define INTERACTION_H_

#include "Utilities/Option.h"

#include "Configuration/Configuration.h"

#include <array>

struct Intersections
{
	Option<double> enter;
	Option<double> exit;
};

struct ObjectInteraction
{
	ObjectInteraction()
		:points{}, 
		charge(0)
	{};
	Option<Intersections> points;
	Option<double> charge;
};

class Interaction
{
	private:
	
		mutable std::array<ObjectInteraction, Configuration::nMaxObjects> interactions;
		
		std::unique_ptr<Track> track;
		
		const std::array<std::unique_ptr<ActiveObject>, Configuration::nMaxObjects>& objects;
		
		mutable bool ran;
		
		void runInteraction() const;
	
	public:
		Interaction(std::unique_ptr<Track>&& track, 
				const std::array<std::unique_ptr<ActiveObject>, Configuration::nMaxObjects>& objects)
			:track(std::move(track)), objects(objects), ran(false)
		{};
		
		std::unique_ptr<Track> getDecay() const;
		
		bool intersectionsComputed(int objectID) const
		{
			return static_cast<bool>(interactions.at(objectID).points);
		};
		bool chargeComputed() const{
			return static_cast<bool>(interactions.at(objectID).charge);
		};
		
		void addIntersectionPoint(int objectID, Intersections points) const;
		Intersections getIntersectionPoint(int objectID) const;
		void addCharge(int objectID, double charge) const;
		double getCharge(int objectID) const;

};
	

#endif
