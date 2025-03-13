#include <REGX52.H>
int released_P2_4_1 = 0;
int released_P2_6_2 = 0;
int released_P2_7_2 = 0;
int not_pressed_P2_6_1 = 1; // 1 - nie wcisniety
int not_pressed_P2_4_2 = 1;
int not_pressed_P2_5_2 = 1;
int not_pressed_P2_6_3 = 1;
unsigned char data press_counter_P2_4_2 = 0;
unsigned char data press_counter_P2_5_2 = 0;
unsigned char data release_counter_P2_6_2 = 0;
unsigned char data release_counter_P2_7_2 = 0;
unsigned char data double_release_counter = 0;
unsigned char data double_press_counter = 0;

void sleep()
{
	int i, j;
	for(i = 0; i < 70; i++)
	{
		for(j = 0; j < 70; j++){;}
	}
}

void block_5sec(){
	int x, y;
	for(x = 0; x < 300; x++){
		for(y = 0; y < 300; y++){
		}
	}
}

void sleep_double_click()
{
	int i, j;
	for(i = 0; i < 70; i++)
	{
		for(j = 0; j < 70; j++){;}
	}
}

void main()
{
	int hold_time = 0;
	int blocked = 0;
	int blocked_seq = 0;
	int activation_counter = 0;
	int correct_sequence1 = 0;
	int correct_sequence2 = 0;
	int func_turned_off = 0;
	//P1 = 255 - 2; // tylko P1_1? od maxa - 2, zamienia dziesietna na bitowa i zrzuca na caly port
	// odczytanie wartosci z portu
	//int a = P1;
	
	// kolumny na 0
	P2_1 = 0;
	P2_2 = 0;
	P2_3 = 0; // 1 - na p2_4 kliknalem jedynke
	
	while(1)
		{
			if(blocked == 0 && blocked_seq == 0)
			{
				P2_1 = 1; // na poczatek 1. kolumna
				P2_2 = 1;
				P2_3 = 0;

				
				// puszczenie gasi diode
				/*if(P2_4 == 0)
				{
					while(1)
					{
						if(P2_4 == 1)
						{
							break;
						}
					}
					sleep();
					P0_0 = 1;
				}*/
				if(!P2_4)
				{
					released_P2_4_1 = 1;
				}
				
				if(released_P2_4_1 && P2_4)
				{
					P0_0 = 1;
					released_P2_4_1 = 0;
				}
				
				// przytrzymanie zapala diode
				if(!P2_5)
				{
					P0_0 = 0;
				}/*else
				{
					P0_0 = 1;
				}*/
				
				//wcisniecie zmienia stan diody
				if(not_pressed_P2_6_1 && !P2_6)
				{
					if(func_turned_off == 0)
					{
						if(P0_0 == 1)
						{
							P0_0 = 0;
						}else
						{
							P0_0 = 1;
						}
					}
					}
				not_pressed_P2_6_1 = P2_6;
				
				// wcisniecie wylacza funkcje 2
				if(P2_7 == 0)
				{
					sleep();
					func_turned_off = 1;
				}

				
				P2_1 = 1; // 2. kolumna
				P2_2 = 0;
				P2_3 = 1;
				
				//co drugie wcisniecie zapalenie, zgaszenie diody
				if(not_pressed_P2_4_2 && !P2_4)
				{
					press_counter_P2_4_2++;
					if(press_counter_P2_4_2 != 0 && press_counter_P2_4_2 % 2 == 0)
					{
						sleep();
						P0_0 = 0;
						sleep();
						P0_0 = 1;
					}
				}
				not_pressed_P2_4_2 = P2_4;
				
				//co trzecie wcisniecie zapalenie, zgaszenie, zapalenie, zgaszenie diody
				if(not_pressed_P2_5_2 && !P2_5)
				{
					press_counter_P2_5_2++;
					if(press_counter_P2_5_2 != 0 && press_counter_P2_5_2 % 3 == 0)
					{
						sleep();
						P0_0 = 0;
						sleep();
						P0_0 = 1;
						sleep();
						P0_0 = 0;
						sleep();
						P0_0 = 1;
					}
				}
				not_pressed_P2_5_2 = P2_5;
				
				// co drugie puszczenie blokuje klawiature
				if(!P2_6)
				{
					released_P2_6_2 = 1;
				}
				
				if(released_P2_6_2 && P2_6)
				{
					release_counter_P2_6_2++;
					
					if(release_counter_P2_6_2 != 0 && release_counter_P2_6_2 % 2 == 0)
					{
						sleep();
						blocked = 1;
					}
					released_P2_6_2 = 0;
				}
				
				// co trzecie puszczenie
				if(!P2_7)
				{
					released_P2_7_2 = 1;
				}
				
				if(released_P2_7_2 && P2_7)
				{
					//release_counter_P2_7_2++;
					release_counter_P2_7_2 = (release_counter_P2_7_2 + 1) % 3;
					//if(release_counter_P2_7_2 != 0 && release_counter_P2_7_2 % 3 == 0)
					if(release_counter_P2_7_2 == 0)
					{
						P1 = 0;
					}
					released_P2_7_2 = 0;
				}
				

				
				P2_1 = 0; // 3. koluumna
				P2_2 = 1;
				P2_3 = 1;
				
				//przytrzymanie na ok. 3 sekundy blokuje na ok. 5s klawe
				if(P2_4 == 0)
				{
					sleep();
					while(P2_4 == 0)
					{
						sleep();
						hold_time++;
						if(hold_time > 3)
						{	
							block_5sec();
							hold_time = 0;
							break;
						}
					}
				}
				
				// podwojne puszczenie nastepuje aktywacja przycisku
				if(P2_5 == 0)
				{
					sleep();
					if(P2_5 == 1)
					{
						sleep();
						double_release_counter++;
						if(P2_5 == 0)
						{
							sleep();
							if(P2_5 == 1)
							{
								sleep();
								double_release_counter++;
							}
						}
						if(double_release_counter == 2)
						{
							sleep();
							activation_counter++;
							if(activation_counter % 3 == 0)
							{
								sleep();
								P0_0 = 0;
							}else if(activation_counter % 3 == 1)
							{
								sleep();
								P0_0 = 1;
							}else if(activation_counter % 3 == 2)
							{
								sleep();
								P0_0 = 0;
								sleep();
								P0_0 = 1;
							}
						}
					}
					double_release_counter = 0; // zerujemy na koniec
				}
				
				//podwojne wcisniecie blokuje klawiature z sekwencja do odblokowania
				if(not_pressed_P2_6_3 && !P2_6)
				{
					sleep_double_click();
					if(!not_pressed_P2_6_3 || P2_6)
					{
						sleep_double_click();
						double_press_counter++;
						if(not_pressed_P2_6_3 && !P2_6)
						{
							sleep_double_click();
							if(!not_pressed_P2_6_3 || P2_6)
							{
								sleep_double_click();
								double_press_counter++;
							}
						}
					}
					if(double_press_counter == 2)
					{
						blocked_seq = 1;
						/*sleep();
						P0_0 = 0;
						sleep();
						P0_0 = 1;
						sleep();
						P0_0 = 0;
						sleep();
						P0_0 = 1;*/
					}
					double_press_counter = 0;
				}
				not_pressed_P2_6_3 = P2_6;
			}else if(blocked == 1)
			{
				//kolumna 1
				P2_1 = 1;
				P2_2 = 1;
				P2_3 = 0;
				
				
				//kolumna 2
				P2_1 = 1;
				P2_2 = 0;
				P2_3 = 1;
				
				// co trzecie puszczenie odblokowuje klawiature
				if(!P2_7)
				{
					released_P2_7_2 = 1;
				}
				
				if(released_P2_7_2 && P2_7)
				{
					//release_counter_P2_7_2++;
					release_counter_P2_7_2 = (release_counter_P2_7_2 + 1) % 3;
					//if(release_counter_P2_7_2 != 0 && release_counter_P2_7_2 % 3 == 0)
					if(release_counter_P2_7_2 == 0)
					{
						blocked = 0;
					}
					released_P2_7_2 = 0;
				}
				
				//kolumna 3
				P2_1 = 0;
				P2_2 = 1;
				P2_3 = 1;
			}else if(blocked_seq == 1) // odblokowanie 6 2 *
			{
				//kolumna 1
				P2_1 = 1;
				P2_2 = 1;
				P2_3 = 0;
				
				if(P2_4 == 0){
					sleep();
					correct_sequence1 = 0;
					correct_sequence2 = 0;
			}
			
			if(P2_5 == 0){
				sleep();
				correct_sequence1 = 0;
				correct_sequence2 = 0;
			}
			
			if(P2_6 == 0){
				sleep();
				correct_sequence1 = 0;
				correct_sequence2 = 0;
			}
			
			//trzeci Z RZEDU poprawny, bo * w 1 kolumnie jest
			if(P2_7 == 0){
				sleep();
				if(correct_sequence1 == 1){
					if(correct_sequence2 == 1){
						blocked_seq = 0;
						correct_sequence1 = 0;
						correct_sequence2 = 0;
					}else{
						correct_sequence1 = 0;
						correct_sequence2 = 0;
					}
				}
			}			
				
				//kolumna 2
				P2_1 = 1;
				P2_2 = 0;
				P2_3 = 1;
			
				//drugi poprawny, bo 2 w 2. kolumnie
				if(P2_4 == 0){
					sleep();
					if(correct_sequence1 == 1){ // 6 wcisniete juz sprawdzamy
						correct_sequence2 = 1;
					}else{
						//nacisniety byl kolejny przycisk zatem wtedy on staje sie pierwszym (a nie jest pierwszym do odblokowania)
						correct_sequence1 = 0;
					}
				}
				
				if(P2_5 == 0){
					sleep();
					correct_sequence1 = 0;
					correct_sequence2 = 0;
				}
				
				if(P2_6 == 0){
					sleep();
					correct_sequence1 = 0;
					correct_sequence2 = 0;
				}
				
				if(P2_7 == 0){
					sleep();
					correct_sequence1 = 0;
					correct_sequence2 = 0;
				}

				
				//kolumna 3
				P2_1 = 0;
				P2_2 = 1;
				P2_3 = 1;
				
				if(P2_4 == 0){
					sleep();
					correct_sequence1 = 0;
					correct_sequence2 = 0;
				}
			
				//pierwszy poprawny, bo 6 w 3. kolumnie
				if(P2_5 == 0){
					sleep();
					correct_sequence1 = 1;
				}
				
				if(P2_6 == 0){
					sleep();
					correct_sequence1 = 0;
					correct_sequence2 = 0;
				}
				
				if(P2_7 == 0){
					sleep();
					correct_sequence1 = 0;
					correct_sequence2 = 0;
				}

			}
		}
}