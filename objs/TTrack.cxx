// Author: Remco de Boer
// Date: May 22nd, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION ======
	This object describes a linear track in a TPC. It therefore contains a point of entry (fState) and a direction vector (fDirection), both with errors.
	Since this object can be created from a fitting algorithm, it also contains data members that contain information about the original fits and a pointer to the cluster on which this fit was performed. These members do not have to be allocated, as you can for instance also get your track information (the vectors) directly from Geant simulations.
*/

// === INCLUDE =======
	#include "TTrack.h"
	#include <stdio.h>
	#include <iostream>
	using namespace std;

// === GETTERS =======
	TVector3 TTrack::GetState() const { return fState; }
	TVector3& TTrack::GetStateError() { return fStateErr; }
	TVector3& TTrack::GetDirection() { return fDirection; }
	TVector3& TTrack::GetDirectionError() { return fDirectionErr; }
	TTimepix* TTrack::GetCluster() const { return fCluster; }
	TLinearFitter* TTrack::GetFitterXY() { return fFitXY; }
	TLinearFitter* TTrack::GetFitterXZ() { return fFitXZ; }

// === MODIFIERS =======
	void TTrack::SetState( Double_t x, Double_t y, Double_t z ) {
		fState.SetX(x);
		fState.SetY(y);
		fState.SetZ(z); }
	void TTrack::SetDirection( Double_t x, Double_t y, Double_t z ) {
		fDirection.SetX(x);
		fDirection.SetY(y);
		fDirection.SetZ(z); }
	void TTrack::SetStateError( Double_t x, Double_t y, Double_t z ) {
		fStateErr.SetX(x);
		fStateErr.SetY(y);
		fStateErr.SetZ(z); }
	void TTrack::SetDirectionError( Double_t x, Double_t y, Double_t z ) {
		fDirectionErr.SetX(x);
		fDirectionErr.SetY(y);
		fDirectionErr.SetZ(z); }

// === MODIFIERS =======
	void TTrack::Print() {
		if(fCluster) {
			cout << endl << "Track data for cluster \"" << fCluster->GetName() << "\"" << endl;
			cout << "  Chi square XY: " << fFitXY->GetChisquare() << endl;
			cout << "  Chi square XZ: " << fFitXZ->GetChisquare() << endl;
		}
		printf("%10.3f +- %5.3f   %7.4f +- %6.4f\n",
			fState.X(), fStateErr.X(), fDirection.X(), fDirectionErr.X() );
		printf("%10.3f +- %5.3f   %7.4f +- %6.4f\n",
			fState.Y(), fStateErr.Y(), fDirection.Y(), fDirectionErr.Y() );
		printf("%10.3f +- %5.3f   %7.4f +- %6.4f\n",
			fState.Z(), fStateErr.Z(), fDirection.Z(), fDirectionErr.Z() );
	}