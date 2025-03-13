#include <REGX52.H>
#define Min 0
#define Max 0xFF
#define Motor P2_4
#define Lampka P2_3
unsigned char data Nastawa;
volatile unsigned char data Suwak;

void Setup(void)
{
  Motor = 0;
	//P3_4 = 0;
	P2_6 = 1;
  Nastawa = 0x80;
  Suwak = 0;
  P2= 0xC5;
  TMOD = 0x20;
  TH1=TL1=0xF0;
  ET1 = 1;
  EA = 1;
  TR1 = 1;
}

void ISR_PWM(void) interrupt 3
{
  Suwak++;
  if(Nastawa > Min)
   {
     if(Nastawa == Max) {Motor = 1;Lampka = 1;} 
     else
      {
        if(Suwak <= Nastawa) {Motor = 1;Lampka = 1;} else {Motor = 0;Lampka = 0;}
      }
   }
  else
   {
     Motor = 0;
     Lampka = 0;
   } 
  if(Suwak == Max) {Suwak = 0;}   
}

void main()
{  
  Setup();
  while(1){;}
}