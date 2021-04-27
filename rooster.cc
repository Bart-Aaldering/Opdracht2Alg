// Implementatie van klasse Rooster

#include <iostream>
#include <fstream>
#include "standaard.h"
#include "rooster.h"
#include <cstring>
#include <cmath>
#include <vector>

using namespace std;

//*************************************************************************

// Default constructor
Rooster::Rooster ()
{
	// TODO: implementeer (zo nodig) deze constructor

}  // default constructor

//*************************************************************************

bool Rooster::leesIn (const char* invoerNaam)
{
	ifstream invoer (invoerNaam, ios::in);

	if (!invoer.is_open()) {
		cout << "Kan file niet openen." << endl;
		return false;
	}

	invoernaam = invoerNaam; //slaat invoernaam op

	invoer >> nrDagen;
	invoer >> nrUrenPerDag;
	invoer >> nrZalen;
	invoer.get();

	invoer >> nrDocenten;
	invoer.get();

	for(int i = 0; i < nrDocenten; i++) {
		docenten[i] = new Docent;

		invoer >> docenten[i]->nrBeschikbareTijdsloten;
		invoer.get();

		for(int j = 0; j < docenten[i]->nrBeschikbareTijdsloten; j++) {
			invoer >> docenten[i]->beschikbareTijdsloten[j];
		}
		invoer.get();
	}

	invoer >> nrVakken;
	invoer.get();

	for (int i = 0; i < nrVakken; i++) {
		vakken[i] = new Vak;

		getline(invoer, vakken[i]->naam);

		invoer >> vakken[i]->docent;
		invoer >> vakken[i]->nrTracks;
		invoer.get();

		for(int j = 0; j < vakken[i]->nrTracks; j++) {
			invoer >> vakken[i]->tracks[j];
		}
		invoer.get();
	}
	return true;
}  // leesIn

//*************************************************************************

void Rooster::drukAf ()
{
	cout << "---------- Info " << invoernaam << " ----------" << endl;
	cout << "Dagen: " << nrDagen << endl;
	cout << "Uren per dag: " << nrUrenPerDag << endl;
	cout << "Hoeveelheid zalen: " << nrZalen << endl;
	cout << "Hoeveelheid docenten: " << nrDocenten << endl;
	cout << "Tijdschema's docenten:" << endl;
	for (int i = 0; i < nrDocenten; i++) {
		cout << "  Docent: " << i << endl;
		cout << "  Hoeveelheid beschikbare tijdsloten: " << docenten[i]->nrBeschikbareTijdsloten << endl;
		cout << "  Tijdsloten: ";
		for (int j = 0; j < docenten[i]->nrBeschikbareTijdsloten -1; j++) {
			cout << docenten[i]->beschikbareTijdsloten[j] << ",";
		}
		cout << docenten[i]->beschikbareTijdsloten[docenten[i]->nrBeschikbareTijdsloten-1] << endl;
		cout << endl;
	}
	cout << "Hoeveelheid vakken: " << nrVakken << endl;
	cout << "Vak info:" << endl;
	for (int i = 0; i < nrVakken; i++) {
		cout << "  Vak nummer: " << i << endl;
		cout << "  Naam vak: " << vakken[i]->naam << endl;
		cout << "  Docent: " << vakken[i]->docent << endl;
		cout << "  Hoeveelheid tracks: " << vakken[i]->nrTracks << endl;
		cout << "  Deelnemende tracks: ";

		for (int j = 0; j < vakken[i]->nrTracks-1; j++) {
			cout << vakken[i]->tracks[j] << ", ";
		}
		cout << vakken[i]->tracks[vakken[i]->nrTracks-1] << endl;
		cout << endl;
	}
	for (int i = 0; i < static_cast<int>(27 + invoernaam.length()); i++) {
		cout << "-";
	}
	cout << endl << endl;

}  // drukAf ()

//*************************************************************************

