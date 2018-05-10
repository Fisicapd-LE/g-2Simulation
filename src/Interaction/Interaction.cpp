#include "Interaction.h"

#include "Decay.h"

#include "Objects/ActiveObject.h"

#include "Tracks/Track.h"

#include "Configuration/B.h"

#include <iostream>

#include "Objects/Absorber.h"


using namespace std;

Interaction::Interaction(unique_ptr<Track>&& track, const std::vector<std::unique_ptr<ActiveObject>>& objects, const BGen* bg)
	:track(std::move(track)), objects(&objects), ordered{0}, ran(false), bg(bg)
{
}

void Interaction::setIntersectionPoints(int objectID, Intersections points)
{
	interactions.at(objectID).points = points;
}

Option<Intersections> Interaction::getIntersectionPoints(int objectID) const
{
	if (!objects->at(objectID))
		return No<Intersections>();

	return interactions.at(objectID).points;
}

void Interaction::setCharge(int objectID, double charge)
{
	interactions.at(objectID).charge = charge;
}

double Interaction::getCharge(int objectID) const
{	
	if (objectID >= (int)objects->size())
		return 0;
		
	//clog << int(track->f) << endl;
	
	return interactions.at(objectID).charge;
}

void insertAndShift(array<ActiveObject*, nMaxObjects>& ordered, ActiveObject* element, int pos)
{
	if (pos < 0)
		exit(1);
	for (int i = nMaxObjects - 1; i > pos; i--)
	{
		if (ordered[i-1] == 0)
			continue;
		
		ordered[i] = ordered[i-1];
	}
	ordered[pos] = element;
}

void Interaction::runInteraction()
{
	for (const auto& obj: *objects)
	{
		if (!obj)
			continue;
			
		Option<Intersections> points = obj->computeIntersectionPoints(*track);
		if (!points)
			continue;
		
		setIntersectionPoints(obj->ID, *points);
		for (int i = 0; i < nMaxObjects; i++)
		{
			if (ordered[i] == 0)
			{
				ordered[i] = obj.get();
				break;
			}
			
			auto otherPoints = *getIntersectionPoints(ordered[i]->ID);
			if (points->enter > otherPoints.enter)
				continue;
			
			insertAndShift(ordered, obj.get(), i);
			break;
		}
	}
	
	for (auto obj: ordered)
	{
		if (!obj)
			continue;
			
		// we are inside ordered, intersections always exist
		auto points = *getIntersectionPoints(obj->ID);
		if (points.exit < track->getStart())
			continue;
			
		double range = obj->getRange(track->energy, track->f, track->magic);
		
		
		double enter = max(points.enter, track->getStart());
		double exit = points.exit;
		
		if (enter + range < points.exit)
		{
			track->setEnd(enter + range);
			exit = track->getEnd();
			
			setCharge(obj->ID, obj->getCharge(enter, exit));
			
			// objs are ordered by track trajectory, break is fine
			break;
		}
		
		track->loseEnergy(obj->getEnergyLoss(track->energy, track->f, points.exit - points.enter, track->magic));
		
		setCharge(obj->ID, obj->getCharge(enter, exit));
	}
	
	ran = true;
}

std::unique_ptr<Track> Interaction::getDecay()
{
	if (track->f >= Track::Flavour::eP)
	{
		return unique_ptr<Track>(nullptr);
	}
		
	if (!ran)
		runInteraction(); 
		
	if (not (track->getEnd() < numeric_limits<double>::max()))
	{
		return unique_ptr<Track>(nullptr);
	}
	
	return Decay::decay(*track, track->getPosition(track->getEnd()), *bg);
}

double Interaction::getTime () const
{
	return track->t;
}

Interaction::~Interaction()
{
}
