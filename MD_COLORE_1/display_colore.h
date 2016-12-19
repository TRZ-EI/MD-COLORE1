
// inizio sviluppo strumento con display a colore

#define fine_stringa 0x0a

int timer_colore;				// per visualizz display colori

char tab_hex[17][2]= {0,0x30,1,0x31,0x32,'2',0x33,'3',4,0x34,5,0x35,6,0x36,7,0x37,8,0x38,9,0x39,10,'A',11,'B',
						12,'C',13,'D',14,'E',16,'F'};
unsigned int acqNumero_colore (unsigned int numero_iniziale, unsigned int max, int num_caratteri,unsigned int virgola);
unsigned int acqso_colore (unsigned int SOGLIA, int riga,int colonna,char num_caratteri,char num_decimali);
int test_lunghezza(void);			// testa lunghezza del buf_asci
int crc_display;						
char vett_hex32[16];
void long_hex (long numero);
void int_hex (int numero);
void char_hex (char numero);
int calcola_crc(void);
void config_variabile1(char num_var1, char font_colore1,char num_caratteri1,char decimali1,char riga1, char colonna1);

void config_variabile4(char num_var1, char font_colore1,char num_caratteri1,char decimali1,char riga1, char colonna1,
			char num_var2, char font_colore2,char num_caratteri2,char decimali2,char riga2, char colonna2,
			char num_var3, char font_colore3,char num_caratteri3,char decimali3,char riga3, char colonna3,
			char num_var4, char font_colore4,char num_caratteri4,char decimali4,char riga4, char colonna4);

void tx_valore_variabile1(char num_var1, long val_var1);
void tx_valore_variabile4(char num_var1, long val_var1, char num_var2,long val_var2, char num_var3,long val_var3,char num_var4,long val_var4 );
void tx_valore_variabile4_a(char num_var1, long val_var1, char num_var2,long val_var2, char num_var3,long val_var3,char num_var4,long val_var4 );

void tx_stringa_comandi(void);
void clear_vett_config( void);
void clear_vett_config( void);
void trasmissione_testo(char font_colore,char riga,int colonna, far char msg[60]);
void config_barra(char num_var,int min,int max,int riga);
void clear_display(void);
void fine_config_variabile (void);
void trasmissione_prova_testo (far char msg[60]);

void inserisce_char (void);
void inserisce_int (void);
void inserisce_long (void);
void iniz_vett_config (void);
char vett_config[80];
char *punt_vett_config;
long var_prova=0;
long attesa_int;				// per ritardo che gira sotto intterrupt





// converte il numero passato in esadecimale residente in vett_hex con la cifra + signif in vett_hex[0]

void long_hex (long numero){
char 	*punt_hex32;
char i,app, app_v;
long num_int;
		punt_hex32=&(vett_hex32[7]);			// inizializza puntatore all'ultimo elemento
		for (i=0;i<8;i++){					// azzera VETTORE
			vett_hex32[i]=0;
		}
		
		for (i=0; i<4;i++){
			num_int=(char)(numero & 0xFF);
			*punt_hex32=tab_hex[num_int &0x0F][1];
			punt_hex32--;
			num_int =num_int >>4;
			*punt_hex32=tab_hex[num_int &0x0F][1];
			punt_hex32--;
			numero=numero >>8;
			
		}
			
}
	



// converte il numero passato in esadecimale residente in vett_hex con la cifra + signif in vett_hex[0]

void int_hex (int numero){
char 	*punt_hex32;
char i,app, app_v;
int num_int;
		punt_hex32=&(vett_hex32[3]);			// inizializza puntatore all'ultimo elemento
		for (i=0;i<4;i++){					// azzera VETTORE
			vett_hex32[i]=0;
		}
		
	
		
			num_int=(char)(numero & 0x0F);
			*punt_hex32=tab_hex[num_int &0x0F][1];
			punt_hex32--;
			
			numero=numero >> 4;				// schifta  4 bit
			
			num_int=(char)(numero & 0xFF);
			*punt_hex32=tab_hex[num_int &0x0F][1];
			punt_hex32--;					// fatto byte meno significativo
			
			numero=numero >> 4;			
			num_int=(char)(numero &0x0F);
			*punt_hex32=tab_hex[num_int  & 0x0F][1];
			punt_hex32--;
			
			numero=numero >> 4;			
			num_int=(char)(numero &0x0F);
			*punt_hex32=tab_hex[num_int  & 0x0F][1];
			punt_hex32--;					// fatto byte piu' significativo
			
	
			
}