bool Rooster::checkdups(vector<int> arr)
{
	int arrSize = sizeof(arr)/sizeof(arr[0]);
	vector<int> dup;
	for (int i = 0; i < arrSize; i++) {
		for (int j = 0; j < i; j++) {
			if (dup[j] == arr[i])
				return false;
		}
		dup.push_back(arr[i]);
	}
	return true;
}

//*************************************************************************
bool Rooster::intersect(int arr1[], int s1, int arr2[], int s2) {
	//returns 1 if no intersect
	int s = 0;
	for (int i = 0; i < s1; i++) {
		for (int j = 0; j < s2; j++) {
			if (arr1[i] == arr2[j]) {
				s++;
			}
		}
	}
	if (s == 0) {
		return true;
	}
	return false;
}

//*************************************************************************


bool Rooster::docentBeschikbaar(int docent, int waarde) {
	// Kan de docent op dat tijdstip les geven?
	for (int i = 0; i < docenten[docent]->nrBeschikbareTijdsloten; i++) {
		if (waarde == docenten[docent]->beschikbareTijdsloten[i]) {
			return true;
		}
	}
	return false;
}

bool Rooster::dupTracks(Vak* vak, int rooster[MaxNrTijdsloten][MaxNrZalen],
								int tijdslot) {
	//Hebben leerlingen niet 2 vakken op hetzelfde moment?
	bool overlap = false;
	if (nrZalen > 1) {
		for (int i = 0; i < nrZalen; i++) {
			if (rooster[tijdslot][i] != -1) {
				if (!intersect(vakken[rooster[tijdslot][i]]->tracks,
									vakken[rooster[tijdslot][i]]->nrTracks,
									vak->tracks,vak->nrTracks)) {
					overlap = true;
				};
			}
		}
	}
	return overlap;
}

vector<int> Rooster::lesDag(int track, int rooster[MaxNrTijdsloten][MaxNrZalen],
							int tijdslot) {
	// Heeft deze track les vandaag?
	vector<int> tijden;
	int dag = tijdslot/nrDagen;
	for (int i = dag*nrUrenPerDag; i < (dag+1)*nrUrenPerDag; i++) {
		for (int j = 0; j < nrZalen; j++) {
			for (int k = 0; k < vakken[rooster[i][j]]->nrTracks; k++) {
				if (vakken[rooster[i][j]]->tracks[k] == track) {
					tijden.push_back(i);
				}
			}
		}
	}
	return tijden;
}

bool Rooster::nulOfTweeVak(Vak* vak, int rooster[MaxNrTijdsloten][MaxNrZalen],
								int tijdslot) {
	vector<int> lessen;
	int l;
	// Heeft een track max 1 tussenuur?
	for (int i = 0; i < vak->nrTracks; i++) {
		lessen = lesDag(vak->tracks[i], rooster, tijdslot);
		l = lessen.size();
		if (l != 1) {
			return true;
		}
		else {
				//2 vakken door 2 docenten met niet-matchende tijdsloten
				//2 vakken door 1 docenten voor dezelfde track
		}
	}
	return false;
}

bool Rooster::lesDocent(Vak* vak, int rooster[MaxNrTijdsloten][MaxNrZalen],
								int tijdslot) {
	//Heeft een docent maximaal 1 vak op een dag
	int dag = tijdslot/nrDagen;
	for (int i = dag*nrUrenPerDag; i < (dag+1)*nrUrenPerDag; i++) {
		for (int j = 0; j < nrZalen; j++) {
			if (vakken[rooster[i][j]]->docent == vak->docent) {
				return false;
			}
		}
	}
	return true;
}

bool Rooster::tussenuur(Vak* vak, int rooster[MaxNrTijdsloten][MaxNrZalen],
								int tijdslot) {
	//vector<int> lessen = lesDag(vak->tracks[i], rooster, tijdslot);
	//hier ben ik bezig...
}



//*************************************************************************

