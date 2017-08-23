#include "Interaction.h"

#include "Decay.h"

#include "Objects/ActiveObject.h"

using namespace std;

void Interaction::addIntersectionPoint(int objectID, Intersections points) const
{
	interactions.at(objectID).points = points;
}

Intersections Interaction::getIntersectionPoint(int objectID) const
{
	if (!ran)
		runInteraction();
		
	if (interactions.at(objectID).points->exit and (track->getEnd() < *interactions.at(objectID).points->exit))
		interactions.at(objectID).points->exit = No<double>();
	
	return *interactions.at(objectID).points;
}

void Interaction::addCharge(int objectID, double charge) const
{
	interactions.at(objectID).charge = charge;
}

double Interaction::getCharge(int objectID) const
{
	if (!ran)
		runInteraction();
		
	if(!objects.at(objectID)->isInside(*track))
		interactions.at(objectID).charge = 0;
	
	return *interactions.at(objectID).charge;
}

void Interaction::runInteraction() const
{
	for (const auto& obj: objects)
	{
		obj->interact(*track, *this);
	}
	ran = true;
}

std::unique_ptr<Track> Interaction::getDecay() const
{
	if (track->f >= Track::Flavour::eP)
		return unique_ptr<Track>(nullptr);
		
	if (!ran)
		runInteraction(); 
		
	if (not (track->getEnd() < numeric_limits<double>::max()))
		return unique_ptr<Track>(nullptr);
	
	return Decay::decay(*track, track->getPosition(track->getEnd()));
}