// converte il numero passato in esadecimale residente in vett_hex con la cifra + signif in vett_hex[0]

void char_hex (char numero){
char 	*punt_hex32;
char i,app, app_v;
int num_int;
		punt_hex32=&(vett_hex32[1]);			// inizializza puntatore all'ultimo elemento
		vett_hex32[0]=0;						// azzera vettore
		vett_hex32[1]=0;
		
		vett_hex32[0]=tab_hex[numero & 0x0F][1];
		numero =numero >>4;
		vett_hex32[1]=tab_hex[numero & 0x0F][1];			
}
	
// inserisce un char il dato e' in vett_hex

void inserisce_char (void){
	char app;
	
		*punt_vett_config=vett_hex32[1];
		*punt_vett_config++;
		*punt_vett_config=vett_hex32[0];
		*punt_vett_config++;
}
// inserisce nel vettore di trasmissione una variabile di tipo int che si trova in vett_hex32

 void inserisce_int (void){
	int i;
	for (i=0;i<4;i++){
		*punt_vett_config=vett_hex32[i];
		*punt_vett_config++;
	}
}
		


// inserisce nel vettore di trasmissione una variabile di tipo long che si trova in vett_hex32

void inserisce_long (void){
	int i;
	for (i=0;i<8;i++){
		*punt_vett_config=vett_hex32[i];
		*punt_vett_config++;
	}
}




// calcola il CRC della stringa residente nel vettore di configurazione usando M16

int calcola_crc(void){
	int l,i, crc;
	char app;
	l=strlen(vett_config);				// calcola la lunghezza del vettore
//	crc=CRC16(&vett_config[0],l);
	crcd=0;
	for (i=0; i<l; i++){
		app=vett_config[i];
		crcin=app;
	}
	crc=crcd;
	return(crc);
}
	


// configura una  4 variabili partendo dall'inizio

void config_variabile4(char num_var1, char font_colore1,char num_caratteri1,char decimali1,char riga1, char colonna1,
			char num_var2, char font_colore2,char num_caratteri2,char decimali2,char riga2, char colonna2,
			char num_var3, char font_colore3,char num_caratteri3,char decimali3,char riga3, char colonna3,
			char num_var4, char font_colore4,char num_caratteri4,char decimali4,char riga4, char colonna4){
	
	char i, app;
	iniz_vett_config();
	
	char_hex(num_var1);
	inserisce_char();
	
	*punt_vett_config=tab_hex[font_colore1 & 0x0F][1];	// inserisce font_colore
	punt_vett_config++;
	
	*punt_vett_config=tab_hex[num_caratteri1 & 0x0F][1];	// inserisce numero caratteri
	*punt_vett_config++;
	
	*punt_vett_config=tab_hex[decimali1 & 0x0F][1];	// inserisce numero decimali
	*punt_vett_config++;
	
	char_hex(riga1);								//converte colonna in Hex
	inserisce_char();								//inserisce la varabile colonna
	
	char_hex(colonna1);								//converte colonna in Hex
	inserisce_char();								//inserisce la varabile colonna
	
	if (num_var2== 0)
		goto fine_c1;
	char_hex(num_var2);
	inserisce_char();
	
	*punt_vett_config=tab_hex[font_colore2 & 0x0F][1];	// inserisce font_colore
	punt_vett_config++;
	
	*punt_vett_config=tab_hex[num_caratteri2 & 0x0F][1];	// inserisce numero caratteri
	*punt_vett_config++;
	
	*punt_vett_config=tab_hex[decimali2 & 0x0F][1];	// inserisce numero decimali
	*punt_vett_config++;
	
	char_hex(riga2);								//converte colonna in Hex
	inserisce_char();								//inserisce la varabile colonna
	
	char_hex(colonna2);								//converte colonna in Hex
	inserisce_char();								//inserisce la varabile colonna
	
	if (num_var3== 0)
		goto fine_c1;
	char_hex(num_var3);
	inserisce_char();
	
	*punt_vett_config=tab_hex[font_colore3 & 0x0F][1];	// inserisce font_colore
	punt_vett_config++;
	
	*punt_vett_config=tab_hex[num_caratteri3 & 0x0F][1];	// inserisce numero caratteri
	*punt_vett_config++;
	
	*punt_vett_config=tab_hex[decimali3 & 0x0F][1];	// inserisce numero decimali
	*punt_vett_config++;
	
	char_hex(riga3);								//converte colonna in Hex
	inserisce_char();								//inserisce la varabile colonna
	
	char_hex(colonna3);								//converte colonna in Hex
	inserisce_char();								//inserisce la varabile colonna
	

	if (num_var4== 0)
		goto fine_c1;
	char_hex(num_var4);
	inserisce_char();
	
	*punt_vett_config=tab_hex[font_colore4 & 0x0F][1];	// inserisce font_colore
	punt_vett_config++;
	
	*punt_vett_config=tab_hex[num_caratteri4 & 0x0F][1];	// inserisce numero caratteri
	*punt_vett_config++;
	
	*punt_vett_config=tab_hex[decimali4 & 0x0F][1];	// inserisce numero decimali
	*punt_vett_config++;
	
	char_hex(riga4);								//converte colonna in Hex
	inserisce_char();								//inserisce la varabile colonna
	
	char_hex(colonna4);								//converte colonna in Hex
	inserisce_char();								//inserisce la varabile colonna
	
	fine_c1:
	fine_config_variabile();						// fine configurazione variabile e trasmissione
}

