// Author: Remco de Boer
// Date: May 20th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	Generic base class for beam objects. Beam objects describe measurement data in an event, such as output of a timepix. Any class that inherits from TBeamObject can be placed on the clipboard and written out to file.
*/

#ifndef TBEAMOBJECT_H
#define TBEAMOBJECT_H 1

// === INCLUDE =======
	#include <list>
	#include "GlobalParameters.h"
	#include "TObject.h"

// === CLASS DECLARATION =======
class TBeamObject {

public:
	// Constructors and destructors
	TBeamObject() {}
	virtual ~TBeamObject();

};

// === TYPE DEFINITIONS =======
	typedef std::list<TBeamObject*> TBeamObjectList_t;
	typedef std::list<TBeamObject*>::iterator TBeamObjectIter_t;

#endif // TBEAMOBJECT_H