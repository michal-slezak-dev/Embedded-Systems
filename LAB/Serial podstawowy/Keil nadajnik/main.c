#include <REGX52.H>

unsigned char i = 0;

void main()
{
	SCON = 0x40; // SM1
	TH2 = RCAP2H = 0xFF;
	TL2 = RCAP2L = 0xDC;
	T2CON = 0x34; // TR2 TCLK RCLK

	while(1)
	{
		TI = 0;
		P1 = i;
		SBUF = i;
		while(TI==0)
			;
		
		i++;	
	}
}
