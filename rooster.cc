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

#include <ctime>

using namespace std;


//*************************************************************************

// Default constructor
Rooster::Rooster ()
{
}  // default constructor

//*************************************************************************

// Leest een instantie in uit invoernaam.
bool Rooster::leesIn (const char* invoerNaam)
{
	ifstream invoer (invoerNaam, ios::in);

	if (!invoer.is_open()) { // Check of de file geopened kan worden
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

		invoer >> docenten[i]->nrTijdsloten;
		invoer.get();

		for(int j = 0; j < docenten[i]->nrTijdsloten; j++) {
			invoer >> docenten[i]->tijdsloten[j];
		}
		invoer.get();
	}

	invoer >> nrVakken;
	invoer.get();

	// Maakt 10 track objecten
	for (int i = 0; i < MaxNrTracks; i++) {
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

			// Vakken per track:
			tracken[vakken[i]->tracks[j]]->vakken.push_back(static_cast<int>(i));
			if (vakken[i]->tracks[j] >= nrTracks) {

				// Bepaalt hoeveel tracks er zijn
				nrTracks = vakken[i]->tracks[j]+1;
			}
		}
		invoer.get();
	}
	return true;
}  // leesIn

//*************************************************************************

// Druk de informatie van de instantie af op het scherm.
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
		cout << "  Hoeveelheid beschikbare tijdsloten: ";
		cout << docenten[i]->nrTijdsloten << endl;
		cout << "  Tijdsloten: ";
		for (int j = 0; j < docenten[i]->nrTijdsloten -1; j++) {
			cout << docenten[i]->tijdsloten[j] << ",";
		}
		cout << docenten[i]->tijdsloten[docenten[i]->nrTijdsloten-1] << endl;
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
	for (int i = 0; i < static_cast<int>(LengteNaamVak-3 + invoernaam.length()); i++) {
		cout << "-";
	}
	cout << endl << endl;
}  // drukAf ()

//*************************************************************************

// Checkt in gegeven vector voor dubbele instanties
// en returned een bool respectievelijk.
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

