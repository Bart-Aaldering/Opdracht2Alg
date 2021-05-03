// Implementatie van klasse Rooster

#include <iostream>
#include <fstream>
#include "standaard.h"
#include "rooster.h"
#include <cstring>
#include <cmath>
#include <vector>

using namespace std;

const bool CompactRooster = true;

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

	for (int i = 0; i < 10; i++) {
		tracken[i] = new Track;
	}

	for (int i = 0; i < nrVakken; i++) {
		vakken[i] = new Vak;

		getline(invoer, vakken[i]->naam);

		invoer >> vakken[i]->docent;
		invoer >> vakken[i]->nrTracks;
		invoer.get();

		for(int j = 0; j < vakken[i]->nrTracks; j++) {
			invoer >> vakken[i]->tracks[j];
		}
		for(int j = 0; j < vakken[i]->nrTracks; j++) {
			tracken[vakken[i]->tracks[j]]->vakken.push_back(static_cast<int>(i)); //vakken per track
			if (vakken[i]->tracks[j] > nrTracks) { //bepaalt hoeveel tracks er zijn
				nrTracks = vakken[i]->tracks[j];
			}
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

vector<int> Rooster::intersect(int arr1[], int s1, int arr2[], int s2) {
	// returns true if nothing intersects
	vector<int> s;
	for (int i = 0; i < s1; i++) {
		for (int j = 0; j < s2; j++) {
			if (arr1[i] == arr2[j]) {
				s.push_back(i);
			}
		}
	}
	return s;
}

//*************************************************************************

// Kan de docent op dat tijdstip les geven?
bool Rooster::docentBeschikbaar(int docent, int tijdslot) {
	for (int i = 0; i < docenten[docent]->nrBeschikbareTijdsloten; i++) {
		if (tijdslot == docenten[docent]->beschikbareTijdsloten[i]) {
			return true;
		}
	}
	return false;
}

//*************************************************************************

// Hebben leerlingen van dezelfde track 2 vakken op hetzelfde moment?
bool Rooster::overlapTracks(Vak* vak, int rooster[MaxNrTijdsloten][MaxNrZalen],
								int tijdslot) {
	// Hebben leerlingen niet 2 vakken op hetzelfde moment?
	if (nrZalen > 1) {
		for (int i = 0; i < nrZalen; i++) {
			if (rooster[tijdslot][i] != -1) {
				if (intersect(vakken[rooster[tijdslot][i]]->tracks,
									vakken[rooster[tijdslot][i]]->nrTracks,
									vak->tracks,vak->nrTracks).size() > 0) {
					return true;
				}
			}
		}
	}
	return false;
}

//*************************************************************************

// Heeft deze track les vandaag?
vector<int> Rooster::lesDag(int track, int rooster[MaxNrTijdsloten][MaxNrZalen],
							int tijdslot)
{
	vector<int> tijden;
	int dag = tijdslot/nrUrenPerDag;
	for (int i = dag*nrUrenPerDag; i < (dag+1)*nrUrenPerDag; i++) {
		for (int j = 0; j < nrZalen; j++) {
			if (rooster[i][j] != -1) {
				for (int k = 0; k < vakken[rooster[i][j]]->nrTracks; k++) {
					if (vakken[rooster[i][j]]->tracks[k] == track) {
						tijden.push_back(i);
					}
				}
			}
		}
	}
	return tijden;
}

//*************************************************************************

//CHECK ACHTERAF
bool Rooster::nulOfTweeVak(int rooster[MaxNrTijdsloten][MaxNrZalen])
{
	vector<int> lessen;
	int l;
	bool test = true;

	for (int i = 0; i < nrTracks; i++) {
		for (int k = 0; k < nrDagen; k+=nrUrenPerDag) {
			// heeft een track van het vak wat we willen inroosteren al iets op die dag?
			lessen = lesDag(i, rooster, k);
			l = lessen.size();
			if (l == 1) { // als er maar 1 les op een dag is, moet daar een reden voor zijn
				l = tracken[i]->vakken.size();
				if (l == 1) {// als er maar 1 vak in die track is
					continue;
				}
				for (int j = 0; j < l-1; j++) {//als alle docenten geen matchende tijden hebben
					if (test && intersect(docenten[vakken[tracken[i]->vakken[j]]->docent]->beschikbareTijdsloten,
									docenten[vakken[tracken[i]->vakken[j]]->docent]->nrBeschikbareTijdsloten,
									docenten[vakken[tracken[i]->vakken[j+1]]->docent]->beschikbareTijdsloten,
									docenten[vakken[tracken[i]->vakken[j+1]]->docent]->nrBeschikbareTijdsloten).size() > 0) {
						return false;
					}
				}
			}
		}
		// 1 track heeft enkel vakken door docenten met niet-matchende tijdsloten: niet mogelijk op 1 dag
	}
	return true;
}

//*************************************************************************


bool Rooster::lesDocent(Vak* vak, int rooster[MaxNrTijdsloten][MaxNrZalen],
								int tijdslot)
{
	// Heeft een docent maximaal 1 vak op een dag
	int dag = tijdslot/nrUrenPerDag;
	for (int i = dag*nrUrenPerDag; i < (dag+1)*nrUrenPerDag; i++) {
		for (int j = 0; j < nrZalen; j++) {
			if (rooster[i][j] != -1 && vakken[rooster[i][j]]->docent == vak->docent) {
				return false;
			}
		}
	}
	return true;
}

bool Rooster::tussenuur(int rooster[MaxNrTijdsloten][MaxNrZalen])
{
	vector<int> lessen;
	int d = 0;
	for (int i = 0; i < nrTracks; i++) {
		// heeft een track van het vak wat we willen inroosteren al iets op die dag?
		for (int k = 0; k < nrDagen*nrUrenPerDag; k+=nrUrenPerDag) {
			lessen = lesDag(i, rooster, k);
			if (lessen.size() >= 2) {
				for (int j = 0; j < static_cast<int>(lessen.size())-1; j++) {
					d += lessen[j+1]-lessen[j]-1;
				}
				if (d > 1) {
					return false;
				}
				d = 0;
			}
		}
	}
	return true;
}

//*************************************************************************

void Rooster::maakRoosterLeeg(int rooster[MaxNrTijdsloten][MaxNrZalen])
{
	for (int i = 0; i < MaxNrTijdsloten; i++) {
		for (int j = 0; j < MaxNrZalen; j++) {
			rooster[i][j] = -1;
		}
	}
}

//*************************************************************************

bool Rooster::bepaalRooster(int rooster[MaxNrTijdsloten][MaxNrZalen],
						long long &aantalDeelroosters)
{
	if (!aantalDeelroosters) {
		maakRoosterLeeg(rooster);
	}
	for (int i = 0; i < nrDagen*nrUrenPerDag; i++) {
		for (int j = 0; j < nrZalen; j++) {
			aantalDeelroosters++;
			if (rooster[i][j] == -1 &&
				docentBeschikbaar(vakken[vakIndex]->docent,i) && // docent kan op dit uur
				!overlapTracks(vakken[vakIndex],rooster,i) && // leerlingen niet 2 vakken op hetzelfde moment
				lesDocent(vakken[vakIndex],rooster,i) // maximaal 1 les per dag voor een docent
				) {

				rooster[i][j] = vakIndex;
				vakIndex++;
				if (vakIndex == nrVakken) { // testen achteraf:
					if (nulOfTweeVak(rooster) && // 0 of 2 vakken als mogelijk
						tussenuur(rooster) // max 1 tussenuur per dag
					) {
						vakIndex = 0;
						return true;
					}
				} else {
					if (bepaalRooster(rooster, aantalDeelroosters)) {
						return true;
					}
				}
				rooster[i][j] = -1;
				vakIndex--;
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
	// Het idee is basically,
	// zodra we een rooster vinden, zoeken we wat het laatste uur is.
	// als dat uur (dus tijdslot) kleiner is dan het huidige tijdslot
	// wat we hebben gevonden, slaan we dat rooster op
	// uiteindelijk houden we alleen het kortste rooster over.
	// zoeken doen we precies hetzelfde, maar in plaats van afsluiten
	// slaan we m op en zoeken we verder.
	return true;
}  // bepaalMinRooster

//*************************************************************************

void Rooster::drukAfRooster (int rooster[MaxNrTijdsloten][MaxNrZalen])
{
	int vak;
	bool lesOpDag = false;
	for (int i = 1; i < nrZalen+1; i++) {
		cout << " Zaal " << i;
		for (int l = 0; l < 26-((i/10)+1); l++) { //compenseer met spaties
			cout << " ";
		}
	}
	cout << endl;
	for (int i = 0; i < nrDagen; i++) {
		for (int j = 0; j < nrUrenPerDag; j++) {
			lesOpDag = false;
			if (CompactRooster) {
				for (int k = 0; k < nrZalen; k++) {
					if (rooster[(i*nrUrenPerDag) + j][k] != -1)
						lesOpDag = true;
				}
			}
			if (CompactRooster && lesOpDag) {
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
						if (vakken[vak]->docent >= 10) {
							for (int l = 0; l < 19; l++) { //compenseer met spaties
								cout << " ";
							}
						} else {
							for (int l = 0; l < 20; l++) { //compenseer met spaties
								cout << " ";
							}
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
	}

	for (int i = 0; i < nrZalen; i++) {
		cout << "+------------------------------"; //30 streepjes
	}
	cout << "+" << endl;
}  // drukAfRooster

//*************************************************************************


int Rooster::bepaalScore(int nrVak, int tijdslot, int zaal, int rooster[MaxNrTijdsloten][MaxNrZalen])
{
	int score = -tijdslot;
	if (docentBeschikbaar(vakken[nrVak]->docent, tijdslot)) {
		score += 100;
	}
	if (!overlapTracks(vakken[nrVak], rooster, tijdslot)) {
		score += 100;
	}
	if (lesDocent(vakken[nrVak], rooster, tijdslot)) {
		score += 100;
	}
	rooster[tijdslot][zaal] = nrVak;
	if (tussenuur(rooster)) {
		score += 100;
	}
	if (tussenuur(rooster)) {
		score += 100;
	}
	rooster[tijdslot][zaal] = -1;
	return score;
}
//*************************************************************************

void Rooster::bepaalRoosterGretig (int rooster[MaxNrTijdsloten][MaxNrZalen])
{
	int score, besteScore, besteTijdslot, besteZaal;
	int gemiddeldeScore = 0;
	maakRoosterLeeg(rooster);

	for (int i = 0; i < nrVakken; i++) {
		besteScore = 0;
		for (int j = 0; j < nrDagen*nrUrenPerDag; j++) {
			for (int k = 0; k < nrZalen; k++) {
				if (rooster[j][k] == -1) {
					score = bepaalScore(i, j, k, rooster);
					if (score > besteScore) {
						besteScore = score;
						besteTijdslot = j;
						besteZaal = k;
					}
				}
			}
		}
		rooster[besteTijdslot][besteZaal] = i;
		gemiddeldeScore += besteScore;
	}
	cout << "de gemiddelde score was: " << gemiddeldeScore / nrVakken << endl;
}  // bepaalRoosterGretig
