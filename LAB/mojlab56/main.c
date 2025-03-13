#include <REGX52.H>

#define OD 	5
#define DO 	30

// 0 malejaca, 1 rosnaca
#define ZLICZANIE_KIERUNEK 0

unsigned char xdata Ones _at_ 0xFD00;
unsigned char xdata Tens _at_ 0xFE00;

unsigned char code Koder[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x6F, 0x76, 0x30, 0x1E, 0x76, 0x38, 0x15, 0x54, 0x3F, 0x73, 0x67, 0x50, 0x6D, 0x78, 0x3E, 0x1C, 0x2A, 0x76, 0x66, 0x5B}; // 8 bitow czyli char, musi globalna byc, bbo w przerwaniu chce uzyc, mam dla 0 1 2 3, wiec potem dla kolejnych trzeba
unsigned char code trailTens2[] = {0x40, 0x40, 0x60, 0x60, 0x61, 0x61, 0x23, 0x07, 0x0F, 0x0E, 0x1E, 0x1C, 0x5C, 0x58, 0x58, 0x50, 0x50, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char code trailOnes2[] = {0x4C, 0x44, 0x44, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x42, 0x42, 0x43, 0x43, 0x63, 0x23, 0x33, 0x31, 0x38, 0x1C, 0x5C};

signed char animationIndx = 0;
unsigned char isFirstClick = 0, timeBetweenClicks, holdTime = 0, freshClick = 0;

unsigned char j = OD;
volatile unsigned char data segmentsTens;
volatile unsigned char data segmentsOnes;
volatile unsigned char data ind1, ind2;
														 
														 
														 
bit isButtonPressed = 0; // czy wcisniete
unsigned char playAnimation1 = 0;

typedef enum {
	STATE_WAIT_FOR_CLICK,
	STATE_WAIT_FOR_DOUBLE_CLICK,
	STATE_WAIT_FOR_RELEASE
} StateProgram;

StateProgram stateProgram = STATE_WAIT_FOR_RELEASE;

char text[] = "81 kazelS lahciM"; // ;-)
char textSize = sizeof(text);

void puste(unsigned char strona) {
	if (strona == 1) 			segmentsTens = 0x00;
	else if (strona == 2) segmentsOnes = 0x00;
}

void sleep(unsigned char data n,unsigned char data m)
{
	int i,j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{;}
	}
}


void wyswietlTekst(unsigned char znak, unsigned char strona_wys) {
	
	unsigned int ascii = znak;
	switch (ascii) {
		
		
		case 32: znak = 0x00;					break;
		
		case 48: znak = Koder[0];					break;
		case 49: znak = Koder[1];					break;
		case 50: znak = Koder[2]; 					break;
		case 51: znak = Koder[3];					break;
		case 52: znak = Koder[4]; 				break;
		case 53: znak = Koder[5]; 					break;
		case 54: znak = Koder[6];					break;
		case 55: znak = Koder[7]; 				break;
		case 56: znak = Koder[8]; 				break;
		case 57: znak = Koder[9];			break;
		
		case 65: case 97: 	znak = Koder[10];	break;
		case 66: case 98: 	znak = Koder[11];	break;
		case 67: case 99: 	znak = Koder[12];	break;
		case 68: case 100:	znak = Koder[13];	break;
		case 69: case 101:	znak = Koder[14];	break;
		case 70: case 102: 	znak = Koder[15];	break;
		case 71: case 103:	znak = Koder[16];	break;
		case 72: case 104:	znak = Koder[17];	break;
		case 73: case 105: 	znak = Koder[18];	break;
		case 74: case 106:	znak = Koder[19];	break;
		case 75: case 107: 	znak = Koder[20];	break;
		case 76: case 108: 	znak = Koder[21];	break;
		case 77: case 109: 	znak = Koder[22];	break;
		case 78: case 110:	znak = Koder[23];	break;
		case 79: case 111:	znak = Koder[24];	break;
		case 80: case 112: 	znak = Koder[25];	break;
		case 81: case 113:	znak = Koder[26];	break;
		case 82: case 114:	znak = Koder[27];	break;
		case 83: case 115: 	znak = Koder[28];	break;
		case 84: case 116:	znak = Koder[29];	break;
		case 85: case 117:	znak = Koder[30];	break;
		case 86: case 118: 	znak = Koder[31];	break;
		case 87: case 119:	znak = Koder[32];	break;
		case 88: case 120:	znak = Koder[33];	break;
		case 89: case 121: 	znak = Koder[34];	break;
		case 90: case 122:	znak = Koder[35];	break;

		default: P1 = ascii;	break;
	}
	
	if (strona_wys == 1) 			segmentsTens = znak;
	else if (strona_wys == 2) segmentsOnes = znak;
}