// Returned een vector met de intersectie van twee arrays.
vector<int> Rooster::intersect(int arr1[], int s1, int arr2[], int s2)
{
	vector<int> s;	// Waardes in beide arrays
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

// Checkt of de docent op dat tijdstip les kan geven
bool Rooster::docentBeschikbaar(int docent, int tijdslot)
{
	for (int i = 0; i < docenten[docent]->nrTijdsloten; i++) {
		// Als de les in de lijst met tijdsloten zit:
		if (tijdslot == docenten[docent]->tijdsloten[i]) {
			return true;
		}
	}
	return false; // Docent kan niet lesgeven in dat uur
} // docentBeschikbaar

//*************************************************************************

// Checkt of studenten niet twee vakken op hetzelfde moment hebben?
bool Rooster::overlapTracks(Vak* vak, int rooster[MaxNrTijdsloten][MaxNrZalen],
								int tijdslot)
{
	if (nrZalen > 1) { // Als er maar 1 zaal is, is het sowieso niet mogelijk
		for (int i = 0; i < nrZalen; i++) {
			if (rooster[tijdslot][i] != -1) {
				// Op een uur moet de intersectie 0 items bevatten,
				// dan is elke track uniek, dus geen dubbele lessen.
				if (intersect(vakken[rooster[tijdslot][i]]->tracks,
									vakken[rooster[tijdslot][i]]->nrTracks,
									vak->tracks,vak->nrTracks).size() > 0) {

					return true; // Er zijn overlappende tracks
				}
			}
		}
	}
	return false; // Er zijn geen overlappende tracks
} // overlapTracks

//*************************************************************************

// Returned tijdslot en zaalnummer van alle lessen van een track op een een dag
vector<int> Rooster::lesDag(int track, int rooster[MaxNrTijdsloten][MaxNrZalen],
							int tijdslot)
{
	vector<int> tijden;	// Lijst met tijden van de lessen
	for (int i = tijdslot; i < tijdslot+nrUrenPerDag; i++) {
		for (int j = 0; j < nrZalen; j++) {
			if (rooster[i][j] != -1) {
				for (int k = 0; k < vakken[rooster[i][j]]->nrTracks; k++) {
					if (vakken[rooster[i][j]]->tracks[k] == track) {
						tijden.push_back(i);	// Pushed tijdslot
						tijden.push_back(j); // Pushed zaalnummer
					}
				}
			}
		}
	}
	return tijden; // Returned de lijst met tijden
} // lesDag

//*************************************************************************

// returns true if nothing intersects
bool Rooster::specialIntersect(int arr1[], int s1, int arr2[], int s2)
{

	for (int i = 0; i < s1; i++) {
		for (int j = 0; j < s2; j++) {
			if (arr1[i]/nrUrenPerDag == arr2[j]/nrUrenPerDag
				&& arr1[i] != arr2[j])
			{
				return true;
			}
		}
	}
	return false;
}

//*************************************************************************

// Checkt hoeveel vakken een track heeft op een dag. Als dat één is,
// moet daar een reden voor zijn. Deze worden eveneens hier getest.
// Het returned of het rooster in dat opzichte valide is.
bool Rooster::nulOfTweeVak(int rooster[MaxNrTijdsloten][MaxNrZalen])
{
	vector<int> lessen;
	int l, aantal, docent1, docent2;

	if (maxTijdslot == 1 || nrUrenPerDag == 1) {
		return true;
	}

	for (int i = 0; i < nrTracks; i++) {
		for (int j = 0; j < nrDagen*nrUrenPerDag; j+=nrUrenPerDag) {

			// heeft een track dat we willen inroosteren al iets op die dag?
			lessen = lesDag(i, rooster, j);
			l = lessen.size();

			// als er maar 1 les op een dag is, moet daar een reden voor zijn
			if (l == 2) {
			
				// aantal vakken binnen de track
				aantal = static_cast<int>(tracken[i]->vakken.size());

				// als er maar 1 vak in deze track zit
				if (aantal == 1) {
					continue;
				}

				// de docent van het vak dat bij een track hoorde 
				// waarvan er die dag maar een uur is
				docent1 = vakken[rooster[lessen[0]][lessen[1]]]->docent;

				// kijkt of de docent van het alleenstaand vak op een dag 
				// les zou kunnen geven die overlapt met andere docenten
				for (int k = 0; k < aantal; k++) {
					docent2 = vakken[tracken[i]->vakken[k]]->docent;
					if (docent1 != docent2 &&
						specialIntersect(docenten[docent1]->tijdsloten,
						docenten[docent1]->nrTijdsloten,
						docenten[docent2]->tijdsloten,
						docenten[docent2]->nrTijdsloten))
					{
						return false;
					}
				}
			}
		}
		// een track heeft enkel vakken door docenten
		// met niet-matchende tijdsloten: niet mogelijk op 1 dag
	}
	return true;
}

//*************************************************************************

// Checkt of een docent maximaal 1 vak op een dag heeft?
bool Rooster::lesDocent(Vak* vak, int rooster[MaxNrTijdsloten][MaxNrZalen],
								int tijdslot)
{
	int dag = tijdslot/nrUrenPerDag; // Dag van het tijdslot
	for (int i = dag*nrUrenPerDag; i < (dag+1)*nrUrenPerDag; i++) {
		for (int j = 0; j < nrZalen; j++) {
			if (rooster[i][j] != -1
				&& vakken[rooster[i][j]]->docent == vak->docent)
			{
				return false; // Als een docent meer lessen geeft op een dag
			}
		}
	}
	return true; // Als een docent maximaal 1 les geeft op een dag
} // lesDocent

//*************************************************************************

// Checkt of studenten maximaal 1 tussenuur hebben
bool Rooster::tussenuur(int rooster[MaxNrTijdsloten][MaxNrZalen])
{
	vector<int> lessen; //
	int d = 0; // Hoeveelheid tussenuren
	for (int i = 0; i < nrTracks; i++) {
		// Heeft de track van het vak al iets op die dag?
		for (int k = 0; k < nrDagen*nrUrenPerDag; k+=nrUrenPerDag) {
			lessen = lesDag(i, rooster, k);
			if (lessen.size() >= 4) { // Lessen bevat 2 items per les: vak,zaal
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
	return true; // Maximaal 1 tussenuur
} // tussenuur

//*************************************************************************

// zet alle elementen van de 2d array rooster op -1
void Rooster::maakRoosterLeeg(int rooster[MaxNrTijdsloten][MaxNrZalen])
{
	// Maakt het gegeven rooster leeg,
	for (int i = 0; i < MaxNrTijdsloten; i++) {
		for (int j = 0; j < MaxNrZalen; j++) {
			rooster[i][j] = -1;
		}
	}
} // maakRoosterLeeg

//*************************************************************************

// Bepaalt een mogenlijk rooster voor de verschillende tracks,
// rekening houdend met de beschbaarheid van de docenten,
// en de eisen aan de tracks.
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
				rooster[i][j] = -1; // Haalt vak uit roostser
				vakIndex--; // Vorige vak
				break;
			}
		}
	}
	return false; // Geen rooster gevonden
} // bepaalRooster

