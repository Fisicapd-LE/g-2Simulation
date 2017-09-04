#include "Interaction.h"

#include "Decay.h"

#include "Objects/ActiveObject.h"

#include "Tracks/Track.h"

#include <iostream>

using namespace std;

Interaction::Interaction(unique_ptr<Track>&& track, const std::vector<std::unique_ptr<ActiveObject>>& objects)
	:track(std::move(track)), objects(&objects), ran(false)
{
}

void Interaction::addIntersectionPoint(int objectID, Intersections points) const
{
	interactions.at(objectID).points = points;
}

Intersections Interaction::getIntersectionPoint(int objectID, bool checked) const
{
	if (!objects->at(objectID))
		return Intersections();

	if (!ran)
	{
		if (checked)
			runInteraction();
		else
			return *interactions.at(objectID).points;
	}
		
	if (interactions.at(objectID).points->enter and track->getEnd() < *interactions.at(objectID).points->enter)
	{
		interactions.at(objectID).points->exit = No<double>();
		interactions.at(objectID).points->enter = No<double>();
	}
	if (interactions.at(objectID).points->exit and (track->getEnd() < *interactions.at(objectID).points->exit))
		interactions.at(objectID).points->exit = No<double>();
	
	return *interactions.at(objectID).points;
}

void Interaction::addCharge(int objectID, double charge) const
{
	interactions.at(objectID).charge = charge;
}

double Interaction::getCharge(int objectID, bool checked) const
{	
	if (objectID >= objects->size())
		return 0;
			
	if (!ran)
	{
		if (checked)
			runInteraction();
		else
			return *interactions.at(objectID).charge;
	}
		
	if(!objects->at(objectID)->isInside(*track))
		interactions.at(objectID).charge = 0;
	
	return *interactions.at(objectID).charge;
}

void Interaction::runInteraction() const
{
	for (const auto& obj: *objects)
	{
		if (obj)
		{
			obj->interact(*track, *this);
		}
	}
	ran = true;
}

std::unique_ptr<Track> Interaction::getDecay() const
{
	if (track->f >= Track::Flavour::eP)
	{
		return unique_ptr<Track>(nullptr);
	}
		
	if (!ran)
		runInteraction(); 
		
	if (not (track->getEnd() < numeric_limits<double>::max()))
	{
		//clog << "track didn't stop: " << track->getEnd() << endl;
		return unique_ptr<Track>(nullptr);
	}
	
	return Decay::decay(*track, track->getPosition(track->getEnd()));
}

double Interaction::getTime () const
{
	return track->t;
}

Interaction::~Interaction()
{
}
