#include <REGX52.H>
int released_P2_4_1 = 0;
int released_P2_6_1	= 0;
int released_P2_6_3 = 0;
int released_P2_7_3 = 0;
// 1 - nie wcisniety
int not_pressed_P2_7_1 = 1;
int not_pressed_P2_4_2 = 1;
int not_pressed_P2_5_2 = 1;
int not_pressed_P2_6_2 = 1;
int not_pressed_P2_7_2 = 1;
int not_pressed_P2_5_3 = 1;
int not_pressed_P2_4_3 = 1;
unsigned char data press_counter_P2_7_1 = 0;
unsigned char data press_counter_P2_7_2 = 0;
unsigned char data release_counter_P2_4_1 = 0;
unsigned char data release_counter_P2_7_3 = 0;
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

void main()
{
	int hold_time = 0;
	int blocked = 0;
	int blocked_seq = 0;
	int activation_counter = 0;
	int correct_sequence1 = 0;
	int correct_sequence2 = 0;
	int func_turned_off = 0;
	
	// kolumny na 0
	P2_1 = 0;
	P2_2 = 0;
	P2_3 = 0; 
	
	while(1)
		{
			if(blocked == 0 && blocked_seq == 0)
			{
				P2_1 = 1; // na poczatek 1. kolumna
				P2_2 = 1;
				P2_3 = 0;

				
				// co trzecie puszczenie blokuje klawiature. Odblokowanie wylacznie kolejno sekwencja				
				if(!P2_4)
				{
					while(1)
					{
						sleep();
						if(P2_4)
						{
							//release_counter_P2_4_1++;
							release_counter_P2_4_1 = (release_counter_P2_4_1 + 1) % 3;
							break;
						}
					}
					//if(release_counter_P2_4_1 != 0 && release_counter_P2_4_1 % 3 == 0)
						if(release_counter_P2_4_1 == 0)
						{
							blocked_seq = 1;
						}
				}
				
				// podwójne puszczenie zapala diode
				if(P2_5 == 1){
					sleep();
					double_release_counter++;
					if(P2_5 == 0){
						sleep();
						if(P2_5 == 1){
							sleep();
							double_release_counter++;
						}
					}
				}
				if(double_release_counter == 2){
					sleep();
					P0_0 = 0;
				}
				double_release_counter = 0;

				
				//puszczenie zapalenie, zgaszenie diody
				if(!P2_6)
				{
					sleep();
					sleep();
					while(1)
					{
						if(P2_6)
						{
							break;
						}
					}
					if(func_turned_off == 0)
							{
								sleep();
								P0_0 = 0;
								sleep();
								P0_0 = 1;
							}
				}
				
				/*if(released_P2_6_1 && P2_6)
				{
					if(func_turned_off == 0)
					{
						sleep();
						P0_0 = 0;
						sleep();
						P0_0 = 1;
						released_P2_6_1 = 0;
					}
				}*/
				
				// co drugie wcisniecie, w zaleznosci od aktywacji robi to co w dokumencie, co drugie klikniecie to dopiero jedna aktywacja
				if(!P2_7)
				{
					press_counter_P2_7_1++;
					
	
					if(press_counter_P2_7_1 != 0 && press_counter_P2_7_1 % 2 == 0)
					{
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

				

				
				P2_1 = 1; // 2. kolumna
				P2_2 = 0;
				P2_3 = 1;
				
				// wcisniecie blokuje klawiature na ok. 5s
				if(not_pressed_P2_4_2 && !P2_4)
				{
					block_5sec();
				}
				not_pressed_P2_4_2 = P2_4;
				
				//przytrzymanie na ok. 3s gasi diode
				if(P2_5 == 0)
				{
					sleep();
					while(P2_5 == 0)
					{
						sleep();
						hold_time++;
						if(hold_time > 3)
						{	
							sleep();
							P0_0 = 1;
							break;
						}
					}
				}
				not_pressed_P2_5_2 = P2_5;
				
				// wcisniecie zapala, gasi diode
				if(not_pressed_P2_6_2 && !P2_6)
				{
					if(func_turned_off == 0)
					{
						sleep();
						P0_0 = 0;
						sleep();
						P0_0 = 1;
					}						
				}
				not_pressed_P2_6_2 = P2_6;
				
				// co trzecie wcisniecie odblokowuje klawiature

				

				
				P2_1 = 0; // 3. koluumna
				P2_2 = 1;
				P2_3 = 1;
				
				//podwojne wcisniecie wylacza dzialanie funkcji przycisku o funkcji 3
					if(!P2_4)
					{
						sleep();
						double_press_counter++;
						if(P2_4)
						{
							if(!P2_4)
							{
								sleep();
								double_press_counter++;
							}
						}
						if(double_press_counter == 2)
						{
							func_turned_off = 1;
							double_press_counter = 0;
						}
						
					}


				
				// wcisniecie zmienia stan diody
				if(not_pressed_P2_5_3 && !P2_5)
				{	
					if(P0_0 == 1)
					{
						sleep();
						P0_0 = 0;
					}else
					{
						sleep();
						P0_0 = 1;
					}
				}
				not_pressed_P2_5_3 = P2_5;
				
				// puszczenie zapalenie, zgaszenie, zapaleniem, zgaszenie diody
				if(!P2_6)
				{
					while(1)
					{
						if(P2_6)
						{
							break;
						}

					}
						sleep();
						P0_0 = 0;
						sleep();
						P0_0 = 1;
						sleep();
						P0_0 = 0;
						sleep();
						P0_0 = 1;
				}
				

			
				// co drugie puszczenie blokuje klawiature
				//if(!P2_7)
				//{
					//released_P2_7_3 = 1;
				//}
				
				if(!P2_7)
				{
					sleep();
					while(1)
					{	
						if(P2_7)
						{
							//release_counter_P2_7_3++;
							release_counter_P2_7_3 = (release_counter_P2_7_3 + 1) % 2;
							break;
						}
					}
						//if(release_counter_P2_7_3 != 0 && release_counter_P2_7_3 % 2 == 0)
						if(release_counter_P2_7_3 == 0)
						{
							blocked = 1;
						}					
			}
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
				
				// co trzecie wcisniecie odblokowuje klawiature
				if(not_pressed_P2_7_2 && !P2_7)
				{
					press_counter_P2_7_2++;
					if(press_counter_P2_7_2 != 0 && press_counter_P2_7_2 % 3 == 0)
					{
						blocked = 0;
					}
				}
				not_pressed_P2_7_2 = P2_7;
				
				//kolumna 3
				P2_1 = 0;
				P2_2 = 1;
				P2_3 = 1;
				
			}else if(blocked_seq == 1) // odblokowanie 8 9 *
			{
				//kolumna 1
				P2_1 = 1;
				P2_2 = 1;
				P2_3 = 0;
				
				if(!P2_4){
					sleep();
					correct_sequence1 = 0;
					correct_sequence2 = 0;
			}
			
			if(!P2_5){
				sleep();
				correct_sequence1 = 0;
				correct_sequence2 = 0;
			}
			
			if(!P2_6){
				sleep();
				correct_sequence1 = 0;
				correct_sequence2 = 0;
			}
			
			//trzeci Z RZEDU poprawny, bo * w 1 kolumnie jest
			if(!P2_7){
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
				
				if(!P2_4){
					sleep();
					correct_sequence1 = 0;
					correct_sequence2 = 0;
				}
				if(!P2_5){
					sleep();
					correct_sequence1 = 0;
					correct_sequence2 = 0;
				}
				
				
				//pierwszy poprawny, bo 8 w 2. kolumnie
				if(!P2_6){
					sleep();
					correct_sequence1 = 1;
				}
				
				
				if(!P2_7){
					sleep();
					correct_sequence1 = 0;
					correct_sequence2 = 0;
				}

				
				//kolumna 3
				P2_1 = 0;
				P2_2 = 1;
				P2_3 = 1;
				
				if(!P2_4){
					sleep();
					correct_sequence1 = 0;
					correct_sequence2 = 0;
				}
			
				if(!P2_5){
					sleep();
					correct_sequence1 = 0;
					correct_sequence2 = 0;
				}
						
				
				//drugi poprawny, bo 9 w 3. kolumnie
				if(!P2_6){
					sleep();
					if(correct_sequence1 == 1){ // czy 8 wcisniete juz sprawdzamy
						correct_sequence2 = 1;
					}else{
						correct_sequence1 = 0;
					}
				}

				if(!P2_7){
					sleep();
					correct_sequence1 = 0;
					correct_sequence2 = 0;
				}
			}
		}
}