//*************************************************************************

// Returned het laatste tijdslot waarop dit rooster les heeft
int Rooster::bepaalMaxTijdslot(int rooster[MaxNrTijdsloten][MaxNrZalen])
{
	//Loop achterwaards door het array tot een waarde is gevonden
	for (int i = nrDagen*nrUrenPerDag; i-- > 0;) {
		for (int j = nrZalen; j-- > 0;) {
			if (rooster[i][j] != -1) {
				return i;
			}
		}
	}
	return 0;
}

//*************************************************************************

// Stopt een zo kort mogenlijk rooster in rooster[][]
bool Rooster::bepaalMinRooster(int rooster[MaxNrTijdsloten][MaxNrZalen],
								long long &aantalDeelroosters)
{
	// Het idee is basically,
	// zodra we een rooster vinden, zoeken we wat het laatste uur is.
	// als dat uur (dus tijdslot) kleiner is dan het huidige tijdslot
	// wat we hebben gevonden, slaan we dat rooster op
	// uiteindelijk houden we alleen het kortste rooster over.
	// zoeken doen we precies hetzelfde, maar in plaats van afsluiten
	// slaan we m op en zoeken we verder.
	int tijdslot;
	bool geldigRooster = false;

	// als dit de eerste keer in deze functie is worden de roosters leeggemaakt
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

				// docent kan op dit uur
				docentBeschikbaar(vakken[vakIndex]->docent,i) &&

				// leerlingen niet 2 vakken op hetzelfde moment
				!overlapTracks(vakken[vakIndex],manipRooster,i) &&

				// maximaal 1 les per dag voor een docent
				lesDocent(vakken[vakIndex],manipRooster,i))
			{
				manipRooster[i][j] = vakIndex;
				vakIndex++;

				// testen achteraf:
				if (vakIndex == nrVakken) {
					if (// 0 of 2 vakken als mogelijk
						nulOfTweeVak(manipRooster) &&

						// max 1 tussenuur per dag
						tussenuur(manipRooster))
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

// Druk parameter rooster overzichtelijk af op het scherm
// (dag, tijd, zaal, naam van vak, nummer van docent).
void Rooster::drukAfRooster (int rooster[MaxNrTijdsloten][MaxNrZalen])
{
	int vak;
	int hulp;
	bool lesOpDag = false;
	for (int i = 1; i < nrZalen+1; i++) {
		cout << " Zaal " << i;
		for (int l = 0; l < LengteNaamVak-4-((i/10)+1); l++) { //compenseer met spaties
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
					cout << "+";
					for (int l = 0; l < LengteNaamVak; l++) {
						cout << "-";
					}
				}
				cout << "+" << endl;
				for (int k = 0; k < nrZalen; k++) {
					vak = rooster[i*nrUrenPerDag+j][k];
					if (vak != -1) {
						cout << "| " << vakken[vak]->naam; //vak

						//compenseer met spaties
						hulp = static_cast<int>(LengteNaamVak-1-vakken[vak]->naam.length());
						for (int l = 0; l < hulp; l++) {
							cout << " ";
						}
					} else {
						cout << "|";
						for (int l = 0; l < LengteNaamVak; l++) {
							cout << " ";
						}
					}
				}

				cout << "| Dag " << i + 1 << endl;
				for (int k = 0; k < nrZalen; k++) {
					vak = rooster[i*nrUrenPerDag+j][k];
					if (vak != -1) {
						cout << "| Docent: " << vakken[vak]->docent; //docent
						if (vakken[vak]->docent >= 10) {

							// compenseer met spaties
							for (int l = 0; l < LengteNaamVak-11; l++) {
								cout << " ";
							}
						} else {

							// compenseer met spaties
							for (int l = 0; l < LengteNaamVak-10; l++) {
								cout << " ";
							}
						}
					} else {
						cout << "|";
						for (int l = 0; l < LengteNaamVak; l++) {
							cout << " ";
						}
					}
				}

				cout << "| Uur " << j + 1 << endl;
				for (int k = 0; k < nrZalen; k++) {
					vak = rooster[i*nrUrenPerDag+j][k];
					if (vak != -1) {
						cout << "| Tracks: ";

						// print de eerste alvast, voor de komma's
						cout << vakken[vak]->tracks[0];

						for (int l = 1; l < vakken[vak]->nrTracks; l++) {
							cout << ", " << vakken[vak]->tracks[l]; //track
						}
						//compenseer met spaties
						for (int l = 0;
							l < LengteNaamVak-10-((vakken[vak]->nrTracks-1)*3); l++)
						{
							cout << " ";
						}
					} else {
						cout << "|";
						for (int l = 0; l < LengteNaamVak; l++) {
							cout << " ";
						}
					}
				}
				cout << "| " << endl;
			}
		}
	}

	for (int k = 0; k < nrZalen; k++) {
		cout << "+";
		for (int l = 0; l < LengteNaamVak; l++) {
			cout << "-";
		}
	}
	cout << "+" << endl;
}  // drukAfRooster

