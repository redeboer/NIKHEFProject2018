// Author: Remco de Boer
// Date: May 22nd, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION ======
	This object describes a linear track in a TPC. It therefore contains a point of entry (fState) and a direction vector (fDirection), both with errors.
	Since this object can be created from a fitting algorithm, it also contains data members that contain information about the original fits and a pointer to the cluster on which this fit was performed. These members do not have to be allocated, as you can for instance also get your track information (the vectors) directly from Geant simulations.
*/

#ifndef TRACK_H
#define TRACK_H 1

// === INCLUDE =======
	#include "TLinearFitter.h"
	#include "TBeamObject.h"
	#include "TTimepix.h"
	#include "TVector3.h"

// === CLASS DECLARATION =======
class TTrack : public TBeamObject {
  
public:
	// Constructors and destructors
		TTrack(TTimepix* cluster)
			: fCluster(cluster) {
				fFitXY = new TLinearFitter(fCluster->GetNHits(),"pol1");
				fFitXZ = new TLinearFitter(fCluster->GetNHits(),"pol1"); }
		~TTrack() {
			if(fFitXY) delete fFitXY;
			if(fFitXZ) delete fFitXZ;}
	// Getters
		TVector3 GetState() const;
		TVector3& GetStateError();
		TVector3& GetDirection();
		TVector3& GetDirectionError();
		TTimepix* GetCluster() const;
		TLinearFitter* GetFitterXY();
		TLinearFitter* GetFitterXZ();
	// Modifiers
		void SetState(Double_t,Double_t,Double_t);
		void SetDirection(Double_t,Double_t,Double_t);
		void SetStateError(Double_t,Double_t,Double_t);
		void SetDirectionError(Double_t,Double_t,Double_t);
	// Information
		void Print();

private:
	// Track data members
	TVector3 fState;     // point where particle enters TPC
	TVector3 fDirection; // direction of particle track
	// Fit data members
	TVector3 fStateErr;
	TVector3 fDirectionErr;
	TTimepix* fCluster; // pointer to collection of points on which fits are based
	TLinearFitter* fFitXY;
	TLinearFitter* fFitXZ;

};

// === TYPE DEFINITIONS =======
	typedef std::list<TTrack*> TTrackList_t;
	typedef std::list<TTrack*>::iterator TTrackIter_t;

#endif // TRACK_H
