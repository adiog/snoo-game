#include "lpc2103.h"
#include "hardware.h"
#include "config.h"




/*

If a pin is selected as an External Interrupt, you cannot read the state of the
pin. You have to change the Pin Function Select Register setting to GPIO in 
order to read the state (or tie the signal to an additional GPIO pin). 



*/


#define KW(a) ((a)*(a))



unsigned int i, j, k, t[3][ILEPOM], sampling=0, connected=0, ile=0, linn=0;

unsigned char s[24], c, have_to_halt;
unsigned int wynik[3];
unsigned int probki[N_PROB][4];

unsigned char bufc, buf[256];

unsigned char msg[8];

/*----------------------------------------------------------*\
 | Delay ns                                                 |
\*----------------------------------------------------------*/
void DelayNS (unsigned int dly) {
	unsigned int i;
	
	for ( ; dly>0; dly--)
		for (i=0; i<5000; i++);
	}

int isprintf(unsigned int i, char * where)
{
  if(i==0)
  {
    where[0] = '0';
    return 1;
  }
  else
  {
    char buf[12];
    int ile=0;
    while(i)
    {
      buf[ile++] = (i%10) | 0x30;
      i /= 10;
    }
    for(i=0; i<ile; i++)
      where[i] = buf[ile-i-1];
    return ile;
  }
}


/*----------------------------------------------------------*\
 | UART1                                                    |
\*----------------------------------------------------------*/
void  UART1_PutCh(unsigned char Ch)	{
	U1THR = Ch;                           
	while( (U1LSR&0x40)==0 );         
	}

unsigned char  UART1_GetCh()	{
	unsigned char c = 0;
	if(U1LSR&0x01)
		c = U1RBR;
	return c;
	}

void  UART1_PutStr(unsigned char const *str) {
	while(*str)
		UART1_PutCh(*str++);        
	}

void  UART1_PutnStr(unsigned char const *str, int n) {
	for(i=0; i<n; i++)
		UART1_PutCh(*(str+i));        
	}

void  atCommand(unsigned char const *str) {
	while(*str) {
		DelayNS(500);
		UART1_PutCh(*str++);      
	}
	DelayNS(50);
	UART1_PutCh(10);
	UART1_PutCh(13);	
}

void AD_init(void) {
	PINSEL_set_accel;
	PINSEL_set_batt;  ADCR = AD_ACCELON | AD_BATTON;
//  ADINTEN = AD_PIN_KEYB;

//	VICVectCntl3=0x20|INT_ANAL;
//	VICVectAddr3=(unsigned int)IRQ_Anal;
//  VICIntEnable=(1<<INT_ANAL);
}

void resetSleepTimer() {
	T0TC = 0;
}

void setRange(char r) {
  if(r)
    IOSET=PIN_ACC_RANGE;
  else
    IOCLR=PIN_ACC_RANGE;
}

void showBattInfo() {
  
}

void showStatus() {
	//sprintf(buf, "T0: %d\r\nS?: %d\r\n", T0TC, sampling);
	UART1_PutStr(buf);
}

void startSampling() {
	sampling=1;  setRange(1);
}

void stopSampling() {
	sampling=0;
}


/******************************************************
*** DOSTEPNE KOMENDY:

DISCONNECT[ED]		- od modulu BT
CONNECT				- od modulu BT
status
batt
start
stop
range <n>
halt
d - don't sleep


1. Jeœli w przerwaniu zerujesz flagi u¿ywaj = a nie |= (doczytaj dlaczego)
2. Wywal wspomniane enable_irq i disable_irq i ZAPOMNIJ o nich na d³u¿szy czas.
3. Do³¹cz sprzêtow¹ lub programow¹ eliminacjê drgañ przycisku (poszukaj debouncing)
4. Nie siedŸ w przerwaniu 100ms. Przerwania powinny byæ jak najkrótsze.


******************************************************/

void parseCommand() {
	switch(buf[0]) {
		case 'D':				/* DISCONNECT */
			stopSampling();
			connected = 0;
			break;
		case 'C':				/* CONNECTED */
			UART1_PutStr("Hello!\r\n");
			connected = 1;
			startSampling(); // gdyz brak komunikacji dwustronnej
			break;
		case 'b':				/* batt */
			showBattInfo();
			break;
		case 'r':				/* range */
			setRange(buf[6]^0x30);
			break;
		case 'd':
			resetSleepTimer();
			break;
		case 'h':
			have_to_halt = 0x01;
			break;
		case 's':
			switch(buf[3]) {
				case 't':		/* status */
					showStatus();
					break;
				case 'r':		/* start */
					startSampling();
					break;
				case 'p':		/* stop */
					stopSampling();
					break;
				default:;
			}
			break;
		default:;
	}
}

/********** PRZERWANIA **********/

/* Przerwanie timera 0, ustawiajace zmienna wymuszajaca wylaczenie urzadzenia */
void __attribute__ ((interrupt)) IRQ_Timer0() {
	have_to_halt = 0x01;
	T0IR = 0x01;
	VICVectAddr = 0x00;	
} 

