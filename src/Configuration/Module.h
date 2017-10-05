#ifndef MODULE_H_
#define MODULE_H_

#include "Option/Option.h"

#include "Utilities/Time.h"

class Module
{
	public:
		virtual ~Module();
		
		virtual void sendTrig(Time muonTime, Option<Time> elecTime) = 0;
	
};

#endif