// configura una  1 variabili partendo dall'inizio

void config_variabile1(char num_var1, char font_colore1,char num_caratteri1,char decimali1,char riga1, char colonna1){
	
	char i, app;
	iniz_vett_config();
	
	char_hex(num_var1);
	inserisce_char();
	
	*punt_vett_config=tab_hex[font_colore1 & 0x0F][1];	// inserisce font_colore
	punt_vett_config++;
	
	*punt_vett_config=tab_hex[num_caratteri1 & 0x0F][1];	// inserisce numero caratteri
	*punt_vett_config++;
	
	*punt_vett_config=tab_hex[decimali1 & 0x0F][1];	// inserisce numero decimali
	*punt_vett_config++;
	
//	*punt_vett_config=tab_hex [(riga1  & 0x0F)][1];	// inserisce riga
//	*punt_vett_config++;
	char_hex(riga1);								//converte colonna in Hex
	inserisce_char();								//inserisce la varabile colonna

	char_hex(colonna1);								//converte colonna in Hex
	inserisce_char();								//inserisce la varabile colonna
	
	fine_config_variabile();						// fine configurazione variabile e trasmissione
}


	
	
	
	
	
// trasmette la stringa che si trova in vett_config

void tx_stringa_comandi(void){
	int l,i;
		l=strlen(vett_config);
	for (i=0;i<l;i++){
		UART0Txdato(vett_config[i]);
	}
}
	

void Delay(long attesa){
	attesa_int =attesa/2;
 	while (attesa_int >0)
		{}
}	




//inizializza il vettore per trasmissione  variabili

void iniz_vett_tx_variabile (void){
		char i, app;
	
	punt_vett_config=&(vett_config[0]);			// inizializza puntatore 
	for (i=0; i<60;i++){						// azzera vettore conficurazione
		vett_config[i]=0;	
	}
	*punt_vett_config=0x5E;						// inserisce sincronismo
	punt_vett_config++;
	*punt_vett_config='v';						// inserisce comando v
	punt_vett_config++;
}



// configura e trasmette 1  variabile partendo dall'inizio 11

void tx_valore_variabile1(char num_var1, long val_var1){
	char i, app;
	int crc;
	
	clear_vett_config();
	iniz_vett_tx_variabile();
	punt_vett_config=&(vett_config[0]);
	*punt_vett_config=0x5E;						// inserisce sincronismo
	punt_vett_config++;
	*punt_vett_config='v';						// inserisce sincronismo
	punt_vett_config++;	
	char_hex(num_var1);
	inserisce_char();								// inserisce numero var 1
	long_hex(val_var1);								
	inserisce_long();								// inserisce valore variabile 1		
	crc=calcola_crc();
	int_hex(crc);
	inserisce_int();
	*punt_vett_config=fine_stringa;	
	tx_stringa_comandi();	
}



// configura e trasmette 4  variabili partendo dall'inizio