bool Rooster::bepaalRooster (int rooster[MaxNrTijdsloten][MaxNrZalen],
						long long &aantalDeelroosters)
{
	if (!aantalDeelroosters) {
		for (int i = 0; i < MaxNrTijdsloten; i++) {
			for (int j = 0; j < MaxNrZalen; j++) {
				rooster[i][j] = -1;
			}
		}
	}
	for (int i = 0; i < nrDagen*nrUrenPerDag; i++) {
		for (int j = 0; j < nrZalen; j++) {
			aantalDeelroosters++;
			if (rooster[i][j] == -1) {
				if (docentBeschikbaar(vakken[vakIndex]->docent,i) && // docent kan op dit uur
					!dupTracks(vakken[vakIndex],rooster,i) // && leerlingen niet 2 vakken op hetzelfde moment
					// lesDocent(vakken[vakIndex],rooster,i) && // maximaal 1 les per dag voor een docent
					// nulOfTweeVak(vakken[vakIndex],rooster,i)
					) { // maximaal 1 tussenuur en niet meer dan 2 uur op een dag
					rooster[i][j] = vakIndex;
					vakIndex++;
					if (vakIndex == nrVakken) {
						vakIndex = 0;
						return true;
					} else {
						if (bepaalRooster(rooster, aantalDeelroosters)) {
							return true;
						}
					}
					vakIndex--;
				}
				rooster[i][j] = -1;
			}
		}
	}
	return false;
}  // bepaalRooster

//*************************************************************************

bool Rooster::bepaalMinRooster (int rooster[MaxNrTijdsloten][MaxNrZalen],
								long long &aantalDeelroosters)
{
	// TODO: implementeer deze memberfunctie

	return true;
}  // bepaalMinRooster

//*************************************************************************

void Rooster::drukAfRooster (int rooster[MaxNrTijdsloten][MaxNrZalen])
{
	int vak;
	for (int i = 1; i < nrZalen+1; i++) {
		cout << " Zaal " << i;
		for (int l = 0; l < 26-((i/10)+1); l++) { //compenseer met spaties
			cout << " ";
		}
	}
	cout << endl;
	for (int i = 0; i < nrDagen; i++) {
		for (int j = 0; j < nrUrenPerDag; j++) {
			for (int k = 0; k < nrZalen; k++) {
				cout << "+------------------------------"; //30 streepkes
			}
			cout << "+" << endl;
			for (int k = 0; k < nrZalen; k++) {
				vak = rooster[i*nrUrenPerDag+j][k];
				if (vak != -1) {
					cout << "| " << vakken[vak]->naam; //vak
					for (int l = 0; l < static_cast<int>(29-vakken[vak]->naam.length()); l++) { //compenseer met spaties
						cout << " ";
					}
				} else {
					cout << "|                              ";
				}
			}

			cout << "| Dag " << i + 1 << endl;
			for (int k = 0; k < nrZalen; k++) {
				vak = rooster[i*nrUrenPerDag+j][k];
				if (vak != -1) {
					cout << "| Docent: " << vakken[vak]->docent; //docent
					for (int l = 0; l < 20; l++) { //compenseer met spaties
						cout << " ";
					}
				} else {
					cout << "|                              ";
				}
			}

			cout << "| Uur " << j + 1 << endl;
			for (int k = 0; k < nrZalen; k++) {
				vak = rooster[i*nrUrenPerDag+j][k];
				if (vak != -1) {
					cout << "| Tracks: ";
					cout << vakken[vak]->tracks[0]; //print de eerste alvast, voor de komma's
					for (int l = 1; l < vakken[vak]->nrTracks; l++) {
						cout << ", " << vakken[vak]->tracks[l]; //track
					}
					for (int l = 0; l < 20 - ((vakken[vak]->nrTracks-1)*3); l++) { //compenseer met spaties
						cout << " ";
					}
				} else {
					cout << "|                              ";
				}
			}
			cout << "| " << endl;
		}
	}

	for (int i = 0; i < nrZalen; i++) {
		cout << "+------------------------------"; //30 streepjes
	}
	cout << "+" << endl;
}  // drukAfRooster

//*************************************************************************

void Rooster::bepaalRoosterGretig (int rooster[MaxNrTijdsloten][MaxNrZalen])
{
	// TODO: implementeer deze functie

}  // bepaalRoosterGretig
