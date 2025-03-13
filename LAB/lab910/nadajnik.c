#include <REGX52.H>

bit not_pressed_P2_5_2 = 1; // 5
bit sent = 0;
bit lastState = 0;

void sendByte(unsigned char dta)
{
    P3_4 = 1;
    TI = 0;
    SBUF = dta;
    while(TI == 0)
    {;}
    TI = 0;
    P3_4 = 0;
}

void sleep()
{
    int i,j;
    for(i = 0; i < 70; i++)
    {
        for(j = 0; j < 70; j++)
        {
            /* nic */;
        }
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
}

void main()
{		
		//sleep_5sec();
    Init();

    // Na starcie wszystkie kolumny na 0
    P2_1 = 0;
    P2_2 = 0;
    P2_3 = 0;
    
    while(1)
    {
			
        // KOLUMNA 1
        P2_1 = 1; 
        P2_2 = 1; 
        P2_3 = 0;

        // KLAWISZ 7 -> P2_6
        

        // KLAWISZ * -> P2_7 (zatwierdzanie PINu)
        
			
				// KLAWISZ 1 -> P2_4
        
				
				// KLAWISZ 4 -> P2_5
        
        

        // KOLUMNA 2
        P2_1 = 1; 
        P2_2 = 0; 
        P2_3 = 1;

        // KLAWISZ 5 -> P2_5
        if(not_pressed_P2_5_2 && !P2_5)
        {
            sleep();
						P0_0 = !P0_0;
        }
        not_pressed_P2_5_2 = P2_5;
				
				if(P0_0 != lastState)
				{
					if(P0_0 && !sent)
					{
						sleep();
						sendByte('0');
						sent = 1;
					}else if (!P0_0 && !sent)
					{
						sleep();
						sendByte('1');
						sent = 1;
					}

				}else
				{
					sent = 0;
				}

        // KLAWISZ 8 -> P2_6
       

        // KLAWISZ 0 -> P2_7
        
				// KLAWISZ 2 -> P2_4
        
        // KOLUMNA 3
        P2_1 = 0; 
        P2_2 = 1; 
        P2_3 = 1;
				
				// KLAWISZ 3 -> P2_4
       
				
				// KLAWISZ 9 -> P2_6
       
        // KLAWISZ # -> P2_7
       
				lastState = P0_0;
    }
}
