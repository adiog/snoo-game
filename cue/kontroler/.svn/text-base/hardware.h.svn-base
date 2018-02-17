#define	LED	(1<<0)

#define MHZ_OSC     12
#define MHZ_D       0
#define MHZ_M       4
#define MHZ_CPU     (MHZ_OSC * (MHZ_M+1) / (1<<MHZ_D))
#define MHZ_APBDIV  0x00000002
#define MHZ_APB     (MHZ_CPU / MHZ_APBDIV)

/* TODO */
#define PINSEL0_NORMAL 0
#define PINSEL0_SLEEP  0
#define PINSEL1_NORMAL 0
#define PINSEL1_SLEEP  0

#define JOY_STATE   ((IOPIN>>14)&0x1f)
#define ACC0_VAL    ((ADDR0 >> 6) & 0x03ff)
#define ACC1_VAL    ((ADDR1 >> 6) & 0x03ff)
#define ACC2_VAL    ((ADDR2 >> 6) & 0x03ff)
#define ACCEL_STATE (((ACC0_VAL)<<20) | ((ACC1_VAL)<<10) | (ACC2_VAL))

#define PIN_ACC_SLEEP (1<<11)
#define PIN_ACC_RANGE (1<<10)
#define PIN_BT_RESET  (1<<12)
#define	PIN_LED	      (1<<0)

#define INT_BUTTON  15
#define INT_UART1	  7
#define INT_TIMER0	4
#define INT_TIMER1  5
#define INT_ANAL    18

#define PINSEL_set_accel PINSEL1 |= 63<<12
#define PINSEL_set_batt  PINSEL1 |= 3<<20

#define AD_PIN_BATT   (1<<7)
#define AD_PIN_ACCEL  (1<<0) | (1<<1) | (1<<2)
#define AD_CLKDIV     (13<<8)
#define AD_BURST      (1<<16)
#define AD_CLKS       (0<<17)
#define AD_POWER_ON   (1<<21)
#define AD_POWER_OFF  (0<<21)

#define AD_BATTON     AD_PIN_BATT | AD_CLKDIV | AD_BURST | AD_CLKS | AD_POWER_ON
#define AD_ACCELON    AD_PIN_ACCEL | AD_CLKDIV | AD_BURST | AD_CLKS | AD_POWER_ON
#define AD_OFF        AD_POWER_OFF

																		  
#define PCTIM0		(1<<1)
#define PCTIM1		(1<<2)
#define PCUART0		(1<<3)
#define PCUART1		(1<<4)
#define PCI2C0		(1<<7)
#define PCSPI		(1<<8)
#define PCRTC		(1<<9)
#define PCSSP		(1<<10)
#define PCAD		(1<<12)
#define PCI2C1		(1<<19)
#define PCTIM2		(1<<28)
#define PCTIM3		(1<<29)

#define	PC_NOAD		(PCUART0 | PCUART1 | PCTIM0)
#define PC_NORMAL	(PC_NOAD | PCAD)
#define PC_DISP		(PCTIM1 | PCSPI)	

#define POWER_OFF_TIMEOUT 60

