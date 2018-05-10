#ifndef INTERACTION_H_
#define INTERACTION_H_

#include "Option/Option.h"

#include "Utilities/channels.h"

#include <array>
#include <memory>
#include <vector>

class Track;
class ActiveObject;
class BGen;

struct Intersections
{
	double enter;
	double exit;
};

struct ObjectInteraction
{
	ObjectInteraction()
		:points{}, 
		charge{}
	{};
	Option<Intersections> points;
	double charge;
};

class Interaction
{
	private:
	
		std::array<ObjectInteraction, nMaxObjects> interactions;
		
		std::unique_ptr<Track> track;
		
		const std::vector<std::unique_ptr<ActiveObject>>* objects;
		std::array<ActiveObject*, nMaxObjects> ordered;
		
		mutable bool ran;
		
		const BGen* bg;
	
	public:
		Interaction(std::unique_ptr<Track>&& track, 
				const std::vector<std::unique_ptr<ActiveObject>>& objects, const BGen* bg);
		Interaction(const Interaction&) = delete;
		Interaction& operator=(const Interaction&) = delete;
		Interaction(Interaction&&) = default;
		Interaction& operator=(Interaction&&) = default;
		~Interaction();
		
		std::unique_ptr<Track> getDecay();
		
		bool completed() const
		{
			return ran;
		};
		
		bool intersectionsComputed(int objectID) const
		{
			return static_cast<bool>(interactions.at(objectID).points);
		};
		bool chargeComputed(int objectID) const
		{
			return static_cast<bool>(interactions.at(objectID).charge);
		};
		
		void runInteraction();
		
		void setIntersectionPoints(int objectID, Intersections points);
		Option<Intersections> getIntersectionPoints(int objectID) const;
		void setCharge(int objectID, double charge);
		double getCharge(int objectID) const;
		
		double getTime () const;

};
	

#endif
