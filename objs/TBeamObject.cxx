// Author: Remco de Boer
// Date: May 20th, 2018
// Inspired by Corryvreckan

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
// === STATIC FUNCTIONS =======
	// Function to get instantiation of inherited class: given a string, give back an object of type 'daughter'
	// TBeamObject* TBeamObject::Factory(string objType, TBeamObject* obj) {
	// 	if(objType == "pixels")  return (obj == NULL) ? new TPixel()   : new TPixel(*(TPixel*)obj);
	// 	if(objType == "timepix") return (obj == NULL) ? new TTimepix() : new TTimepix(*(TTimepix*)obj);
	// }