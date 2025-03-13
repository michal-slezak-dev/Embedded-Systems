#include <REGX52.H>
unsigned char xdata Lewy _at_ 0xFE00;   // pamiec zewnetrzna, wiec xdata
unsigned char xdata	Prawy _at_ 0xFD00; // zawsze umieszczone w tym samym obszarze pamieci

//data 13 bajtow pamieci uzylismy, czt wartosci w koder beda zmieniane ? nie! wiec w pamieci code!!!
unsigned char code Koder[] = {0x3F, 0x06, 0x5B, 0x4F}; // 8 bitow czyli char, musi globalna byc, bbo w przerwaniu chce uzyc, mam dla 0 1 2 3, wiec potem dla kolejnych trzeba
// pamiec kodu jest typu flash
volatile unsigned char data ind1, ind2; // bo w obsludze przerwania, po to ze jak wejde lewy prawy to ind1 i ind2 beda juz zainicjowane
// volatile bierze nowsza ze stosu ?
void init(void)
{
	
	P3_4 = 0;
	ind1 = 0;
	ind2 = 0;
	TCON = 0x00;
	TMOD = 0X20;
	TH1 = 0x00;
	ET1 = 1; // enable timer 1, wlaczylem przerwania od timera, jesli wlaczylem przerwania to musze miec funkcje obslugi zrobiona, np. BCD
	// musze jeszcze skonfigutowac timer
	
	IT0 =1;
	EX0 = 1; // obsluga przelacznika
	
	// na koncu
	EA = 1;
	TR1 = 1; // bo mielismy w trybie stop a chcemy, zeby dzialal, laga od timera gasi sie sama w momencie wyjscia z procedury obslugi przerwania
}

void BCD(void) interrupt 3 // timer 1
{
	Lewy = Koder[ind1]; //  00111111 dp g f e d c b a, jesli chce 0 to dp 0(zgaszony)0, 1111...i na hexa zamieniam
	Prawy = Koder[ind2]; 
}

void Licznik(void) interrupt 0 // zerowy ma wyzszy priorytet
{
	
	ind2++;
	if(ind2 == 4) // doszedlem do konca zliczania
	{
		ind2 = 0;
		ind1++;
		if(ind1 == 4)
		{
			ind1 = 0;
		}
	}
	Lewy = Koder[ind1];
	Prawy = Koder[ind2]; 
}

void main(void)
{
	init();
	while(1){;}
}