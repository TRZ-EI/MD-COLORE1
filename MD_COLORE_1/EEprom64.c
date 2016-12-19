#include "EEprom.h"

extern unsigned char vett_ee[];

//=========== subroutine per eeprom 25aa640  inizio ==================

// abilita la scrittura in memoria

void wrmem (void)
	{
	unsigned int i,cod_ist,cod_app;
	CKEE=0;
	WPE=1;
	DIEE=0;
	CSEE=0;
	cod_ist=06;
	
	for (i=0;i<8; i++)
		{
		cod_app=cod_ist & 0X80;
		if (cod_app ==0)
			DIEE=0;
			else
			DIEE=1;
			
		DIEE=DIEE;			// attesa
		DIEE=DIEE;			// attesa
		DIEE=DIEE;			// attesa

		CKEE=1;
		CKEE=1;
		CKEE=1;
		CKEE=0;
		cod_ist=cod_ist <<1;
		}
		WPE=0;
		CSEE=1;						// deseleziona
		
		}
		


// disabilita  la scrittura in memoria

void wrdis (void)
	{
	unsigned int i,cod_ist,cod_app;
	CKEE=0;
	WPE=0;
	DIEE=0;
	CSEE=0;
	cod_ist=04;
	
	for (i=0;i<8; i++)
		{
		cod_app=cod_ist & 0X80;
		if (cod_app ==0)
			DIEE=0;
			else
			DIEE=1;
			
		DIEE=DIEE;			// attesa
		DIEE=DIEE;			// attesa	
		DIEE=DIEE;			// attesa
		CKEE=1;
		CKEE=1;
		CKEE=1;
		CKEE=0;
		cod_ist=cod_ist <<1;
		}
		
		CSEE=1;						// deseleziona
		
		}
				
		
		
// scrivi un byte all'indirizzo passato


void scrivi_ee (unsigned int ind_ee, char dato_ee)
	{
	unsigned int f_ind_ee, cod_ist, cod_app, i;
	
	
	CKEE=0;
	WPE=1;
	DIEE=0;
	CSEE=0;
	cod_ist =02;
	
	// tx codice
	for (i=0;i<8; i++)
		{
		cod_app=cod_ist & 0X80;
		if (cod_app ==0)
			DIEE=0;
			else
			DIEE=1;
			
		DIEE=DIEE;			// attesa
		DIEE=DIEE;			// attesa	
		DIEE=DIEE;			// attesa
		CKEE=1;
		CKEE=1;
		CKEE=1;

		CKEE=0;
		cod_ist=cod_ist <<1;
		}
		
// tx indirizzo
		
	f_ind_ee=ind_ee;	
	for (i=0;i<16; i++)
		{
		f_ind_ee=ind_ee & 0x8000;
		if (f_ind_ee ==0)
			DIEE=0;
			else
			DIEE=1;
			
		DIEE=DIEE;			// attesa
		DIEE=DIEE;			// attesa

		CKEE=1;
		CKEE=1;

		CKEE=0;
		ind_ee=ind_ee <<1;
		}
		
// tx dato
cod_ist =dato_ee;		
		
for (i=0;i<8; i++)
		{
		cod_app=cod_ist & 0X80;
		if (cod_app ==0)
			DIEE=0;
			else
			DIEE=1;
			
		DIEE=DIEE;			// attesa
		DIEE=DIEE;			// attesa
		CKEE=1;
		CKEE=1;

		CKEE=0;
		cod_ist=cod_ist <<1;
		}
	CKEE=0;
	DIEE=0;
	CSEE=1;		// deseleziona memoria
	
	}	
	
	
// leggi un dato dalla memoria EEProm all'indirizzo passato

char leggi_ee(unsigned int ind_ee)
	{
	unsigned int f_ind_ee, cod_ist, cod_app, i,f_dato, app_dato;
		
	CKEE=0;
	WPE=1;
	DIEE=0;
	cod_ist =03;		// cod lettura dato
	CSEE=0;
	
		for (i=0;i<8; i++)
		{
		cod_app=cod_ist & 0X80;
		if (cod_app ==0)
			DIEE=0;
			else
			DIEE=1;
			
		DIEE=DIEE;			// attesa
		DIEE=DIEE;			// attesa
		DIEE=DIEE;			// attesa

		CKEE=1;
		CKEE=1;
		CKEE=1;

		CKEE=0;
		cod_ist=cod_ist <<1;
		}		

// tx indirizzo	

		
	f_ind_ee=ind_ee;	
	for (i=0;i<16; i++)
		{
		f_ind_ee=ind_ee & 0x8000;
		if (f_ind_ee ==0)
			DIEE=0;
			else
			DIEE=1;
		DIEE=DIEE;			// attesa			
		DIEE=DIEE;			// attesa
		CKEE=1;
		CKEE=1;

		CKEE=0;
		ind_ee=ind_ee <<1;
		}


// legge il dato dalla memoria

f_dato=0;
app_dato=0;

		for (i=0;i<8; i++)
		{
		f_dato=f_dato<<1;
		
		if (DOEE==1)
		f_dato=f_dato | 01;

		CKEE=1;
		CKEE=1;
		CKEE=0;
		CKEE=0;

		
		}
	CKEE=0;
	DIEE=0;
	CSEE=1;		// deseleziona memoria
		
			
		return (f_dato);
		
	}
		
			
