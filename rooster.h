// Definitie van klasse Rooster

#ifndef RoosterHVar  // voorkom dat dit bestand meerdere keren
#define RoosterHVar  // ge-include wordt

#include "constantes.h"
#include <string>
#include <vector>

using namespace std;

class Docent
{
	//Object Docent
	public:
		int nrBeschikbareTijdsloten;		// Aantal beschikbare tijdsloten
		int beschikbareTijdsloten[50];	// Array van tijdsloten
	private:
}; //Docent

class Vak
{
	// Object Vak
	public:
		string naam = "";						// Naam van het vak
		int docent;								// Nummer van die docent
		int nrTracks;							// Aantal tracks die dit vak hebben
		int tracks[10];						// Array van die tracks
	private:
}; // Vak

class Track
{
	// Object Track
	public:
		vector<int> vakken;					// Vector van vakken van de track
}; // Track

class Rooster
{
	public:

		// Default constructor
		Rooster ();
		// Lees een instantie in uit invoernaam.
		// Controleer daarbij of het bestand wel te openen is.
		// Pre:
		// * het aantal dagen, het aantal uren per dag, het aantal zalen,
		//   het aantal docenten, het aantal tijdsloten per docent,
		//   die tijdsloten zelf, het aantal vakken, de nummers van de docent
		//   van een vak, het aantal tracks en de tracks zelf die je inleest
		//   vallen binnen de gegeven grenzen (dat hoef je dus niet meer
		//   te controleren).
		// Retourneer:
		// * true, als het bestand te openen was.
		//   In dat geval is alle ingelezen informatie in het object opgeslagen.
		// * false, anders
		bool leesIn (const char* invoerNaam);

		// Druk de informatie van de instantie af op het scherm.
		void drukAf ();

		// Checkt of een docent beschikbaar is op gegeven tijdslot
		// en returned een bool respectievelijk.
		bool docentBeschikbaar(int docent, int tijdslot);

		// Checkt of er niet twee vakken op hetzelfde moment worden gegeven
		// en returned een bool respectievelijk.
		bool overlapTracks(Vak* vak, int rooster[MaxNrTijdsloten][MaxNrZalen],
								int tijdslot);

		// Checkt in gegeven vector voor dubbele instanties
		// en returned een bool respectievelijk.
		bool checkdups(vector<int> arr);

		// Geeft de intersectie van twee arrays terug als vector
		vector<int> intersect(int arr1[], int s1, int arr2[], int s2);

		// Checkt of alle docenten geen matchende tijden hebben met een
		// andere docent *per dag*. Deze intersectie is speciaal gericht
		// naar deze eis.
		bool specialIntersect(int arr1[], int s1, int arr2[], int s2);

		// Geeft alle lessen op een gegeven dag van een gegeven track terug
		// als vector.
		vector<int> lesDag(int track, int rooster[MaxNrTijdsloten][MaxNrZalen],
							int tijdslot);

		// Checkt hoeveel vakken een track heeft op een dag. Als dat één is,
		// moet daar een reden voor zijn. Deze worden eveneens hier getest.
		// Het returned of het rooster in dat opzichte valide is.
		bool nulOfTweeVak(int rooster[MaxNrTijdsloten][MaxNrZalen]);

		// Checkt of een docent maximaal één les geeft op een dag,
		// en returned een bool respectievelijk.
		bool lesDocent(Vak* vak, int rooster[MaxNrTijdsloten][MaxNrZalen],
								int tijdslot);

		// Checkt of een track maximaal 1 tussenuur heeft op een dag
		// en returned een bool respectievelijk.
		bool tussenuur(int rooster[MaxNrTijdsloten][MaxNrZalen]);

