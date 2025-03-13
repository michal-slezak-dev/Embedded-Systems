#include <REGX52.H>

#define FROM 	5
#define TO 	30

// 0 malejaca, 1 rosnaca
#define COUNT_DIRECTION 0

unsigned char xdata Ones _at_ 0xFD00;
unsigned char xdata Tens _at_ 0xFE00;

unsigned char code Koder[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x6F, 0x76, 0x30, 0x1E, 0x76, 0x38, 0x15, 0x54, 0x3F, 0x73, 0x67, 0x50, 0x6D, 0x78, 0x3E, 0x1C, 0x2A, 0x76, 0x66, 0x5B}; // 8 bitow czyli char, musi globalna byc, bbo w przerwaniu chce uzyc, mam dla 0 1 2 3, wiec potem dla kolejnych trzeba
unsigned char code animationLeft[] = {0x40, 0x40, 0x60, 0x60, 0x61, 0x61, 0x23, 0x07, 0x0F, 0x0E, 0x1E, 0x1C, 0x5C, 0x58, 0x58, 0x50, 0x50, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char code animationRight[] = {0x4C, 0x44, 0x44, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x42, 0x42, 0x43, 0x43, 0x63, 0x23, 0x33, 0x31, 0x38, 0x1C, 0x5C};

volatile signed char animationIndx = 26;
unsigned char isFirstClick = 0, timeBetweenClicks;

unsigned char j = TO;
volatile unsigned char data segmentLeft;
volatile unsigned char data segmentRight;
														 
														 
														 
bit isPressed = 0; // czy wcisniete
unsigned char animationNum = 0;

typedef enum {
	STATE_WAIT_FOR_CLICK,
	STATE_WAIT_FOR_DOUBLE_CLICK,
	STATE_WAIT_FOR_RELEASE
} State;

State programState = STATE_WAIT_FOR_RELEASE;

char text[] = " 81 kazelS lahciM"; // ;-)
char textSize = sizeof(text);

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


void displayNum(unsigned int n, int base) {
				if(n < base) {
				displayText('0', 1);
				
				if (base > 10 && n >= 10) {
					displayText(n + 65 - 10, 2); 
				} else {
					displayText(n + 48, 2);
				}
				
			} else {
				unsigned int remainder = n % base; 
				unsigned int tens = (n - remainder) / base; 
				
				if (base > 10 && tens >= 10) {
					displayText(tens + 65 - 10, 1); 
				} else {
					displayText(tens + 48, 1); 
				}
				
				if (base > 10 && tens >= 10) {
					displayText(remainder + 65 - 10, 2); 
				} else {
					displayText(remainder + 48, 2);
				}
				
			}
}

// button na przerwanie INT0
void ISR_INT() interrupt 0
{
    // czekam na wcisniecie
    if (P3_2 == 0 && isPressed == 0) {
			isPressed = 1;
			
			if(programState == STATE_WAIT_FOR_CLICK) // czekam na akcje uzytkownika 
			{
				// przechodze do stanu 2
				animationNum = 2;
				programState = STATE_WAIT_FOR_DOUBLE_CLICK;
			}
			
			// podwojne wcisniecie
			else if(programState == STATE_WAIT_FOR_DOUBLE_CLICK) // jestem w stanie 2 i czekam na trzecia akcje
			{
				if(!isFirstClick)
				{
					isFirstClick = 1;
					timeBetweenClicks = 0;
				}
				else
				{
					if(timeBetweenClicks < 2)
					{
							animationNum = 3; // przechodze do stanu 3
					}
					isFirstClick = 0;
				}
			}
    }
}


unsigned char i = 0;
// T1
void ISR_T1() interrupt 3
{
	if(isFirstClick) timeBetweenClicks++;
	
	if(animationNum == 1) // przewijanie tekstu w prawo automatycznie, animacja w stanie 1
	{
		displayText(text[i], 1); 
		if (i > 0) {
			displayText(text[i-1], 2);
		} else {
			blank_bcd(1);
		}
		i++;
		
		if (i >= textSize) i = 0;
	}
	else if(animationNum == 2) // animacja w stanie 2
	{
		segmentLeft = animationLeft[animationIndx];
		segmentRight = animationRight[animationIndx];
		
		animationIndx--;
		if(animationIndx == -1) animationIndx = 26;
	}
	else if(animationNum == 3) // animacja w stanie 3
	{		
		// rosnaco
		if (COUNT_DIRECTION == 1) {
			if (j <= TO) {
				displayNum(j, 10);
				j++;
			}else
			{
				j = FROM;
			}
		}else { // malejaco
			unsigned char current = TO - j + FROM;
			if (current >= FROM) {
				displayNum(current, 10);
				j++; 
			}else
			{
				j = FROM;
			}
		}
	}
	}



void main(void)
{
    EA = 1;   // globalne przerwania wlaczam
	
    EX0 = 1;  // zezwalam na przerwanie INT0
    IT0 = 1;  // przerwanie INT0 na zbocze opadajace
	
		// T1
		ET1 = 1;
		TR1 = 1; // uruchamiam w tryb pracy timera 1, bo normalnie stop
		TMOD = 0 | T1_M0_; 
	
    while (1) {
			
				// puszczenie
        if (P3_2 == 1 && isPressed == 1) {
            isPressed = 0;  // po puszczeniu
						
						
            if(programState == STATE_WAIT_FOR_RELEASE) // czekam na pierwsza akcje uzytkownika
						{
							animationNum = 1;
							programState = STATE_WAIT_FOR_CLICK;
						}
						
        }
				
        Tens = segmentLeft;
        Ones = segmentRight;
    }
}
