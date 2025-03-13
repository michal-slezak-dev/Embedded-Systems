#include <REGX52.H>

#define SPINS1_TO_2 18
#define SPINS2_TO_2 19
#define SPINS1_TO_1 20
#define SPINS2_TO_1 21

char pwmHelper1 = 0; // Timer 0 PWM helper
char pwmHelper2 = 0; // Timer 1 PWM helper

unsigned char spins1 = 0; // obroty silnika 1 (INT1)
unsigned char spins2 = 0; // obroty silnika 2 (INT0)

typedef enum {
    STATE_1,
    STATE_2,
} programState;

programState state = STATE_1;

// licznik obrotow INT0 dla silnika 2
void ISR_INT0() interrupt 0 {
    spins2++;
}

// licznik obrotow INT1 dla silnika 1
void ISR_INT1() interrupt 2 {
    spins1++;
}

// funkcja obslugi Timera 0
void ISR_T0() interrupt 1 {
    //TH0 = 250;

    if (state == STATE_1) {
        // silnik 2: 5/6 max predkosci
        // cykl = 6, wlaczone = 5
        if (pwmHelper1 < 5)
            P2_4 = 1;
        else
            P2_4 = 0;

        pwmHelper1++;
        if (pwmHelper1 == 6)
            pwmHelper1 = 0;

        // przejscie do stanu 2, po spelnieniu warunkow
        if (spins1 == SPINS1_TO_2 || spins2 == SPINS2_TO_2) {
            pwmHelper1 = 0;
            pwmHelper2 = 0;
            spins1 = 0;
            spins2 = 0;
            state = STATE_2;
        }

    } else {
        // silnik 1: 1/8 max predkosci
        // cykl = 8, wlaczone = 1
        if (pwmHelper1 < 1)
            P2_3 = 1;
        else
            P2_3 = 0;

        pwmHelper1++;
        if (pwmHelper1 == 8)
            pwmHelper1 = 0;

        // przejscie do stanu 1, po spelnieniu warunkow
        if (spins1 == SPINS1_TO_1 || spins2 == SPINS2_TO_1) {
            pwmHelper1 = 0;
            pwmHelper2 = 0;
            spins1 = 0;
            spins2 = 0;
            state = STATE_1;
        }
    }
}

// funkcja obslugi Timera 1
void ISR_T1() interrupt 3 {
    //TH1 = 250;

    if (state == STATE_1) {
        // silnik 1: 3/5 max predkosci
        // cykl = 5, wlaczone = 3
        if (pwmHelper2 < 3)
            P2_3 = 1;
        else
            P2_3 = 0;

        pwmHelper2++;
        if (pwmHelper2 == 5)
            pwmHelper2 = 0;

    } else {
        // silnik 2: 3/8 max predkosci
        // cykl = 8, wlaczone = 3
        if (pwmHelper2 < 3)
            P2_4 = 1;
        else
            P2_4 = 0;

        pwmHelper2++;
        if (pwmHelper2 == 8)
            pwmHelper2 = 0;
    }
}

void main(void) {
    P1 = 0;

    // INT0 (silnik 2), port 3_2
    IT0 = 1;
    EX0 = 1; // przerwania zewnetrzne

    // INT1 (silnik 1), port 3_3
    IT1 = 1;
    EX1 = 1; // przerwania zewnetrzne

    // Timer 0
    TL0 = 0;
    TH0 = 250;
    TR0 = 1;
    ET0 = 1;

    // Timer 1
    TL1 = 0;
    TH1 = 250;
    TR1 = 1;
    ET1 = 1;

    EA = 1; // wlaczam globalne przerwania

    while (1) {
        if (state == STATE_1) {
					// stan 1: Timer0 tryb 0, Timer1 tryb 0
            TMOD = 0x00;

            // kierunek obrotu silników w stanie 1:
            // silnik 1 przeciwnie
            P2_1 = 0;
            P2_2 = 1;

            // silnik 2 zgodnie
            P2_5 = 1;
            P2_6 = 0;
        } else {
					// stan 2: Timer0 tryb 2, Timer1 tryb 2
            TMOD = 0x22;

            // kierunek silników w stanie 2:
            // silnik 1 zgodnie
            P2_1 = 1;
            P2_2 = 0;

            // silnik 2 przeciwnie
            P2_5 = 0;
            P2_6 = 1;
        }
    }
}
