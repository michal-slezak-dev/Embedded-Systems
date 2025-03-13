#include <REGX52.H>

// wwwwkkk1
//11101 jesli k 0, musze wygen 1
// 11101111 -> 0xEF
// 11011111 -> 0xDF 
// 1011111 -> 0xBf
// 01111111 -> 0x7F
unsigned char code Tab[] = {0xEF, 0xDF, 0xBF, 0x7F}; // nie beda sie zmieniac, zapisana w pam kodu, programu, 
unsigned char data indx; // inicjaizacja tutaj to zmienne gotowe musza byc, kompilator wlasny kod dorzuca, data nie chce do bitow sie dostac
unsigned char bdata Key;  // zbuforowac trzeba w ifie wcisniecia, pamiec adresowana bajtowo i bitowo, chcemy sie do bitow dostac zmiennej
// P2_1 P2_2 P2_3
sbit Key_1 = Key^1; // 1. bit
sbit Key_2 = Key^2;
sbit Key_3 = Key^3;

void Init(void)
{
	indx = 0;
}

/*void wait(void)
{
	unsigned char data x, y;
	for(x = 0; x < 80; x++)
		for(y = 0; y < 80; y++){;}
}*/

void main(void)
{//P2.1-3 kol P2.4-7 wiersze
	Init();
	while(1)
	{
		P2 = Tab[indx];
		Key = P2; // tylko tu uzytkownik ma prawo cos powiedziec, bo uzytkownik potem do decyzji nie ma dostepu juz
		
		if((Key_1 && Key_2 && Key_3) == 0) // PRZYCISK WCISNIETY, w oparciu o swoja zmienna KEY, jesli ktorekolwiek o 0
		{
			P1 = Key; // od dolu zero moje, a gora to to co wyszlo , teraz wiemy, w zal od P1 cos robimy
		}
		//wait();
		indx++;
		
		if(indx == 4)
		{
			indx = 0;
		}
	}
}