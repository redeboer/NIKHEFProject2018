// Author: Remco de Boer
// Date: May 23rd, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION ======
	This object describes the point where the proton probably scattered, including its energy loss. To be able to retrieve how it was reconstructed, pointers to the tracks that were used to reconstruct the it are also stored.
*/

#ifndef TRECONSTRUCTEDPOINT_H
#define TRECONSTRUCTEDPOINT_H 1

// === INCLUDE =======
	#include "TBeamObject.h"
	#include "TRecPoint.h"
	#include "TTrack.h"
	#include "TVector3.h"

// === CLASS DECLARATION =======
class TRecPoint : public TBeamObject {
  
public:
	// Constructors and destructors
		TRecPoint(Double_t x, Double_t y, Double_t z, Double_t de=0)
			: fRecPoint(x,y,z), fEnergyLoss(de), fTrack1(NULL), fTrack2(NULL) {}
		TRecPoint(TTrack* t1, TTrack* t2, Double_t x, Double_t y, Double_t z, Double_t de=0)
			: fRecPoint(x,y,z), fEnergyLoss(de), fTrack1(t1), fTrack2(t2) {}
		~TRecPoint() {}
	// Getters
		TVector3& GetPoint();
		Double_t GetEnergyLoss() const;
		TTrack* GetTrack1();
		TTrack* GetTrack2();
	// Modifiers
		void SetPoint(Double_t,Double_t,Double_t);
		void SetEnergyLoss(Double_t);
		void SetTrack1(TTrack*);
		void SetTrack2(TTrack*);
	// Information
		void Print();

private:
	TVector3 fRecPoint; // point where the particle probably scattered
	Double_t fEnergyLoss; // reconstructed energy loss at that point
	TTrack* fTrack1; // track in TPC 1 that was used to reconstruct this point
	TTrack* fTrack2; // track in TPC 2 that was used to reconstruct this point

};

// === TYPE DEFINITIONS =======
	typedef std::list<TRecPoint*> TRecPointList_t;
	typedef std::list<TRecPoint*>::iterator TRecPointIter_t;

#endif // TRECONSTRUCTEDPOINT_H