		// Bepaal zo mogelijk een rooster voor de verschillende tracks,
		// rekening houdend met de beschikbaarheid van de docenten,
		// en de eisen aan de tracks.
		// Retourneer:
		// * true, als het lukt om een rooster te bepalen
		// * false, als het niet lukt om een rooster te bepalen
		// Post:
		// * als het lukt om een rooster te bepalen, bevat parameter `rooster'
		//   zo'n rooster. Dan geldt: rooster[s][z] =
		//   - het nummer van het vak dat op tijdslot s (in de week) en zaal z
		//     is ingeroosterd
		//   - -1 als er geen vak op tijdslot s en zaal z is ingeroosterd
		// * aantalDeelroosters is gelijk aan het aantal deelroosters dat we
		//   hebben gezien bij het bepalen van een rooster
		bool bepaalRooster (int rooster[MaxNrTijdsloten][MaxNrZalen],
										long long &aantalDeelroosters);

		// Bepaal zo mogelijk een rooster voor de verschillende tracks,
		// rekening houdend met de beschikbaarheid van de docenten,
		// en de eisen aan de tracks. Als er een rooster te vinden is,
		// bepaal dan een zo kort mogelijk rooster (een rooster dat zo vroeg
		// mogelijk (qua tijdslot in de week) klaar is).
		// Retourneer:
		// * true, als het lukt om een rooster te bepalen
		// * false, als het niet lukt om een rooster te bepalen
		// Post:
		// * als het lukt om een rooster te bepalen, bevat parameter `rooster'
		//   een zo kort mogelijk rooster. Dan geldt: rooster[s][z] =
		//   - het nummer van het vak dat op tijdslot s (in de week) en zaal z
		//     is ingeroosterd
		//   - -1 als er geen vak op tijdslot s en zaal z is ingeroosterd
		// * aantalDeelroosters is gelijk aan het aantal deelroosters dat we
		//   hebben gezien bij het bepalen van een rooster
		bool bepaalMinRooster(int rooster[MaxNrTijdsloten][MaxNrZalen],
										long long &aantalDeelroosters);

		// Druk parameter rooster overzichtelijk af op het scherm
		// (dag, tijd, zaal, naam van vak, nummer van docent).
		// Pre:
		// * er is al een instantie ingelezen, en parameter rooster
		//   bevat een rooster voor die instantie
		void drukAfRooster(int rooster[MaxNrTijdsloten][MaxNrZalen]);


		// berekent de score van een potentiele inroostering
		int bepaalScore(int nrVak, int tijdslot, int zaal,
								int rooster[MaxNrTijdsloten][MaxNrZalen]);

		// Bepaal op een gretige manier een rooster voor de ingelezen vakken,
		// docenten en tracks. Als het niet lukt om aan alle eisen voor een
		// geldig rooster te voldoen, probeer er dan `zo goed mogelijk' aan
		// te voldoen.
		// Pre:
		// * het aantal vakken is <= het aantal combinaties (tijdslot,zaal),
		//   zodat daadwerkelijk alle vakken te geven zijn (los van onze andere,
		//   logische eisen aan een rooster)
		// Post:
		// * rooster bevat een rooster voor alle ingelezen vakken.
		void bepaalRoosterGretig (int rooster[MaxNrTijdsloten][MaxNrZalen]);
	private:
		Docent* docenten[40];	//Array van klassen van docenten (max 40)
		Vak* vakken[50];			//Array van klassen van vakken (max 50)
		Track* tracken[10];		//Array van klassen van tracken (max 10)

		// Een test rooster voor bepaalMinRooster().
		int manipRooster[MaxNrTijdsloten][MaxNrZalen];

		string invoernaam = ""; //Naam van de invoerfile

		int nrDagen,      // Aantal dagen in het rooster
			nrUrenPerDag,  // Aantal uren per dag
			nrZalen,       // Aantal beschikbare zalen
			nrDocenten,		// Aantal docenten
			nrVakken,		// Aantal vakken
			nrTracks;		// Aantal tracks

		// Geeft aan bij welk vak ingeroosterd moet worden in de recursie.
		int vakIndex = 0;

		// Geeft het laatste tijdslot aan in een minimaal rooster.
		int maxTijdslot;

		// Zet overal in de 2d array -1
		void maakRoosterLeeg(int rooster[MaxNrTijdsloten][MaxNrZalen]);

		// Returned het laatste tijdslot waarop dit rooster les heeft.
		int bepaalMaxTijdslot(int rooster[MaxNrTijdsloten][MaxNrZalen]);
}; //Rooster

#endif
