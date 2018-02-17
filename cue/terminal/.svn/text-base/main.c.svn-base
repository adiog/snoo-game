/***********************************

Terminal
- program przekazujacy dane pomiedzy
  UART0 a UART1 z predkoscia 19200

autor: Adam Morawski

***********************************/

#include "../lpc2103.h"

#define	LED	(1<<3)

unsigned int i, j, k;

void UNDEF_Routine();
void SWI_Routine();
void IRQ_Routine();
void FIQ_Routine();

void UNDEF_Routine() {}
void SWI_Routine() {}
void IRQ_Routine() {} 
void FIQ_Routine() {}



/*----------------------------------------------------------*\
 | Delay ns                                                 |
\*----------------------------------------------------------*/
void DelayNS (unsigned int dly) {
	unsigned int i;
	
	for ( ; dly>0; dly--)
		for (i=0; i<5000; i++);
	}
/*----------------------------------------------------------*\
 | UART0 Initial                                            |
\*----------------------------------------------------------*/
void  UART0_init(void)	{
	unsigned short int fdiv;

	PINSEL0 |= 5;              

	U0LCR = 0x83;                      // DLAB = 1
	fdiv = (15000000/16)/19200;
	U0DLM= fdiv/256;
	U0DLL = fdiv%256;
	U0LCR = 0x03;
	}

/*----------------------------------------------------------*\
 | UART0 Send Char                                          |
\*----------------------------------------------------------*/
void  UART0_PutCh(unsigned char Ch)	{
	U0THR = Ch;                           
	while( (U0LSR&0x40)==0 );         
	}

/*----------------------------------------------------------*\
 | UART0 Receive Char                                       |
\*----------------------------------------------------------*/
unsigned char  UART0_GetCh()	{
	unsigned char c = 0;
	if(U0LSR&0x01)
		c = U0RBR;
	return c;
	}

/*----------------------------------------------------------*\
 | UART1 Initial                                            |
\*----------------------------------------------------------*/

unsigned short int set = 0x00;
unsigned short int fdiv = (15000000/16)/9600;

void  UART1_init()	{
	// unsigned short int fdiv;
	PINSEL0 |= 5<<16;       
	U1LCR = 0x80 | set;                      // DLAB = 1
	// fdiv = (60000000/16)/spd;
	U1DLM= fdiv/256;
	U1DLL = fdiv%256;
	U1LCR = 0x00 | set;
	}

/*----------------------------------------------------------*\
 | UART1 Send Char                                          |
\*----------------------------------------------------------*/
void  UART1_PutCh(unsigned char Ch)	{
	U1THR = Ch;                           
	while( (U1LSR&0x40)==0 );         
	}

/*----------------------------------------------------------*\
 | UART1 Receive Char                                       |
\*----------------------------------------------------------*/
unsigned char  UART1_GetCh()	{
	unsigned char c = 0;
	if(U1LSR&0x01)
		c = U1RBR;
	return c;
	}

/*----------------------------------------------------------*\
 | UART0 Send String                                        |
\*----------------------------------------------------------*/
void  UART0_PutStr(unsigned char const *str) {
	while(*str)
		UART0_PutCh(*str++);        
	}
void  UART1_PutStr(unsigned char const *str) {
	while(*str)
		UART1_PutCh(*str++);        
	}

void  BT_Command(unsigned char const *str) {
	while(*str){
		UART1_PutCh(*str++);
		DelayNS(20);
	      if((i++)%2)
			IOSET=LED;
	      else
		    IOCLR=LED;		        
	}
	UART1_PutCh(0x0D);
	UART1_PutCh(0x0A);
}

void feed() {
	PLLFEED = 0xAA;
	PLLFEED = 0x55;
}

void przyspiesz(unsigned int p) {
	PLLCON = 3;
	PLLCFG = p;
	PLLCFG|= 1<<5;
	feed();
}

int mode = 0;

int main(void) {
	unsigned char c;
	IODIR|=LED;	
	IOCLR=LED;
	UART0_init();
	UART1_init();
    przyspiesz(4);
  UART0_PutStr("Hello! > ");
	for(;;) {
	    if(c = UART0_GetCh()) {
	     if(!mode)
	     {
	      if((i++)%2)
		    	IOSET=LED;
	      else
		      IOCLR=LED;
		    if(c=='@') {
		    	mode = 1;
		    	UART0_PutStr("Tryb ustawien: on\n");
		    }
		    else {
		      UART0_PutCh(c);
		      UART1_PutCh(c);
		    }
		   }
		   else
		   {
		  	if(c=='#') {
		  		mode = 0;
		  		UART0_PutStr("Tryb ustawien: off\n");
		  	}
		  	else {
		  	  if(c=='1')
		  	    fdiv = (15000000/16)/1200;
		  	  if(c=='2')
		  	    fdiv = (15000000/16)/2400;
		  	  if(c=='3')
		  	    fdiv = (15000000/16)/4800;
		  	  if(c=='4')
		  	    fdiv = (15000000/16)/9600;
		  	  if(c=='5')
		  	    fdiv = (15000000/16)/19200;
		  	  if(c=='6')
		  	    fdiv = (15000000/16)/38400;
		  	  if(c=='q')
		  	    set = (set & 0xfc) | 0x00;
		  	  if(c=='w')
		  	    set = (set & 0xfc) | 0x01;
		  	  if(c=='e')
		  	    set = (set & 0xfc) | 0x02;
		  	  if(c=='r')
		  	    set = (set & 0xfc) | 0x03;
		  	  if(c=='a')
		  	    set = (set & 0xf3) | 0x00;
		  	  if(c=='s')
		  	    set = (set & 0xf3) | 0x04;
		  	  if(c=='d')
		  	    set = (set & 0xf3) | 0x08;
		  	  if(c=='f')
		  	    set = (set & 0xf3) | 0x0c;

		  	  UART1_init();
		  	  UART1_PutStr("terefere TRALALA");
		  		UART0_PutStr("OK\n");
		  	}
		 
		   }
		 
	    }
	    if(c = UART1_GetCh()) {
	      if((i++)%2)
			IOSET=LED;
	      else
		    IOCLR=LED;
		  UART0_PutCh(c);
	    }

	}
}