// scrive 256 dati in memoria all'indirizzo passato 
//i dati sono in vett_ee
void scrivi_ee_256 (unsigned int ind_ee)
	{
	unsigned int k,att;
	unsigned char dato;
	
	wrmem();	
	for (k=0; k< 255; k++)
		{
		dato=vett_ee[k];
	
	
		scrivi_ee (ind_ee, vett_ee[k]);
		for (att=0; att<65000;att++)
			{
			att++;
			att--;
			}
		ind_ee++;
		}
	wrdis();
	}
	
	
	
	
// legge un blocco di dati a partire dall'ind. passato
// e lo mette in vett_ee

void leggi_ee_256 (unsigned int ind_ee)
	{
	unsigned int k;
	unsigned char dato;



	wrmem();
	for (k=0; k< 255; k++)
		{

		vett_ee[k]=leggi_ee(ind_ee);
		dato=vett_ee[k];
		ind_ee++;
		}
	wrdis();
	}

	













// scrivi un blocco di 256 dati residenti in vett_ee al numero di blocco passato
// la scrittura viene ripetuta 4 volte il blocco effettivo sono 256 byte

void scrivi_ee_bloc(unsigned int bloc_ee)
	{
	unsigned int f_ind_ee,ind_ee, cod_ist, cod_app, i,nn;
	char  z,registro;
	unsigned int bl, ind_vett;
	
	
	DI();
	ind_vett=0;
	CKEE=0;
	WPE=1;
	DIEE=0;
	CSEE=0;
	cod_ist =02;
	
	
for (bl=0; bl <8; bl++)
	{	
	
		// tx codice
		for (i=0;i<8; i++)
			{
			cod_app=cod_ist & 0X80;
			if (cod_app ==0)
				DIEE=0;
				else
				DIEE=1;
			
			DIEE=DIEE;			// attesa
			CKEE=1;
		//	CKEE=1;
			CKEE=0;
			cod_ist=cod_ist <<1;
			}
		
	// tx indirizzo
		ind_ee=(256 * bloc_ee) + (32 * bl);	// non capisco il 32 *bl
								// tolto +32 7/01/07
		f_ind_ee=ind_ee;	
			for (i=0;i<16; i++)
				{
				f_ind_ee=ind_ee & 0x8000;
				if (f_ind_ee ==0)
					DIEE=0;
					else
					DIEE=1;
			
					DIEE=DIEE;			// attesa
				//	CKEE=1;
					CKEE=1;
					CKEE=0;
					ind_ee=ind_ee <<1;
				}
		
			z=0; 			// iniz contatore		
		
	// tx dato

	for (z=0; z<32; z++)
		{

		cod_ist =vett_ee[ind_vett];		
		ind_vett++;
		for (i=0;i<8; i++)
				{
				cod_app=cod_ist & 0X80;
				if (cod_app ==0)
				DIEE=0;
				else
				DIEE=1;
			
				DIEE=DIEE;			// attesa
				CKEE=1;
			//	CKEE=1;
				CKEE=0;
				cod_ist=cod_ist <<1;
				}
		}
	CSEE=1;
	CSEE=1;
	
// attendi la fine della scrittura del blocco

//	do
//	{
//	registro=leggi_stato_ee();
//	registro=registro &0x80;
//	}
//	while (1);		// attendi fine scrittura




for (nn=0; nn<10000; nn++)
	{}	
		
	wrmem();			// abilita la scrittura
	CSEE=0;
	cod_ist=02;
	}
			CSEE=1;		// deseleziona memoria
			
			
	EI();
	}	
	



	
// leggi un blocco di 32 *8 dati dal blocco passato e lo mette in vett_ee


void  leggi_ee_bloc(unsigned int bloc_ee)
	{
	unsigned int f_ind_ee, ind_ee,cod_ist, cod_app, i,f_dato, app_dato;
	unsigned int zz;
	
	
	DI();
	CKEE=0;
	WPE=1;
	DIEE=0;
	cod_ist =03;		// cod lettura dato
	CSEE=0;
	
		for (i=0;i<8; i++)
		{
		cod_app=cod_ist & 0X80;
		if (cod_app ==0)
			DIEE=0;
			else
			DIEE=1;
			
		DIEE=DIEE;			// attesa
		CKEE=1;
		CKEE=1;
		CKEE=1;
		CKEE=0;
		CKEE=0;
		cod_ist=cod_ist <<1;
		}		

// tx indirizzo	

	ind_ee = bloc_ee *256;	
	f_ind_ee=ind_ee;	
	for (i=0;i<16; i++)
		{
		f_ind_ee=ind_ee & 0x8000;
		if (f_ind_ee ==0)
			DIEE=0;
			else
			DIEE=1;
			
		DIEE=DIEE;			// attesa
		CKEE=1;
		CKEE=1;
		CKEE=1;
		CKEE=0;
		CKEE=0;
		ind_ee=ind_ee <<1;
		}


// legge il dato dalla memoria




for (zz=0; zz<255; zz++)
	{
	f_dato=0;
	app_dato=0;

			for (i=0;i<8; i++)
			{
			f_dato=f_dato<<1;
			CKEE=1;
			CKEE=1;
			CKEE=1;
			CKEE=1;
			if (DOEE==1)
				f_dato=f_dato | 01;
			CKEE=0;
			CKEE=0;
			}
			vett_ee[zz]=f_dato;
		}	
			
		CSEE=1;				// deseleziona
		
	
	EI();
	}
		
			

//  +++++++++++++ fine lettura eeprom 25aa640++++++++++++++++


