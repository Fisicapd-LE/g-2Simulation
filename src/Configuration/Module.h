#ifndef MODULE_H_
#define MODULE_H_

#include "Option/Option.h"

class Module
{
	public:
		virtual ~Module();
		using time = long;
		
		virtual void sendTrig(time muonTime, Option<time> elecTime) = 0;
	
};

#endif
