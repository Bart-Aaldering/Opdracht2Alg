// Definitie van klasse constantes
// constantes.h
// Geschreven door : Bart Aaldering (s2969866) en Lex Janssens (s2989344)
// Compiler: GNU GCC Compiler
// Beschrijving:
/*
	Enkele constantes die op verschillende plekken gebruikt kunnen worden.
*/
#ifndef ConstantesHVar  // voorkom dat dit bestand meerdere keren
#define ConstantesHVar  // ge-include wordt

// Maximaal aantal tijdsloten
const int MaxNrTijdsloten = 50;

// Maximaal aantal tracks
const int MaxNrTracks = 10;

// Maximaal aantal docenten
const int MaxNrDocenten = 40;

// Maximaal aantal vakken
const int MaxNrVakken = 50;

// Maximaal aantal zalen
const int MaxNrZalen = 5;

// Maakt een rooster compact door lege dagen niet af te drukkenn
// Wanneer deze constante op true staat, wordt er bij een gevonden rooster
// enkel de rijen afgedrukt die een vak bevatten. Dit maakt het lezen van
// het rooster overzichtelijker, door alle uren waar geen les op wordt
// gegeven eruit te filteren. False drukt het gehele rooster af.
const bool CompactRooster = true;

// Defineert de lengte van het vak en tracks,
// zodat het rooster netjes uitgeprint kan worden.
// Door deze variabele langer te maken, heeft elk blokje bij het uitprinten
// meer ruimte.
const int LengteNaamVak = 30;

#endif