void tx_valore_variabile4(char num_var1, long val_var1, char num_var2,long val_var2, char num_var3,long val_var3,char num_var4,long val_var4 ){
	
	char i, app;
	int crc;
	clear_vett_config();
	iniz_vett_tx_variabile();
	punt_vett_config=&(vett_config[0]);
	*punt_vett_config=0x5E;						// inserisce sincronismo
	punt_vett_config++;
	*punt_vett_config='v';						// inserisce sincronismo
	punt_vett_config++;	
	char_hex(num_var1);
	inserisce_char();								// inserisce numero var 1
	long_hex(val_var1);								
	inserisce_long();								// inserisce valore variabile 1
	
	if (num_var2 ==0)
		goto fine_inserimento;
	
	char_hex(num_var2);
	inserisce_char();								// inserisce numero var 2
	long_hex(val_var2);								
	inserisce_long();								// inserisce valore variabile 2
	
	if (num_var3 ==0)
	goto fine_inserimento;
	
	char_hex(num_var3);
	inserisce_char();								// inserisce numero var 3
	long_hex(val_var3);								
	inserisce_long();								// inserisce valore variabile 3

	if (num_var4 ==0)
	goto fine_inserimento;
	
	char_hex(num_var4);
	inserisce_char();								// inserisce numero var 3
	long_hex(val_var4);								
	inserisce_long();								// inserisce valore variabile 3
	
fine_inserimento:	
		
	crc=calcola_crc();
	int_hex(crc);
	inserisce_int();
	*punt_vett_config=fine_stringa;	
	tx_stringa_comandi();
	
		
	
}



// azzera il vettore di congigurazione
void clear_vett_config( void){
	int i;
	
	punt_vett_config=&(vett_config[0]);			// inizializza puntatore 
	for (i=0; i<80;i++){						// azzera vettore conficurazione
		vett_config[i]=0;
	}	
}





// visualizzazione testo

void trasmissione_testo (char font_colore,char riga,int colonna, far char msg[60]){
	char i, app;
	int crc;
	clear_vett_config();
	
	punt_vett_config=&(vett_config[0]);
	*punt_vett_config=0x5E;						// inserisce sincronismo
	punt_vett_config++;
	*punt_vett_config='t';						// inserisce comando visual testo
	punt_vett_config++;	
//	char_hex(font_colore);						// inserisce font + colcore
//	inserisce_char();
	*punt_vett_config=tab_hex[font_colore & 0x0F][1];	// inserisce numero decimali
	*punt_vett_config++;
	char_hex(riga);								// inserisce riga
	inserisce_char();	
	char_hex(colonna);								// inserisce colonna
	inserisce_char();
	for (i=0;(i<60 && msg[i] !=0);i++){
		*punt_vett_config=msg[i];
		punt_vett_config++;	
	}
	crc=calcola_crc();
	int_hex(crc);
	inserisce_int();
	*punt_vett_config=fine_stringa;
	tx_stringa_comandi();
}
void trasmissione_prova_testo (far char msg[60]){
	char i, app;
	int crc;
	clear_vett_config();
	
	punt_vett_config=&(vett_config[0]);
	for (i=0;(i<60 && msg[i] !=0);i++){
		*punt_vett_config=msg[i];
		punt_vett_config++;	
	}
	*punt_vett_config=fine_stringa;
	tx_stringa_comandi();
}	
// azzera il didplay

void clear_display(void){	
	int crc;
	clear_vett_config();
	
	punt_vett_config=&(vett_config[0]);
	*punt_vett_config=0x5E;						// inserisce sincronismo
	punt_vett_config++;
	*punt_vett_config='C';						// inserisce comando di clear C
	punt_vett_config++;
	crc=calcola_crc();
	int_hex(crc);
	inserisce_int();
	*punt_vett_config=fine_stringa;
	tx_stringa_comandi();	
}

// configurazione barra

void config_barra(char num_var,int min,int max,int riga){
	int crc, app;
	clear_vett_config();
	
	punt_vett_config=&(vett_config[0]);
	*punt_vett_config=0x5E;						// inserisce sincronismo
	punt_vett_config++;
	*punt_vett_config='B';						// inserisce comando di clear C
	punt_vett_config++;
	
	char_hex(num_var);
	inserisce_char();							// inserisce numero variabile
	
	long_hex(min);
	inserisce_long();							// inserisce valore minimo	
	
	long_hex(max);
	inserisce_long();							// inserisce valore massimo

	char_hex(riga);
	inserisce_char();							// inserisce valore riga
	
	crc=calcola_crc();
	int_hex(crc);
	inserisce_int();
	*punt_vett_config=fine_stringa;
	tx_stringa_comandi();
}



