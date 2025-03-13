#include <REGX52.H>

extern w50mS();
unsigned char data Var1, Var2, Var3;

bit not_pressed_P2_4_1 = 1;
bit not_pressed_P2_5_1 = 1;
bit not_pressed_P2_4_2 = 1;
bit not_pressed_P2_6_2 = 1; // 8
bit not_pressed_P2_6_1 = 1; // 7
bit not_pressed_P2_7_1 = 1; // *
bit not_pressed_P2_5_2 = 1; // 5
bit not_pressed_P2_7_2 = 1; // 0
bit not_pressed_P2_7_3 = 1; // #
bit not_pressed_P2_4_3 = 1;
bit not_pressed_P2_5_3 = 1;
bit not_pressed_P2_6_3 = 1;

// Informacja, czy aktualnie wprowadzamy PIN
bit pin_entry_in_progress = 0;

// Flagi do sprawdzania poprawnej sekwencji
bit correct_sequence1 = 0;
bit correct_sequence2 = 0;
bit correct_sequence3 = 0;

// Domyslnie uklad jest ZABLOKOWANY (jak domofon)
int blocked = 1;      // 1 = zablokowane, 0 = odblokowane (ale i tak tymczasowo)
int blocked_5sec = 0; // Flaga blokady na 5s
int wrong_pin_counter = 0;

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

// ~3 sekundy (mnoznik sleep)
void sleep_3sec()
{
    unsigned int x;
    for(x = 0; x < 20; x++)
        sleep();
}

// ~5 sekund (blokada czasowa)
void sleep_5sec()
{
    int x;
    for(x = 0; x < 20; x++)
    {
			
				w50mS();
        /*for(y = 0; y < 300; y++)
        {
            ;
        }*/
    }
}

// Miganie 2 razy (PIN bledny)
void blinkLedWrongPin()
{
    unsigned char k;
    for(k = 0; k < 2; k++)
    {
        P0_0 = 0;  // LED ON
        sleep();
        P0_0 = 1;  // LED OFF
        sleep();
    }
}

// Dioda swieci ~3 sekundy (PIN poprawny)
void lightLedCorrectPin3sec()
{
    P0_0 = 0;       // LED ON
    sleep_3sec();   // ~3 sek
    P0_0 = 1;       // LED OFF
}

// 15 krótkich mrugniec (dzwonek)
void doorbell()
{
    unsigned char u;
    for(u = 0; u < 15; u++)
    {
        sleep();
        P0_0 = 0;  // LED ON
        sleep();
        P0_0 = 1;  // LED OFF
    }
}

// UART – jesli potrzebujesz wyslac bajt
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

