#include "EEprom.h"

extern unsigned char vett_ee[];

//=========== subroutine per eeprom 25LC512  inizio ==================

// Scrivi un byte

void scrivi_ee_dato (char dato) {
	unsigned short i = 0x80;

	CKEE = 0;

	for (; i > 0; i >>= 1) {
		if (dato & i) DIEE = 1;
		else DIEE = 0;

		asm("nop");
		asm("nop");
		CKEE = 1;
		asm("nop");
		asm("nop");

		CKEE = 0;
	}
}

// Leggi un byte

char leggi_ee_dato (void) {
	unsigned short i = 8;
	char dato;

	CKEE = 0;

	for (; i > 0; i--) {
		asm("nop");
		CKEE = 1;
		asm("nop");
		asm("nop");
		dato <<= 1;
		if (DOEE) dato |= 1;
		CKEE = 0;
	}

	return dato;
}

// abilita la scrittura in memoria

void wrmem (void) {
	CSEE = 0;
	WPE  = 1;

	scrivi_ee_dato(6);

	CSEE = 1;
}

// disabilita  la scrittura in memoria

void wrdis (void) {
	CSEE = 0;

	scrivi_ee_dato(4);

	WPE  = 0;
	CSEE = 1;
}

// scrivi un byte all'indirizzo passato

void scrivi_ee (unsigned int ind_ee, char dato_ee) {
	unsigned long delay;

	wrmem();
	CSEE = 0;

	// Comando "Write"
	scrivi_ee_dato(2);
	// Scrittura indirizzo
	scrivi_ee_dato((char) (ind_ee >> 8));
	scrivi_ee_dato((char) (ind_ee & 0xFF));
	// Scrittura dato
	scrivi_ee_dato(dato_ee);
	
	CSEE = 1;
	for (delay=0; delay<20000; delay++);
	wrdis();
	for (delay=0; delay<20000; delay++);
}
	
// leggi un dato dalla memoria EEProm all'indirizzo passato

char leggi_ee(unsigned int ind_ee) {
	char dato;

	CSEE = 0;

	// Comando "Read"
	scrivi_ee_dato(3);
	// Scrittura indirizzo
	scrivi_ee_dato((char) (ind_ee >> 8));
	scrivi_ee_dato((char) (ind_ee & 0xFF));
	// Lettura dato
	dato = leggi_ee_dato();
	
	CSEE = 1;
	return dato;
}
		
			
// scrive 256 dati in memoria all'indirizzo passato 
//i dati sono in vett_ee
/*
void scrivi_ee_256 (unsigned int ind_ee) {
}
*/	
// legge un blocco di dati a partire dall'ind. passato
// e lo mette in vett_ee
/*
void leggi_ee_256 (unsigned int ind_ee) {
}
*/

// scrivi un blocco di 256 dati residenti in vett_ee al numero di blocco passato

void scrivi_ee_bloc(unsigned int bloc_ee) {
	const unsigned int page_size = 128;
	unsigned int pages = 2; // pages = 256 / page_size
	unsigned int current_page, indirizzo;

	unsigned int index = 0, j;
	unsigned long delay;
//	char dato, fine_scrittura = 0;

	DI();
	// wrmem();

	for (current_page = 0; current_page < pages; current_page++) {
		wrmem();
		CSEE = 0;
		indirizzo = (bloc_ee * 256) + (current_page * page_size);

		// Comando "Write"
		scrivi_ee_dato(2);
		// Scrittura indirizzo
		scrivi_ee_dato((char) (indirizzo >> 8));
		scrivi_ee_dato((char) (indirizzo & 0x00FF));
		// Scrittura dati
		for (j = page_size; j > 0; j--) {
			scrivi_ee_dato(vett_ee[index]);
			index++;
		}

		// Scrittura blocco
		CSEE = 1;

		for (delay=0; delay<20000; delay++);
		wrdis();
		for (delay=0; delay<20000; delay++);

/*		do {
			// Leggi STATUS register
			scrivi_ee_dato(5);
			dato = leggi_ee_dato();
			fine_scrittura = dato & 1;
		} while (!fine_scrittura);*/
	}

	// wrdis();
	EI();
}

	
// leggi un blocco di 256 dati dal blocco passato e lo mette in vett_ee

void  leggi_ee_bloc(unsigned int bloc_ee) {
	unsigned int index;

	DI();
	CSEE = 0;

	// Comando "Read"
	scrivi_ee_dato(3);
	// Scrittura indirizzo
	scrivi_ee_dato((char) (bloc_ee & 0xFF));
	scrivi_ee_dato(0);
	// Lettura dati
	for (index = 0; index < 256; index++)
		vett_ee[index] = leggi_ee_dato();

	CSEE = 1;
	EI();
}

