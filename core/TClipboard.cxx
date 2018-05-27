// Author: Remco de Boer
// Date: May 20th, 2018
// Inspired by Corryvreckan

/* === CLASS DESCRIPTION =======
	The TClipboard class is used to transfer information between algorithms during the event processing.
	Any object inheriting from TBeamObject can be placed on the clipboard and retrieved by its name. At the end of each event (each algorithm run), the clipboard is wiped clean. It is therefore best practice to add any dynamically created object immediately to the clipboard using Put.
*/

// === INCLUDES =======
	#include "TClipboard.h"
	using namespace std;

// === MODIFIERS =======
	// Add objects to clipboard
	void TClipboard::Put(string name, TBeamObject* object)
	{
		fObjectsMap[name].push_back(object);
	}
	// Add specific objects to the clipboard (name detected 'automatically')
	void TClipboard::Put(TCalo*     obj) { Put("calorimeter", obj); }
	void TClipboard::Put(TPixel*    obj) { Put("pixels",      obj); }
	void TClipboard::Put(TRecPoint* obj) { Put("recpoint",    obj); }
	void TClipboard::Put(TTimepix*  obj) { Put("timepix",     obj); }
	void TClipboard::Put(TTrack*    obj) { Put("tracks",      obj); }

	// Clear items on the clipboard
	void TClipboard::Clear()
	{
		// loop over map
		TBeamObjectMapIter_t mit = fObjectsMap.begin();
		while( mit!=fObjectsMap.end() ) {
			// loop over each list in the map
			TBeamObjectIter_t it = mit->second.begin();
			while( it!=mit->second.end() ) {
				delete *it;
				++it;
			}
			// erase map element
			fObjectsMap.erase(mit);
			++mit;
		}
	}

// === GETTERS =======
	// Get objects from clipboard by name
	TBeamObjectList_t* TClipboard::Get(string& name) {
		return &fObjectsMap[name]; }
	TBeamObjectList_t* TClipboard::Get(TString& name) {
		string str(name.Data());
		return &fObjectsMap[str]; }
	TBeamObjectList_t* TClipboard::Get(const char* name) {
		return &fObjectsMap[name]; }
	TFile* TClipboard::GetOutputFile() { return fFile; }

// === INFORMATION FUNCTIONS =======
	// Quick function to check what is currently held by the clipboard
	void TClipboard::CheckCollections()
	{
		if(fObjectsMap.size()) {
			cout << endl <<
				"Clipboard contains " << fObjectsMap.size() << " object types" << endl;
			TBeamObjectMapIter_t mit = fObjectsMap.begin();
			while( mit!=fObjectsMap.end() ) {
				cout << "  \"" << mit->first << "\" with " << mit->second.size() << " members" << endl;
				++mit;
			}
		} else cout << endl << "Clipboard is empty" << endl;
	}