//inizializza il vettore per configurazioni variabili

void iniz_vett_config (void){
		char i, app;
	
	clear_vett_config();
	*punt_vett_config=0x5E;						// inserisce sincronismo
	punt_vett_config++;
	*punt_vett_config='V';						// inserisce comando V
	punt_vett_config++;
}



//fine configurazione variabili

void fine_config_variabile (void){
		int crc_display;
		crc_display=calcola_crc();
		int_hex(crc_display);
		inserisce_int ();					// inserisce CRC
		*punt_vett_config=fine_stringa;				// inserisce fine stringa
		tx_stringa_comandi();
}	






// configura e trasmette 4 di tipo_int variabili partendo dall'inizio

void tx_valore_variabile4_a(char num_var1, long val_var1, char num_var2,long val_var2, char num_var3,long val_var3,char num_var4,long val_var4 ){
	
	char i, app;
	int crc;
	clear_vett_config();
	iniz_vett_tx_variabile();
	punt_vett_config=&(vett_config[0]);
	*punt_vett_config=0x5E;						// inserisce sincronismo
	punt_vett_config++;
	*punt_vett_config='v';						// inserisce sincronismo
	punt_vett_config++;	
	
	char_hex(num_var1);
	inserisce_char();								// inserisce numero var 1
	int_hex(val_var1);								
	inserisce_int();								// inserisce valore variabile 1
	
	if (num_var2 ==0)
		goto fine_inserimento_a;
	
	char_hex(num_var2);
	inserisce_char();								// inserisce numero var 2
	int_hex(val_var2);								
	inserisce_int();								// inserisce valore variabile 2
	
	if (num_var3 ==0)
	goto fine_inserimento_a;
	
	char_hex(num_var3);
	inserisce_char();								// inserisce numero var 3
	int_hex(val_var3);								
	inserisce_int();								// inserisce valore variabile 3


	if (num_var4 ==0)
	goto fine_inserimento_a;
	
	char_hex(num_var3);
	inserisce_char();								// inserisce numero var 3
	int_hex(val_var3);								
	inserisce_int();								// inserisce valore variabile 3
	
fine_inserimento_a:	
		
	crc=calcola_crc();
	int_hex(crc);
	inserisce_int();
	*punt_vett_config=fine_stringa;	
	tx_stringa_comandi();
	
		
	
}



// sabroutine per aquisire una sogglia senza segno , valore max 6000
// (non attiva) . visualizza all'indirizzo passato, cifra meno
//significativa, la soglia viene memorizzata in binario,
//  ritorna la soglia

unsigned int acqso_colore (unsigned int SOGLIA, int riga,int colonna,char num_caratteri,char num_decimali)
	{
	unsigned char  lung,j,k, ft ;
	config_variabile1(1,2,num_caratteri,num_decimali,riga,colonna);
	ft=1;
	FTAS=0;
	VTAS=0;
ril_soglia_colore:
	if( SOGLIA > 32000)
		SOGLIA=0;
		else
		{}
	for (j=0;j<6;j++)
		buf_asci[j]=' ';	
	mitoa (SOGLIA);
	for (k=0;k<7;k++){
	if (buf_asci[k] >='0' && buf_asci[k] <='9')
			{}
		else{
			buf_asci[k]=' ';
		}
				}
	lung =strlen(&buf_asci[0]);					// CALCOLA  IL NUMERO DI
												// CARATTERI
	
	// azzera il buffer display
	for (j=0; j<5 ;j++)
		{
		ind_wr=(i-j);
		wr_car_l(' ');
		}
		tx_valore_variabile1(1,(long)SOGLIA);

	for (j=0; ((j<4) && (buf_asci[j])!=0);j++)
		{
		ind_wr=i-(lung-1)+j;
		wr_car_l(buf_asci[j]);			// visualizza soglia
		}
		
		
   ft=1;
lop_tast_colore:
		if(bit_i (FTAS,TPIG)==0)			// testa se pigiato un tasto
		goto lop_tast_colore;
		if (VTAS==F4)
			return(SOGLIA);
		if (VTAS== F3)
			return(SOGLIA);
			if ( VTAS==ENT)
				{
				VTAS=0;
				FTAS=0;					// azzera tastiera
				for (k=0;k<7;k++){
					if (buf_asci[k] >='0' && buf_asci[k] <='9')
						{}
						else{
							buf_asci[k]=' ';
						}
				}					
			SOGLIA = atoi ( &buf_asci[0]);
			if (SOGLIA > 32000)
				{
				SOGLIA=0;
				goto ril_soglia_colore;
			}
				else
				{
				return (SOGLIA);
			}
				}
				else
					{
					if (VTAS<=9)

						{
						// testa il flag ft che indica che e' gia' stato
						// pigiato un tasto numerico
						if (ft==0)
							{
							for ( k=0; k<=5; k++)
						   buf_asci[k] = ' ';			// azzera buf 
						   ft=1;							// setta il flag ft
						   }

						   else{}
						// schifta in avanti il buffer
				
						for ( k=0; k<5; k++)
						buf_asci[k] = buf_asci[k+1];
							
						// memorizza il tasto
						

						// visualizza la soglia
// inserisce spazi nel buf_asci
	
						for (k=0;k<7;k++){
							if (buf_asci[k] >='0' && buf_asci[k] <='9')
								{}
								else{
									buf_asci[k]=' ';
								}
						}
						
						SOGLIA = atoi ( &buf_asci[0]);
						if (SOGLIA >32000)
							SOGLIA=0;
						tx_valore_variabile1(1,(long)SOGLIA);
	/*					for (j=0; (j< 4  &(buf_asci[j])!=0); j++)
							{
							ind_wr=(i-3+j);
							wr_car_l(buf_asci[j]);
							}
	*/
						VTAS=0;
						FTAS=0;
						}
						else{}
													// chiude il primo else
					}								// chiude il secondo else
					goto  lop_tast_colore;
	} 												// chiude funz.

