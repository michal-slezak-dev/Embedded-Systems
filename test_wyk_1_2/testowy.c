#include <REGX52.H>
#define Led_On P0_0 = 0
#define Led_Off P0_0 = 1
#define Jeden 0xE7
#define Gwiazdka 0x77 // czytamprzyccisk, jesli wcisniety to wchodze do waita na ok. 3sekundy i wtej obsludze jeszcze raz czytam przy isk, jesli nadalwcisniety czyli zero to wchodze w akcje, jesli nie to wychodze
unsigned char code Tab[] = {0xEF, 0xDF, 0xBF, 0x7F}; // Wiersze klawiatury
unsigned char data indx;  // Zmienna indeksujaca wiersze
unsigned char bdata Key;  // Zmienna do przechowywania stanu kolumn
bit Enabled;
sbit Key_1 = Key^1;  // P2.1 - Kolumna 1
sbit Key_2 = Key^2;  // P2.2 - Kolumna 2
sbit Key_3 = Key^3;  // P2.3 - Kolumna 3 (przycisk 9 znajduje sie tutaj)

sbit LED = P0^0;  // Dioda podlaczona do P0.0

void Init(void)
{	
	P3_4 = 0;
	indx = 0;  // Ustawienie indeksu poczatkowego na 0
	Enabled = 1;
}

void wait(unsigned int time)  // Prosta funkcja opózniajaca
{
    unsigned int x, y;
    for(x = 0; x < time; x++)
        for(y = 0; y < 127; y++) {;}  // Opóznienie
}

/*void blinkLED(void)  // Funkcja zapalajaca diode 3 razy szybko
{
    unsigned char i;
    for(i = 0; i < 3; i++)  // Powtórzenie 3 razy
    {
        P0_0 = 1;  // Zapalenie diody
        wait(100);  // Krótkie opóznienie (ok. 100 ms)
        P0_0 = 0;  // Zgaszenie diody
        wait(100);  // Krótkie opóznienie
    }
}*/

void main(void)
{	
		//bit Enabled = 1;
    Init();  // Inicjalizacja zmiennych

    while(1)
    {
        P2 = Tab[indx];  // Ustawienie wiersza
        Key = P2;  // Odczyt stanu kolumn
				//flaga jesli nie bylismy tam to wchodze ?
				if((Key_1 && Key_2 && Key_3) == 0) // jesli przycisk wcisniety tak dlugo jak wcisniety to jak drugi raz wchodze nie robie enabled zero jak nie bylem to enabled 1
				{
					//P1=Key;
					if(Enabled == 1)
					{
						//if(Key==Jeden){Led_On;wait(40);Led_Off;Enabled = 0;} // P0_0 = 0, teraz mam migacza, ale tak nie moze byc 
						if(Key==Jeden){Led_On;} // P0_0 = 0, teraz mam migacza, ale tak nie moze byc 						
						if(Key==Gwiazdka){Led_Off;} //P0_0 = 1, jesli zostales wylaczony to wylacz
						Enabled = 0; // ten tutaj zapis odobry, jesli wczesniej mam cala obsulge klawiatury juz 
					}
				}else // jak przycisk puszczony
				{	
					Enabled = 1;
					indx++;  // Zmieniamy wiersz
					if(indx == 4) indx = 0;  // Powrót do pierwszego wiersza po czterech wierszach
				}
        // Sprawdzamy, czy wcisnieto przycisk 9 (wiersz 2, kolumna 3)
        //if((indx == 2) && (Key_3 == 0))  // Wiersz 2 i Kolumna 3 odpowiadaja przyciskowi 9
        //{
          //  blinkLED();  // Zapal diode 3 razy szybko, jesli wcisnieto przycisk 9
        //}else if(Key_3 == 1)
				//{
					//P0_0 = 0;
				//}
        //wait(10);  // Krótkie opóznienie dla stabilizacji sygnalu (debouncing)

    }
}
// DE transmitujace RE odbierajace ulad moze nadawac lub odbierac, ale na nadawanie ustawiony, bo data enabled jest na 1 my chcemy, zeby zaczynaly od nasluchu a nie od nadawania, bo inaczej transmisja sie nie uda
// jak nsluch p34 na 0 wysterowac trzeba