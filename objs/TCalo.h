// Author: Remco de Boer
// Date: May 23rd, 2018
// Inspired by Corryvreckan

/* === CLASS DESCRIPTION ======
	This object contains all relevant information of a calorimeter. (At this stage, that is only energy.)
*/

#ifndef TCALODATA_H
#define TCALODATA_H 1

// === INCLUDE =======
	#include "TLinearFitter.h"
	#include "TBeamObject.h"
	#include "TTimepix.h"

// === CLASS DECLARATION =======
class TCalo : public TBeamObject {
  
public:
	// Constructors and destructors
	TCalo() {}
	TCalo(const char* name, ULong64_t ts, Double_t energy)
		: fName(name), fTimestamp(ts), fEnergy(energy) {}
	~TCalo() {}
	// Getters
	TString& GetName();
	Double_t GetEnergy() const;
	ULong64_t GetTimestamp() const;
	// Modifiers
	void SetName(TString&);
	void SetName(const char*);
	void SetTimestamp(ULong64_t);
	void SetEnergy(Double_t);
	// Information
	void Print();

private:
	TString fName;
	ULong64_t fTimestamp;
	Double_t fEnergy;

};

// === TYPE DEFINITIONS =======
	typedef std::list<TCalo*> TCaloList_t;
	typedef std::list<TCalo*>::iterator TCaloIter_t;

#endif // TRACK_H