void numeryWyswietl(unsigned int n, int baza) {
				if(n < baza) {
				wyswietlTekst('0', 1);
				
				if (baza > 10 && n >= 10) {
					wyswietlTekst(n + 65 - 10, 2); 
				} else {
					wyswietlTekst(n + 48, 2);
				}
				
			} else {
				unsigned int reszta = n % baza; 
				unsigned int dziesiatki = (n - reszta) / baza; 
				
				if (baza > 10 && dziesiatki >= 10) {
					wyswietlTekst(dziesiatki + 65 - 10, 1); 
				} else {
					wyswietlTekst(dziesiatki + 48, 1); 
				}
				
				if (baza > 10 && dziesiatki >= 10) {
					wyswietlTekst(reszta + 65 - 10, 2); 
				} else {
					wyswietlTekst(reszta + 48, 2);
				}
				
			}
}

/*void AnimacjaLewa(void)
{

	while(1)
	{				
		segmentsTens = AnimacjaL[ind1];
		ind1++;
		sleep(40,40);
			if(ind1 >= sizeof(AnimacjaL)/sizeof(AnimacjaL[0])) {
				break;
			}
			else
				continue;

	}
}*/

/*void AnimacjaPrawa(void)
{ 

	puste(1);	
	while(1)
	{
		segmentsOnes = AnimacjaP[ind2];
		ind2++;
		sleep(40,40);
		if(ind2==5)
		{
			AnimacjaLewa();
		}

	}

}*/

// button na przerwanie INT0
void ISR_INT() interrupt 0
{
    // czekam na wcisniecie
    if (P3_2 == 0 && isButtonPressed == 0) {
			isButtonPressed = 1;
			
			if(stateProgram == STATE_WAIT_FOR_CLICK)
			{
				playAnimation1 = 2;
				P1_0 = !P1_0;
				stateProgram = STATE_WAIT_FOR_DOUBLE_CLICK;
			}
			
			// podwojne wcisniecie
			else if(stateProgram == STATE_WAIT_FOR_DOUBLE_CLICK)
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
							P1_1 = !P1_1;
							playAnimation1 = 3;
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
	if(isButtonPressed) holdTime++;
	
	if(playAnimation1 == 1) // przewijanie tekstu w prawo automatycznie
	{
		wyswietlTekst(text[i], 1); 
		if (i > 0) {
			wyswietlTekst(text[i-1], 2);
		} else {
			puste(1);
		}
		i++;
		
		if (i >= textSize) i = 0;
	}
	else if(playAnimation1 == 2)
	{
		segmentsTens = trailTens2[animationIndx];
		segmentsOnes = trailOnes2[animationIndx];
		animationIndx++;
		if(animationIndx == 28) animationIndx = 0;
	}
	else if(playAnimation1 == 3)
	{		
		// rosnaco
		if (ZLICZANIE_KIERUNEK == 1) {
			if (j <= DO) {
				numeryWyswietl(j, 10);
				j++;
			}else
			{
				j = OD;
			}
		}else { // malejaco
			unsigned char obecna = DO - j + OD;
			if (obecna >= OD) {
				numeryWyswietl(obecna, 10);
				j++; 
			}else
			{
				j = OD;
			}
		}
	}
	}



void main(void)
{
		ind1 = 0, ind2 = 0;
    EA = 1;   // globalne przerwania wlaczam
	
    EX0 = 1;  // zezwalam na przerwanie INT0
    IT0 = 1;  // przerwanie INT0 na zbocze opadajace
	
		// T1
		ET1 = 1;
		TR1 = 1; // uruchamiam w tryb pracy timera 1, bo normalnie stop
		TMOD = 0 | T1_M0_; 
	
    while (1) {
			
				// puszczenie
        if (P3_2 == 1 && isButtonPressed == 1) {
            isButtonPressed = 0;  // po puszczeniu
						
						
            if(stateProgram == STATE_WAIT_FOR_RELEASE)
						{
							playAnimation1 = 1;
							stateProgram = STATE_WAIT_FOR_CLICK;
						}
						
						holdTime = 0;
        }
				
        Tens = segmentsTens;
        Ones = segmentsOnes;
    }
}
