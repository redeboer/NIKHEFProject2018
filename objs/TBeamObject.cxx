// Author: Remco de Boer
// Date: May 20th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	Generic base class for beam objects. Beam objects describe measurement data in an event, such as output of a timepix. Any class that inherits from TBeamObject can be placed on the clipboard and written out to file.
*/

// === INCLUDE =======
	#include "TBeamObject.h"
	#include "TPixel.h"
	#include "TTimepix.h"
	using namespace std;

// === CONSTRUCTORS =======
	TBeamObject::~TBeamObject() {}