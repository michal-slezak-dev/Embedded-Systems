#include <REGX52.H>


unsigned char xdata Ones _at_ 0xFD00;
unsigned char xdata Tens _at_ 0xFE00;

unsigned char code Koder[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x6F, 0x76, 0x30, 0x1E, 0x76, 0x38, 0x15, 0x54, 0x3F, 0x73, 0x67, 0x50, 0x6D, 0x78, 0x3E, 0x1C, 0x2A, 0x76, 0x66, 0x5B}; // 8 bitow czyli char, musi globalna byc, bbo w przerwaniu chce uzyc, mam dla 0 1 2 3, wiec potem dla kolejnych trzeba

volatile unsigned char data segmentLeft;
volatile unsigned char data segmentRight;
unsigned char i = 0;
	
bit isPressed = 0; // czy wcisniete

void sleep()
{
    int i, j;
    for(i = 0; i < 70; i++)
    {
        for(j = 0; j < 70; j++)
        {
            /* nic */;
        }
    }
}
	
void blank_bcd(unsigned char side) {
	if (side == 1) 			segmentLeft = 0x00;
	else if (side == 2) segmentRight = 0x00;
}

void displayText(unsigned char character, unsigned char bcd_side) {
	
	unsigned int ascii = character;
	switch (ascii) {
		
		
		case 32: character = 0x00;					break;
		
		case 48: character = Koder[0];					break;
		case 49: character = Koder[1];					break;
		case 50: character = Koder[2]; 					break;
		case 51: character = Koder[3];					break;
		case 52: character = Koder[4]; 				break;
		case 53: character = Koder[5]; 					break;
		case 54: character = Koder[6];					break;
		case 55: character = Koder[7]; 				break;
		case 56: character = Koder[8]; 				break;
		case 57: character = Koder[9];			break;
		
		case 65: case 97: 	character = Koder[10];	break;
		case 66: case 98: 	character = Koder[11];	break;
		case 67: case 99: 	character = Koder[12];	break;
		case 68: case 100:	character = Koder[13];	break;
		case 69: case 101:	character = Koder[14];	break;
		case 70: case 102: 	character = Koder[15];	break;
		case 71: case 103:	character = Koder[16];	break;
		case 72: case 104:	character = Koder[17];	break;
		case 73: case 105: 	character = Koder[18];	break;
		case 74: case 106:	character = Koder[19];	break;
		case 75: case 107: 	character = Koder[20];	break;
		case 76: case 108: 	character = Koder[21];	break;
		case 77: case 109: 	character = Koder[22];	break;
		case 78: case 110:	character = Koder[23];	break;
		case 79: case 111:	character = Koder[24];	break;
		case 80: case 112: 	character = Koder[25];	break;
		case 81: case 113:	character = Koder[26];	break;
		case 82: case 114:	character = Koder[27];	break;
		case 83: case 115: 	character = Koder[28];	break;
		case 84: case 116:	character = Koder[29];	break;
		case 85: case 117:	character = Koder[30];	break;
		case 86: case 118: 	character = Koder[31];	break;
		case 87: case 119:	character = Koder[32];	break;
		case 88: case 120:	character = Koder[33];	break;
		case 89: case 121: 	character = Koder[34];	break;
		case 90: case 122:	character = Koder[35];	break;

		default: P1 = ascii;	break;
	}
	
	if (bcd_side == 1) 			segmentLeft = character;
	else if (bcd_side == 2) segmentRight = character;
}
bit correct = 0;
bit incorrect = 0;
unsigned char receivedByte;
void ISR_Serial(void) interrupt  4
{
  if(TI == 1) {TI = 0;}
  if(RI == 1)
   {
     RI = 0;
		 receivedByte = SBUF;
		 sleep();
     if(receivedByte == '1') {correct = 1;}
     else if(receivedByte == '0') {incorrect = 1;}
     IE1 = 1;
   }
}  


// T1
void ISR_T1() interrupt 3
{	
		if(correct) // on
		{
			displayText('O', 1);
			displayText('N', 2);
			P1_0 = 1;
			correct = 0;
		}
	
		if(incorrect) // off
		{
			displayText('O', 1);
			displayText('F', 2);
			P1_1 = 1;
			incorrect = 0;
		}
	}

void Init(void)
  {
    P3_4 = 0;
      
    SCON = 0x50;
    T2CON = 0x30;
    TH2 = RCAP2H = 0xFF;
    TL2 = RCAP2L = 0xDC;
    TR2=1;
		ES = 1;
    EA  = 1;
		
		
		EX0 = 1;  // zezwalam na przerwanie INT0
    IT0 = 1;  // przerwanie INT0 na zbocze opadajace
	
		// T1
		EX1 = 1;
    IT1 = 1;
		ET1 = 1;
		TR1 = 1; // uruchamiam w tryb pracy timera 1, bo normalnie stop
		TMOD = 0 | T1_M0_; 
  }

void main(void)
{
		Init();
    while (1) {				
        Tens = segmentLeft;
        Ones = segmentRight;
    }
}