//*************************************************************************

// Bepaald de score voor een mogenlijk plek voor het inroosteren van een vak
int Rooster::bepaalScore(int nrVak, int tijdslot, int zaal,
						int rooster[MaxNrTijdsloten][MaxNrZalen])
{
	int score = 100-tijdslot; // minpunten voor een later tijdslot

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

// Bepaalt op een gretige manier een rooster voor de ingelezen vakken,
// docenten en tracks. Als het niet lukt om aan alle eisen voor een
// geldig rooster te voldoen, word er 'zo goed mogelijk' aan voldaan.
void Rooster::bepaalRoosterGretig (int rooster[MaxNrTijdsloten][MaxNrZalen])
{
	int score, besteScore, besteTijdslot, besteZaal;

	maakRoosterLeeg(rooster);

	// voor ieder vak
	for (int i = 0; i < nrVakken; i++) {
		besteScore = 0;

		// bereken de beste score per geschikte plek voor het vak
		for (int j = 0; j < nrDagen*nrUrenPerDag; j++) {
			for (int k = 0; k < nrZalen; k++) {
				if (rooster[j][k] == -1) {
					score = bepaalScore(i, j, k, rooster);

					// kijk of de score een nieuwe beste is
					if (score > besteScore) {
						besteScore = score;
						besteTijdslot = j;
						besteZaal = k;
					}
				}
			}
		}
		// rooster het vak in op de beste plek
		rooster[besteTijdslot][besteZaal] = i;
	}
}  // bepaalRoosterGretig
