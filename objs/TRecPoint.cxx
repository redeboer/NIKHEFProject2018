// Author: Remco de Boer
// Date: May 23rd, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION ======
	This object describes the point where the proton probably scattered, including its energy loss. To be able to retrieve how it was reconstructed, pointers to the tracks that were used to reconstruct the it are also stored.
*/

// === INCLUDE =======
	#include "TRecPoint.h"
	#include <stdio.h>
	#include <iostream>
	using namespace std;

// === GETTERS =======
	TVector3& TRecPoint::GetPoint() { return fRecPoint; }
	Double_t TRecPoint::GetEnergyLoss() const { return fEnergyLoss; }
	TTrack* TRecPoint::GetTrack1() { return fTrack1; }
	TTrack* TRecPoint::GetTrack2() { return fTrack2; }

// === MODIFIERS =======
	void TRecPoint::SetPoint( Double_t x, Double_t y, Double_t z ) {
		fRecPoint.SetX(x);
		fRecPoint.SetY(y);
		fRecPoint.SetZ(z); }
	void TRecPoint::SetEnergyLoss( Double_t de ) { fEnergyLoss=de; }
	void TRecPoint::SetTrack1(TTrack* track) { fTrack1 = track; }
	void TRecPoint::SetTrack2(TTrack* track) { fTrack2 = track; }

// === MODIFIERS =======
	void TRecPoint::Print() {
		printf("Coordinates: %7.3f  %7.3f  %7.3f\n",
			fRecPoint.X(), fRecPoint.Y(), fRecPoint.Z() );
		printf("Energy loss: %f\n", fEnergyLoss );
		if(fTrack1&&fTrack2) cout << "(constructed from \"" <<
			fTrack1->GetCluster()->GetName() << "\" and \"" <<
			fTrack2->GetCluster()->GetName() << "\"" << endl;
	}