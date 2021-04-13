// Implementatie van klasse Rooster

#include <iostream>
#include <fstream>
#include "standaard.h"
#include "rooster.h"
#include <cstring>
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
	invoer.get();

	invoer >> nrVakken;
	invoer.get();

	for (int i = 0; i < nrVakken; i++) {
		vakken[i] = new Vak;

		invoer >> vakken[i]->naam;
		invoer.get();

		invoer >> vakken[i]->docent;
		invoer >> vakken[i]->nrTracks;
		invoer.get();

		for(int j = 0; j < vakken[i]->nrTracks; j++) {
			invoer >> vakken[i]->tracks;
		}
		invoer.get();
	}
	for (int i = 0; i < nrVakken; i++) {
		cout << vakken[i]->naam << endl;

		cout << vakken[i]->docent;
		cout << vakken[i]->nrTracks << endl;

		for(int j = 0; j < vakken[i]->nrTracks; j++) {
			cout << vakken[i]->tracks << ' ';
		}
		cout << endl;
	}
	return true;
}  // leesIn

//*************************************************************************

void Rooster::drukAf ()
{
	// TODO: implementeer deze memberfunctie

}  // drukAf ()

//*************************************************************************

bool Rooster::bepaalRooster (int rooster[MaxNrTijdsloten][MaxNrZalen],
								long long &aantalDeelroosters)
{
	// TODO: implementeer deze memberfunctie

	return true;
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
	// TODO: implementeer deze memberfunctie

}  // drukAfRooster

//*************************************************************************

void Rooster::bepaalRoosterGretig (int rooster[MaxNrTijdsloten][MaxNrZalen])
{
	// TODO: implementeer deze functie

}  // bepaalRoosterGretig

