#include <REGX52.H>
#define Max 255
#define Min   0
#define Motor P2_4
#define Lamp  P2_3

volatile unsigned char data Suwak, Nastawa;

void Init(void)
{
  P2 = 0x00;
  P2_1 = 1;
  P2_2 = 1;
  P2_5 = 0;
  P2_6 = 1;
  P3_4 = 0;
  Suwak = 0x00;
  Nastawa = 0x0F;
  TMOD = 0x02;
  TCON = 0x00;
  TH0=TL0=0xF0;
  ET0 = 1;
  EA = 1;
  TR0 = 1;
}

void Motor_ISR(void) interrupt 1
{
  Suwak++;
  if(Nastawa == Min){Motor = 0;Lamp = 0;}
  else
  {
    if(Nastawa == Max){Motor = 1;Lamp = 1;}
    else
    {
      if(Suwak<=Nastawa)
       {Motor = 1;Lamp = 1;} 
      else 
       {Motor = 0;Lamp = 0;}
    }
  } 
  if(Suwak==Max) {Suwak = Min;}
}
void main(void)
{
  Init();
  for(;;){;}
}