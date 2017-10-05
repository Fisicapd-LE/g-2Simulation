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
	Option<double> enter;
	Option<double> exit;
};

struct ObjectInteraction
{
	ObjectInteraction()
		:points{}, 
		charge{}
	{};
	Option<Intersections> points;
	Option<double> charge;
};

class Interaction
{
	private:
	
		mutable std::array<ObjectInteraction, nMaxObjects> interactions;
		
		std::unique_ptr<Track> track;
		
		const std::vector<std::unique_ptr<ActiveObject>>* objects;
		
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
		
		std::unique_ptr<Track> getDecay() const;
		
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
		
		void runInteraction() const;
		
		void addIntersectionPoint(int objectID, Intersections points) const;
		Intersections getIntersectionPoint(int objectID, bool checked = true) const;
		void addCharge(int objectID, double charge) const;
		double getCharge(int objectID, bool checked = true) const;
		
		double getTime () const;

};
	

#endif
