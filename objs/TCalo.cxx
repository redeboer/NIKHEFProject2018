// Author: Remco de Boer
// Date: May 23rd, 2018
// Inspired by Corryvreckan

/* === CLASS DESCRIPTION ======
	This object contains all relevant information of a calorimeter. (At this stage, that is only energy.)
*/

// === INCLUDE =======
	#include "TCalo.h"
	#include <iostream>
	using namespace std;

// === GETTERS =======
	TString& TCalo::GetName() { return fName; }
	ULong64_t TCalo::GetTimestamp() const { return fTimestamp; }
	Double_t TCalo::GetEnergy() const { return fEnergy; }

// === MODIFIERS =======
	void TCalo::SetName(TString& name) { fName = name; }
	void TCalo::SetName(const char* name) { fName = name; }
	void TCalo::SetTimestamp(ULong64_t timestamp) {
		fTimestamp = timestamp; }
	void TCalo::SetEnergy( Double_t energy ) {
		fEnergy = energy; }

// === MODIFIERS =======
	void TCalo::Print() {
		cout << "Calorimeter \"" << fName << "\": " << fEnergy << endl; }