// ================== fine aqso colore ==========================
	



unsigned int acqNumero_colore (unsigned int numero_iniziale, unsigned int max, int num_caratteri,unsigned int virgola) {
    unsigned char i, j, lunghezza, cifra;
    unsigned long numero = (unsigned long) numero_iniziale;
	unsigned int numero_vis;
	unsigned int posizione=20;
    VTAS = FTAS = 0;
	clear_display();
	config_variabile1(1,4,num_caratteri,virgola,10,8);
    while(1) {

    // Conversione numero -> ASCII
    mitoa((long) (numero));
    lunghezza = strlen(&buf_asci[0]);
    j = 5 - lunghezza;
    if (j) {
        for (i = lunghezza; i > 0;) {
	    i--;
	    buf_asci[i + j] = buf_asci[i];
	}
	for (; j > 0;)
	    buf_asci[--j] = '0';
    }

    // Visualizza numero
	numero=atol(&buf_asci[0]);
	tx_valore_variabile1(1,numero);
	
    ind_wr = posizione - 5;
    cifra = 0;
    for (i = 0; i <= 5; i++) {
        if (virgola + i < 5) {   // Cifre prima della virgola
	    if (buf_asci[i] == '0') {
		if (!cifra) {
				if ((i == 4) && !virgola)
						// Se il numero e' intero, visualizza lo 0
						{}
//				    wr_car_l(buf_asci[i]);
				else
				{}
//	            	wr_car_l(' ');
		    continue;
		}
	    }
//	    wr_car_l(buf_asci[i]);
	    cifra++;
	}
        if ((virgola + i == 5) && virgola)    // Posizione della virgola (solo se il numero non e' intero)
 			{}
 //           wr_car_l('.');
	if (virgola + i > 5) 
		   // Cifre dopo la virgola
		   {}
//	    wr_car_l(buf_asci[i - 1]);
    }

    while ((FTAS &01) == 0);     // Attesa tasto
    if (VTAS == F4) return (numero_iniziale);     // Annulla
	if (VTAS == F3) return (numero_iniziale);     // Annulla
    if (VTAS <= 9) {
		numero = numero - ((unsigned long) (buf_asci[0] - 0x30)) * 10000;  // Eliminazione della cifra più significativa * 10^5
        numero = (numero * 10) + (VTAS);   // Aggiunta dell'ultima cifra
        VTAS = 0;
        FTAS = 0;
    }
    if (VTAS == ENT) {
        VTAS = 0;
        FTAS = 0;
	if (numero <= max) return ((unsigned int) numero);       // Se il numero immesso è inferiore al massimo, ritorna
	numero = numero_iniziale;                 // Altrimenti azzera
    }

    }   // Chiusura while
	return ((unsigned int) numero);
}