/* Przerwanie zewnetrzne, wywolywane po uspieniu i blokujace samo siebie */
void __attribute__ ((interrupt))IRQ_Button() {
	VICIntEnClr = (1<<INT_BUTTON);
	EXTWAKE = 0x00;
	VICVectAddr = 0x00;
}

/* Przerwanie portu szeregowego */
void __attribute__ ((interrupt)) IRQ_Uart1() {				// pobierz znak przy przerwaniu z BT
	unsigned char c = 0;
	IOSET = LED;
	if(U1LSR&0x01)
		c = U1RBR;
    if((bufc < 254) && (c != 10) && (c != 13))
    {
    	buf[bufc++]=c;
    	buf[bufc]=0;
    }
    else if(bufc)
    {
    	bufc=0;
    	parseCommand();
    }
	VICVectAddr = 0x00;
}

/* Przerwanie przetwornika A/C - nieuzywane */
void __attribute__ ((interrupt)) IRQ_Anal() {	
	unsigned char c = 0;
//	VICVectAddr = 0x00;
}

void __attribute__ ((interrupt)) UNDEF_Routine() {}
void __attribute__ ((interrupt)) SWI_Routine() {}
void __attribute__ ((interrupt)) FIQ_Routine() {}
void __attribute__ ((interrupt)) IRQ_Routine() {}


void pll_init() {
/* Dla kwarcu 12MHz:
 * PLLCON 6:5 - potega 2 przy dzielniku
 * PLLCON 4:0 - mnoznik - 1
 * CPU freq = 12 * (p+1) MHz
 */
	PLLCFG = MHZ_M | (MHZ_D<<5);  /* ustaw */
	// PLLCFG = 0x00000024; 
	PLLCON = 1;     /* zalacz */
	PLLFEED = 0xAA;
	PLLFEED = 0x55;
	while(!(PLLSTAT & (1<<10))); /* czekaj na zablokowanie */
	PLLCON = 3;     /* dolacz do procesora */
	PLLFEED = 0xAA;
	PLLFEED = 0x55;
  APBDIV = MHZ_APBDIV; /* czestotliwosc magistrali = CPU freq / 2 */
}

/* wylacz urzadzenie: zmien znaczenie pinu przerwania zewnetrznego, ustaw przerwanie,
   przejrz do trybu power down */
void power_down(void) {
  IOSET = LED; /* wygas diode */
  IOCLR = PIN_ACC_SLEEP; /* wylacz akcel. */
	PINSEL0 |= (1<<28);																		   
	EXTWAKE = 0x02;	 /* wake up by EINT1 */
	EXTMODE = 0x02; /* edge sensitive */
	EXTINT = 0x03;
	VICVectCntl2=0x20|INT_BUTTON;
	VICVectAddr2=(unsigned int)IRQ_Button;
	VICIntEnable = (1<<INT_BUTTON);
	PCON = 0x02;	 /* power down mode */
}

/* bezposrednio po wlaczeniu: wyzeruj zmienna have_to_halt, przywroc ustawienia PLL,
   wyzeruj timer0, przywroc ustawienia zasilania */
void power_up(void) {
	have_to_halt = 0x00;
  pll_init(); 
	T0TC = 0;
	sampling = 0;
	connected = 0;
	IOSET = PIN_BT_RESET;
	DelayNS(50);
	IOCLR = PIN_BT_RESET;
	IOSET = PIN_ACC_SLEEP;
}



/* void setButInterrupt() {
	PINSEL0 |= (1<<28);
	VICVectCntl0 = 0x20 | INT_BUTTON;
	VICVectAddr0 = (unsigned long) EINT1_routine;
	VICIntEnable = (1<<INT_BUTTON);
}	*/

/*----------------------------------------------------------*\
 | UART1 Initial                                            |
\*----------------------------------------------------------*/
void  UART1_init(void)	{
	unsigned short int fdiv;
	/* inicjalizacja interfejsu */
	PINSEL0 |= 5<<16;              

	U1LCR = 0x83;                      // DLAB = 1
	fdiv = (30000000/16)/19200;
	U1DLM= fdiv/256;
	U1DLL = fdiv%256;
	/* U1DLL = 0x000000C2 */;
	U1LCR = 0x03;
																	   
	/* inicjalizacja przerwan */
	U1IER = 0x01;
	VICIntSelect=0x00;
	VICVectCntl0=0x20|INT_UART1;
	VICVectAddr0=(unsigned int)IRQ_Uart1;
	VICIntEnable=(1<<INT_UART1); 
}

unsigned int AnalToPix(int port) {
  volatile unsigned long r;
	ADINTEN = 0;
	ADSTAT = 0;
	ADCR = 0x00200300 + (1 << port);// enable converter
	DelayNS(10);
	ADCR = 0x01200300 + (1 << port); // enable converer and start

	do {
		r = ADGDR;
	} while ((r & 0x80000000) == 0);
	return (r >> 6) & 0x03ff;
	
	return ADDR0 | ADDR1 | ADDR2 | ADDR3 | ADDR4 | ADDR5 | ADDR6 | ADDR7 ; //(ADGDR >> 6) & 0x03ff;
}


