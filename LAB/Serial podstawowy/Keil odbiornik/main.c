#include <REGX52.H>

void Serial() interrupt 4
{
	P1 = SBUF;
	RI = 0;
}


main()
{
	SCON = 0x50; // SM1 REN
	TH2 = RCAP2H = 0xFF;
	TL2 = RCAP2L = 0xDC;
	T2CON = 0x34; // TR2 TCLK RCLK
	 
	ES = 1;
	EA = 1;
	
	while(1)
	 ;
}

