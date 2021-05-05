// Implementatie van klasse Rooster
// rooster.cc
// Geschreven door : Bart Aaldering (s2969866) en Lex Janssens (s2989344)
// Compiler: GNU GCC Compiler
// Beschrijving:
/*
	Hier staan alle functies uitgewerkt. Dit bestand voert, buiten het menu,
	alles uit. Het bevat de backtracking algoritmes om een rooster, minimaal
	rooster en een gretig rooster te bepalen.
*/

#include <iostream>
#include <fstream>
#include "standaard.h"
#include "rooster.h"
#include <cstring>
#include <vector>

using namespace std;

// Wanneer deze constante op true staat, wordt er bij een gevonden rooster
// enkel de rijen afgedrukt die een vak bevatten. Dit maakt het lezen van
// het rooster overzichtelijker, door alle uren waar geen les op wordt
// gegeven eruit te filteren. False drukt het gehele rooster af.
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

	if (!invoer.is_open()) { //Check of de file geopened kan worden
		cout << "Kan file niet openen." << endl;
		return false;
	}

	invoernaam = invoerNaam; // Slaat de invoernaam op

	invoer >> nrDagen;
	invoer >> nrUrenPerDag;
	invoer >> nrZalen;
	invoer.get();

	invoer >> nrDocenten;
	invoer.get();

	// Maakt een object voor elke docent met daarin haar beschikbare tijden
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

	// Maakt 10 track objecten
	for (int i = 0; i < 10; i++) {
		tracken[i] = new Track;
	}

	// Maakt Vak objecten met haar eigenschappen
	for (int i = 0; i < nrVakken; i++) {
		vakken[i] = new Vak;

		getline(invoer, vakken[i]->naam);

		invoer >> vakken[i]->docent;
		invoer >> vakken[i]->nrTracks;
		invoer.get();

		for(int j = 0; j < vakken[i]->nrTracks; j++) {
			invoer >> vakken[i]->tracks[j];
		}
		// Slaat in de juiste Track op, of die Track dat vak heeft.
		for(int j = 0; j < vakken[i]->nrTracks; j++) {
			//Vakken per track:
			tracken[vakken[i]->tracks[j]]->vakken.push_back(static_cast<int>(i));
			if (vakken[i]->tracks[j] > nrTracks) {
				//Bepaalt hoeveel tracks er zijn
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
	// Drukt alle informatie die uit het invoerbestand is gehaald af.
	// Algemene informatie:
	cout << "---------- Info " << invoernaam << " ----------" << endl;
	cout << "Dagen: " << nrDagen << endl;
	cout << "Uren per dag: " << nrUrenPerDag << endl;
	cout << "Hoeveelheid zalen: " << nrZalen << endl;

	// Docent informatie:
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

	// Vak informatie
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
	// Sluit netjes af.
	for (int i = 0; i < static_cast<int>(27 + invoernaam.length()); i++) {
		cout << "-";
	}
	cout << endl << endl;

}  // drukAf ()

//*************************************************************************

bool Rooster::checkdups(vector<int> arr)
{

	int arrSize = sizeof(arr)/sizeof(arr[0]); // Lengte van de array
	vector<int> dup;	// Vector met mogelijke duplicates
	for (int i = 0; i < arrSize; i++) {
		for (int j = 0; j < i; j++) {
			if (dup[j] == arr[i])
				return false; // Zodra gevonden, eindigt de functie
		}
		dup.push_back(arr[i]); // Voegt item toe aan checklist
	}
	return true;
} // checkdups

//*************************************************************************

vector<int> Rooster::intersect(int arr1[], int s1, int arr2[], int s2) {
	// Returned een vector met de intersectie van twee arrays.
	vector<int> s;	//Waardes in beide arrays
	for (int i = 0; i < s1; i++) {
		for (int j = 0; j < s2; j++) {
			if (arr1[i] == arr2[j]) {
				s.push_back(i); // Als waarde in beide arrays, voeg toe.
			}
		}
	}
	return s; //Returned de intersectie.
} //intersect

//*************************************************************************

bool Rooster::docentBeschikbaar(int docent, int tijdslot) {
	// Checkt of de docent op dat tijdstip les kan geven
	for (int i = 0; i < docenten[docent]->nrBeschikbareTijdsloten; i++) {
		// Als de les in de lijst met tijdsloten zit:
		if (tijdslot == docenten[docent]->beschikbareTijdsloten[i]) {
			return true;
		}
	}
	return false; // Docent kan niet lesgeven in dat uur
}// docentBeschikbaar

//*************************************************************************

bool Rooster::overlapTracks(Vak* vak, int rooster[MaxNrTijdsloten][MaxNrZalen],
								int tijdslot)
{
	// Checkt of studenten niet twee vakken op hetzelfde moment hebben?
	if (nrZalen > 1) { //Als er maar 1 zaal is, is het sowieso niet mogelijk
		for (int i = 0; i < nrZalen; i++) {
			if (rooster[tijdslot][i] != -1) {
				// Op een uur moet de intersectie 0 items bevatten,
				// dan is elke track uniek, dus geen dubbele lessen.
				if (intersect(vakken[rooster[tijdslot][i]]->tracks,
									vakken[rooster[tijdslot][i]]->nrTracks,
									vak->tracks,vak->nrTracks).size() > 0) {

					return true; //Er zijn overlappende tracks
				}
			}
		}
	}
	return false; //Er zijn geen overlappende tracks
} // overlapTracks

//*************************************************************************

vector<int> Rooster::lesDag(int track, int rooster[MaxNrTijdsloten][MaxNrZalen],
							int tijdslot)
{  // Returned lessen van een track op een bepaalde uur op de dag
	vector<int> tijden;	// Lijst met tijden van de lessen
	int dag = tijdslot/nrUrenPerDag;	// Vind de hele dag van tijdslot
	for (int i = dag*nrUrenPerDag; i < (dag+1)*nrUrenPerDag; i++) {
		for (int j = 0; j < nrZalen; j++) {
			if (rooster[i][j] != -1) {
				for (int k = 0; k < vakken[rooster[i][j]]->nrTracks; k++) {
					if (vakken[rooster[i][j]]->tracks[k] == track) {
						tijden.push_back(i);	//Pushed vaknummer
						tijden.push_back(j); //Pushed zaalnummer
					}
				}
			}
		}
	}
	return tijden; //Returned de lijst met tijden
}// lesDag

//*************************************************************************

bool Rooster::specialIntersect(int arr1[], int s1, int arr2[], int s2) {
	// returns true if nothing intersects
	for (int i = 0; i < s1; i++) {
		for (int j = 0; j < s2; j++) {
			if (arr1[i]/nrUrenPerDag == arr2[j]/nrUrenPerDag && arr1[i] != arr2[j]) {
				return true;
			}
		}
	}
	return false;
}

bool Rooster::nulOfTweeVak(int rooster[MaxNrTijdsloten][MaxNrZalen])
{
	vector<int> lessen;
	int l, doc;
	if (maxTijdslot == 1) {
		return true;
	}
	for (int i = 0; i < nrTracks; i++) {
		for (int k = 0; k < nrDagen; k+=nrUrenPerDag) {
			// heeft een track van het vak wat we willen inroosteren al iets op die dag?
			lessen = lesDag(i, rooster, k);
			l = lessen.size();
			if (l == 2) { // als er maar 1 les op een dag is, moet daar een reden voor zijn
				if (tracken[i]->vakken.size() == 1) {// als er maar 1 vak in die track is
					continue;
				}
				doc = vakken[rooster[lessen[0]][lessen[1]]]->docent;
				for (int j = 0; j < tracken[i]->vakken.size(); j++) {// als alle docenten geen matchende tijden hebben
					if (doc != vakken[tracken[i]->vakken[j]]->docent &&
							specialIntersect(docenten[doc]->beschikbareTijdsloten,
							docenten[doc]->nrBeschikbareTijdsloten,
							docenten[vakken[tracken[i]->vakken[j]]->docent]->beschikbareTijdsloten,
							docenten[vakken[tracken[i]->vakken[j]]->docent]->nrBeschikbareTijdsloten)) {
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
	// Checkt of een docent maximaal 1 vak op een dag heeft?
	int dag = tijdslot/nrUrenPerDag; //Dag van het tijdslot
	for (int i = dag*nrUrenPerDag; i < (dag+1)*nrUrenPerDag; i++) {
		for (int j = 0; j < nrZalen; j++) {
			if (rooster[i][j] != -1 && vakken[rooster[i][j]]->docent == vak->docent) {
				return false; // Als een docent meer lessen geeft op een dag
			}
		}
	}
	return true; // Als een docent maximaal 1 les geeft op een dag
} //lesDocent

bool Rooster::tussenuur(int rooster[MaxNrTijdsloten][MaxNrZalen])
{
	// Checkt of studenten maximaal 1 tussenuur hebben
	vector<int> lessen; //
	int d = 0; // Hoeveelheid tussenuren
	for (int i = 0; i < nrTracks; i++) {
		// Heeft de track van het vak al iets op die dag?
		for (int k = 0; k < nrDagen*nrUrenPerDag; k+=nrUrenPerDag) {
			lessen = lesDag(i, rooster, k);
			if (lessen.size() >= 4) { //Lessen bevat 2 items per les: vak,zaal
				for (int j = 0; j < static_cast<int>(lessen.size()/2)-1; j++) {
					d += lessen[j+1]-lessen[j]-1;
				}
				if (d > 1) { // Als er meer dan 1 tussenuur is
					return false;
				}
				d = 0;
			}
		}
	}
	return true; //Maximaal 1 tussenuur
} //tussenuur

//*************************************************************************

void Rooster::maakRoosterLeeg(int rooster[MaxNrTijdsloten][MaxNrZalen])
{
	// Maakt het gegeven rooster leeg,
	for (int i = 0; i < MaxNrTijdsloten; i++) {
		for (int j = 0; j < MaxNrZalen; j++) {
			rooster[i][j] = -1;
		}
	}
} //maakRoosterLeeg

//*************************************************************************

bool Rooster::bepaalRooster(int rooster[MaxNrTijdsloten][MaxNrZalen],
						long long &aantalDeelroosters)
{
	if (!aantalDeelroosters) { // Begin met 0 deelRoosters en leeg rooster
		maakRoosterLeeg(rooster);
	}
	for (int i = 0; i < nrDagen*nrUrenPerDag; i++) {
		for (int j = 0; j < nrZalen; j++) {
			aantalDeelroosters++;
			if (// Er staat geen vak ingepland op dit uur
				rooster[i][j] == -1 &&
				// Checkt beschikbaarheid docent op dit uur
				docentBeschikbaar(vakken[vakIndex]->docent,i) &&
				// Checkt studenten vakken overlap
				!overlapTracks(vakken[vakIndex],rooster,i) &&
				// Checkt maximaal 1 les per dag voor een docent
				lesDocent(vakken[vakIndex],rooster,i)
				) {
				rooster[i][j] = vakIndex; // Rooster vak in
				vakIndex++;	// Volgende vak
				if (vakIndex == nrVakken) { // Als alle vakken zijn ingeroosterd
					if (// Checkt 0 of 2 vakken als mogelijk, met extra tests
						nulOfTweeVak(rooster) &&
						// Checkt maximaal 1 tussenuur per dag
						tussenuur(rooster)
					) {
						vakIndex = 0; // Klaar met zoeken
						return true;
					}
				} else {
					if (bepaalRooster(rooster, aantalDeelroosters)) {
						return true; // Wanneer klaar, stoppen
					}
				}
				rooster[i][j] = -1; //Haalt vak uit roostser
				vakIndex--; //Vorige vak
				break;
			}
		}
	}
	return false; //Geen rooster gevonden
}  // bepaalRooster

//*************************************************************************

int Rooster::bepaalMaxTijdslot(int rooster[MaxNrTijdsloten][MaxNrZalen]) {

	for (int i = nrDagen*nrUrenPerDag; i-- > 0;) {
		for (int j = nrZalen; j-- > 0;) {
			if (rooster[i][j] != -1) {
				return i;
			}
		}
	}
	return 0;
}

bool Rooster::bepaalMinRooster(int rooster[MaxNrTijdsloten][MaxNrZalen],
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
	int tijdslot;
	bool geldigRooster = false;

	if (!aantalDeelroosters) {
		maakRoosterLeeg(manipRooster);
		maakRoosterLeeg(rooster);
		maxTijdslot = MaxNrTijdsloten+1;
	}
	for (int i = 0; i < nrDagen*nrUrenPerDag; i++) {
		if (i >= maxTijdslot) {
			break;
		}
		for (int j = 0; j < nrZalen; j++) {
			aantalDeelroosters++;
			if (manipRooster[i][j] == -1 &&
				docentBeschikbaar(vakken[vakIndex]->docent,i) && // docent kan op dit uur
				!overlapTracks(vakken[vakIndex],manipRooster,i) && // leerlingen niet 2 vakken op hetzelfde moment
				lesDocent(vakken[vakIndex],manipRooster,i) // maximaal 1 les per dag voor een docent
				) {
				manipRooster[i][j] = vakIndex;
				vakIndex++;
				if (vakIndex == nrVakken) { // testen achteraf:;
					if (nulOfTweeVak(manipRooster) && // 0 of 2 vakken als mogelijk
						tussenuur(manipRooster)) // max 1 tussenuur per dag
						{
						tijdslot = bepaalMaxTijdslot(manipRooster);
						if (tijdslot < maxTijdslot) {
							geldigRooster = true;
							maxTijdslot = tijdslot;

							for (int k = 0; k < nrDagen*nrUrenPerDag; k++) {
								for (int l = 0; l < nrZalen; l++) {
									rooster[k][l] = manipRooster[k][l];
								}
							}

						}
					}
				} else {
					if (bepaalMinRooster(rooster, aantalDeelroosters)) {
						geldigRooster = true;
					}
				}
				manipRooster[i][j] = -1;
				vakIndex--;
				break;
			}
		}
	}
	return geldigRooster;
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
	int score = 100-tijdslot;
	if (docentBeschikbaar(vakken[nrVak]->docent, tijdslot)) {
		score += 200;
	}
	if (!overlapTracks(vakken[nrVak], rooster, tijdslot)) {
		score += 200;
	}
	if (lesDocent(vakken[nrVak], rooster, tijdslot)) {
		score += 200;
	}
	rooster[tijdslot][zaal] = nrVak;
	if (nulOfTweeVak(rooster)) {
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

	maakRoosterLeeg(rooster);
	cout << nrVakken << endl;
	for (int i = 0; i < nrVakken; i++) {
		besteScore = 0;
		cout << "i: " << i << endl;
		for (int j = 0; j < nrDagen*nrUrenPerDag; j++) {
			cout << "j: " << j << endl;
			for (int k = 0; k < nrZalen; k++) {
				cout << "k: " << k << endl;
				if (rooster[j][k] == -1) {
					score = bepaalScore(i, j, k, rooster);
					cout << score << endl;
					if (score > besteScore) {
						besteScore = score;
						besteTijdslot = j;
						besteZaal = k;
					}
				}
			}
		}
		rooster[besteTijdslot][besteZaal] = i;
	}
}  // bepaalRoosterGretig