// Inicjalizacja
void Init()
{
    EX1 = 1;
    IT1 = 1;
    SCON = 0x50;
    T2CON = 0x30;
    TH2 = RCAP2H = 0xFF;
    TL2 = RCAP2L = 0xDC;
    ES = 1;
    EA  = 1;
    TR2 = 1;
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
		
				
        // Sprawdz, czy mamy blokade 5 sekund (po 3 blednych PINach)
        if(blocked_5sec)
        {
            // Nie pozwalamy na wpisywanie PINu
            continue;  // skacz na poczatek petli
        }
				
				/*if(wrong_pin_counter < 3)
				{
					blocked_5sec = 0;
				}*/
				
				
        // Jezeli 3 razy bledny PIN -> wlacz blokade 5 sek
        if(wrong_pin_counter == 3)
        {
            blocked_5sec = 1;  // wlacz flage blokady czasowej
            // faktyczny "stop" na 5 sek. (kod ponizej nie wykona sie w tym przebiegu)
            sleep_5sec();
            // Po 5 sekundach wracamy tutaj
            blocked_5sec = 0;
            wrong_pin_counter = 0;
            pin_entry_in_progress = 0;
            continue;
        }
        
        //--------------------------------------------
        // Kolumna 1 (P2_1=1, P2_2=1, P2_3=0) – 
        //--------------------------------------------
        P2_1 = 1; 
        P2_2 = 1; 
        P2_3 = 0;

        // KLAWISZ 7 -> P2_6
        if(not_pressed_P2_6_1 && !P2_6)
        {
            sleep();
            correct_sequence1 = 1;    // Pierwszy krok sekwencji
            pin_entry_in_progress = 1;
        }
        not_pressed_P2_6_1 = P2_6;

        // KLAWISZ * -> P2_7 (zatwierdzanie PINu)
        if(not_pressed_P2_7_1 && !P2_7)
        {
            sleep();
            // Jezeli sekwencja 7,5,0
            if(correct_sequence1 == 1 && correct_sequence2 == 1 && correct_sequence3 == 1)
            {
                // Poprawny PIN
          			sendByte('0'); // sygnal "OK"  
                lightLedCorrectPin3sec();    
                // Wyzeruj stany
                correct_sequence1 = 0;
                correct_sequence2 = 0;
                correct_sequence3 = 0;
                pin_entry_in_progress = 0;
                wrong_pin_counter = 0;

            }
            else
            {
                // Bledny PIN
                wrong_pin_counter++;
                blinkLedWrongPin();
							  sendByte('1'); // np. sygnal bledu
                // Reset sekwencji
                correct_sequence1 = 0;
                correct_sequence2 = 0;
                correct_sequence3 = 0;
                pin_entry_in_progress = 0;
            }
        }
        not_pressed_P2_7_1 = P2_7;
			
				// KLAWISZ 1 -> P2_4
        if(not_pressed_P2_4_1 && !P2_4)
        {
            sleep();
            correct_sequence1 = 0;
						correct_sequence2 = 0;
						correct_sequence3 = 0;
            pin_entry_in_progress = 1;
        }
        not_pressed_P2_4_1 = P2_4;
				
				// KLAWISZ 4 -> P2_5
        if(not_pressed_P2_5_1 && !P2_5)
        {
            sleep();
            correct_sequence1 = 0;
						correct_sequence2 = 0;
						correct_sequence3 = 0;
            pin_entry_in_progress = 1;
        }
        not_pressed_P2_5_1 = P2_5;
        

        //--------------------------------------------
        // Kolumna 2 (P2_1=1, P2_2=0, P2_3=1) –
        //--------------------------------------------
        P2_1 = 1; 
        P2_2 = 0; 
        P2_3 = 1;

        // KLAWISZ 5 -> P2_5
        if(not_pressed_P2_5_2 && !P2_5)
        {
            sleep();
            if(correct_sequence1 == 1)
            {
                correct_sequence2 = 1;
            }
            else
            {
                // jezeli spodziewalismy "7" a dostalismy co innego
                correct_sequence1 = 0;
            }
            pin_entry_in_progress = 1;
        }
        not_pressed_P2_5_2 = P2_5;

        // KLAWISZ 8 -> P2_6 (dzwonek, ale TYLKO gdy nie wpisujemy PIN-u)
        if(not_pressed_P2_6_2 && !P2_6)
        {
            sleep();
            if(!pin_entry_in_progress)
            {
                doorbell();
            }
            // Przerwanie sekwencji PIN
            correct_sequence1 = 0;
            correct_sequence2 = 0;
            correct_sequence3 = 0;
        }
        not_pressed_P2_6_2 = P2_6;

        // KLAWISZ 0 -> P2_7
        if(not_pressed_P2_7_2 && !P2_7)
        {
            sleep();
            pin_entry_in_progress = 1;
            if(correct_sequence1 == 1 && correct_sequence2 == 1)
            {
                correct_sequence3 = 1; // Mamy 7,5,0
            }
            else
            {
                // Sekwencja przerwana
                correct_sequence1 = 0;
                correct_sequence2 = 0;
            }
        }
        not_pressed_P2_7_2 = P2_7;
				
				// KLAWISZ 2 -> P2_4
        if(not_pressed_P2_4_2 && !P2_4)
        {
            sleep();
            correct_sequence1 = 0;
						correct_sequence2 = 0;
						correct_sequence3 = 0;
            pin_entry_in_progress = 1;
        }
        not_pressed_P2_4_2 = P2_4;

        //--------------------------------------------
        // Kolumna 3 (P2_1=0, P2_2=1, P2_3=1) –
        //--------------------------------------------
        P2_1 = 0; 
        P2_2 = 1; 
        P2_3 = 1;
				
				// KLAWISZ 3 -> P2_4
        if(not_pressed_P2_4_3 && !P2_4)
        {
            sleep();
            correct_sequence1 = 0;
						correct_sequence2 = 0;
						correct_sequence3 = 0;
            pin_entry_in_progress = 1;
        }
        not_pressed_P2_4_3 = P2_4;
				
				// KLAWISZ 6 -> P2_5
        if(not_pressed_P2_5_3 && !P2_5)
        {
            sleep();
            correct_sequence1 = 0;
						correct_sequence2 = 0;
						correct_sequence3 = 0;
            pin_entry_in_progress = 1;
        }
        not_pressed_P2_5_3 = P2_5;
				
				// KLAWISZ 9 -> P2_6
        if(not_pressed_P2_6_3 && !P2_6)
        {
            sleep();
            correct_sequence1 = 0;
						correct_sequence2 = 0;
						correct_sequence3 = 0;
            pin_entry_in_progress = 1;
        }
        not_pressed_P2_6_3 = P2_6;
				
        // KLAWISZ # -> P2_7 – reset wpisywania
        if(not_pressed_P2_7_3 && !P2_7)
        {
            sleep();
            correct_sequence1 = 0;
            correct_sequence2 = 0;
            correct_sequence3 = 0;
            pin_entry_in_progress = 0;
        }
        not_pressed_P2_7_3 = P2_7;
        
    }
}
