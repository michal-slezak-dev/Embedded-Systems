#include <REGX52.H>
// extern ... z innego pliku wait();
void wait(void)
{
	unsigned char data x, y; // nie musze inicjowac, bo for inicjuje
	for(x = 0; x < 120; x++)
		for(y = 0; y < 120; y++){;} // kompilator optymalizujacy, nic ; byleby w bajcie sie zmiescic
}

void main(void)
{
	while(1)
	{
		// dioda do portu zerowego i linii zerowej, zerowy bit portu P0
		P0_0 = 1;
		wait();
		P0_0 = 0;
		wait(); // trzeba spowolnic, zeby zmiany zobaczyc
	}
}