void sample() {
	    if((i++)%2)
			  IOSET=LED;
	    else
		    IOCLR=LED;
			
		for(j=0; j<N_PROB; j++) {
	    	    probki[j][0] = ACC0_VAL;
	    	    probki[j][1] = ACC1_VAL;
	    	    probki[j][2] = ACC2_VAL;
		}
		
	    for(k=0; k<3; k++)
		  wynik[k]=0;
	

	    for(k=0; k<3; k++) {
		  for(j=0; j<N_PROB; j++)
		    wynik[k] += probki[j][k];
		  wynik[k] /= N_PROB;
		}		
		
		for(j=0; j<N_PROB; j++) {
          probki[j][3] = 0;
		  for(k=0; k<3; k++)
		    probki[j][3] += KW(probki[j][k] - wynik[k]);
		}
		for(j=1; j<N_PROB; j++)
		  for (k=0; k<j; k++)
		    if (probki[k][3] > probki[k+1][3]) {
			  unsigned int a;
              for(a=0; a<4; a++) {
			    unsigned int tmp=probki[k+1][a];
			    probki[k+1][a]=probki[k][a];
			    probki[k][a]=tmp;
	        }
		  }
	    
		for(k=0; k<3; k++) {
		  wynik[k] = 0;
		  for(j=N_WYWAL; j<N_PROB-N_WYWAL; j++)
		    wynik[k] += probki[j][k];
		  wynik[k] /= (N_PROB-N_WYWAL-N_WYWAL);
		}		
		s[14]=13; s[15]=10; s[16]=0; s[4]=s[9]=32;
	  for(k=0; k<3; k++) {
			for(j=3; j>=0; j--) {
			  if(wynik[k])
		  		  s[5*k+j]=(wynik[k] % 10)+0x30;
				else
				    s[5*k+j]=32;
		  	wynik[k]/=10;
			}
		}
		UART1_PutStr(s);
}

void timer0_init() {
	T0TC=0;
	T0PR=99;
	T0MCR=0x03;
	T0MR0=600000 * POWER_OFF_TIMEOUT;
	T0TCR=0x01;
	VICIntSelect=0x00;
	VICVectCntl1=0x20|INT_TIMER0;
	VICVectAddr1=(unsigned int)IRQ_Timer0;
	VICIntEnable=(1<<INT_TIMER0);
}


int main(void) {
	int last_key = -1;
	int tmp;
	sampling=0;
	connected=0;
	IODIR|=(PIN_LED|PIN_ACC_SLEEP|PIN_ACC_RANGE|PIN_BT_RESET);	
	IOCLR=LED;
	
	IOSET=PIN_ACC_SLEEP;
	
	PINSEL0|=0x00000000;
	PINSEL1|=0x00000200;

  pll_init(); 
  
  
  UART1_init();
  
	timer0_init();
	PCONP = 0xffffffff; //(PC_NORMAL | PC_DISP);

/*	atCommand("++++");
	atCommand("+++ATH0");
	atCommand("ATL2");
	atCommand("ATZ0");		  
	atCommand("ATN"); */
  
  PINSEL_set_accel;
  ADCR=AD_ACCELON;
  
	for(;;) {
		//tmp=AnalToPix(1);
/*	  tmp = (ACC1_VAL-250) / 50;
		if(tmp!=last_key) {
		 	last_key = tmp;
			s[0]='K'; s[1]=':'; s[2]=' ';
			s[3]=tmp|0x30; s[4]=0;
		} */
		if(connected)
		  resetSleepTimer();
		if(sampling)
		{
	    if((i++)%8 > 2)
			  IOSET=LED;
	    else
		    IOCLR=LED;
		  //k=isprintf(ACCEL_STATE, s);
		  k=isprintf(ACC0_VAL, s);
		  s[k++]=' ';
		  k+=isprintf(ACC1_VAL, s+k);
		  s[k++]=' ';
		  k+=isprintf(ACC2_VAL, s+k);
		  s[k++]=' ';
		  k+=isprintf(JOY_STATE, s+k);
		  //k=isprintf(ADDR0, s);
		  s[k] = '\r'; s[k+1] = '\n';
		  s[k+2] = 0; 
		  /* k=ACCEL_STATE;
		  msg[0]='r';
		  msg[1]='y';
		  msg[2]=' ';
		  msg[3]=JOY_STATE;
		  for(tmp=0; tmp<4; tmp++)
		    msg[3+tmp] = ((k>>((3-tmp)*8))&0xff); */
		  //UART1_PutnStr(msg, 8);
		  UART1_PutStr(s);
		  DelayNS(1);
		}
		else {
		  if(connected) {
	      if(((i++)%100000) > 50000)
			    IOSET=LED;
	      else
		      IOCLR=LED;
		  }
		  else {
	      if(((i++)%100000) > 10000)
			    IOSET=LED;
	      else
		      IOCLR=LED;
      }
		}
		if(have_to_halt) {
			power_down();
			power_up();
		}
	}
}
