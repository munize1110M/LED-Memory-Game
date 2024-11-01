
#include "engr2350_msp432.h"
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
//using the Texas Instruments MSP432E4 MCU
void GPIO_Init();
void Timer_Init();
void show_sequence();
void Timer_ISR();
void Port4_ISR();
uint8_t Button_Press();
void toggle_green();
void toggle();
void show_once();


Timer_A_UpModeConfig test;

uint8_t yes_game;


uint32_t presses[6]={6,6,6,6,6,6};
uint32_t sequence[5];

uint8_t round_norm=1;

uint16_t timer_reset_counter;

uint16_t bmp0=1;
uint16_t bmp1=1;
uint16_t bmp2=1;
uint16_t bmp3=1;
uint16_t bmp4=1;
uint16_t bmp5=1;

uint16_t var=6;

int main (void) /* Main Function */
 {
    SysInit();
    GPIO_Init();
    Timer_Init();

    
    printf("Instructions");

    GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN4);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN6);


    while(yes_game==0){
        Port4_ISR();
        yes_game=GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN3);
    }

    yes_game=0;
    while(1){

        printf("\n%d\n",round_norm);
        if (round_norm==1){
            uint8_t j=0;
            srand(time(NULL));
            for(j=0; j<5; j++){
                sequence[j] = rand()%(5+1-0)+0;
                printf("%d",sequence[j]);
            }
            show_once();
        }
        GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
        while(timer_reset_counter<3){

        }
        timer_reset_counter=0;
        GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN4);
        GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN6);
        show_sequence();
//BiLED turned red
        //now print out sequence
        GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN6);
        GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN4);

        uint8_t x;
        uint8_t cnt=5-round_norm;
        while(timer_reset_counter<30){
            x=Button_Press();

            //printf("%d",x);
            if ((x==0)||(x==1)||(x==2)||(x==3)||(x==4)||(x==5)){
                presses[cnt]=x;
                cnt++;
            }
            if(x==7){
                while(timer_reset_counter<3){

                }
                timer_reset_counter=0;
                show_once();
                round_norm--;

            }

        }

        timer_reset_counter=0;
        uint8_t v=5-round_norm;
        while(v<5){
        if (presses[v]!=sequence[v]){
            printf("INCORRECT");
            while(yes_game==0){
                toggle();
                yes_game=GPIO_getInputPinValue(GPIO_PORT_P3,GPIO_PIN3);
            }
            yes_game=0;
            round_norm=0;
            //printf("\nRound norm check %d\n", round_norm);
        }

        v++;
        }
        if ((round_norm==5)){
           printf("Win");
           while(yes_game==0){
               toggle_green();
               yes_game=GPIO_getInputPinValue(GPIO_PORT_P3,GPIO_PIN3);
           }
           yes_game=0;
           round_norm=0;
        }

        timer_reset_counter=0;

        //show_sequence();
        round_norm++;
        while(timer_reset_counter<5){

        }
        timer_reset_counter=0;
    }
}



void GPIO_Init (){
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2); //ON board LED
    GPIO_setAsInputPin(GPIO_PORT_P3,GPIO_PIN3);//Pushbutton set as input pin
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4|GPIO_PIN6);//sets BiLED stuff to output

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN0|GPIO_PIN2|GPIO_PIN3|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    //^sets bumpers as inputs

}

void show_sequence(){
    uint8_t print_len=5-round_norm;
    uint8_t i=0;
    //printf("IN SHOW SEQUENCE");
    while(i<print_len){
        if(sequence[i]==0){
            //printf("red");
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0);//red
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN1|GPIO_PIN2);
            while(timer_reset_counter<5){

            }
            timer_reset_counter=0;
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
            while(timer_reset_counter<3){

            }
            timer_reset_counter=0;
        }else if(sequence[i]==1){
            //printf("green");
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1);//green
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN2);
            while(timer_reset_counter<5){

            }
            timer_reset_counter=0;
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
            while(timer_reset_counter<3){

            }
            timer_reset_counter=0;
        }else if(sequence[i]==2){
            //printf("blue");
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN2);//blue
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1);
            while(timer_reset_counter<5){

            }
            timer_reset_counter=0;
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
            while(timer_reset_counter<3){

            }
            timer_reset_counter=0;
        }else if(sequence[i]==3){
            //printf("yellow");
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1);//yellow
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN2);
            while(timer_reset_counter<5){

            }
            timer_reset_counter=0;

            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
            while(timer_reset_counter<3){

            }
            timer_reset_counter=0;
        }else if(sequence[i]==4){
            //printf("cyan");
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1|GPIO_PIN2);//cyan
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0);
            while(timer_reset_counter<5){

            }
            timer_reset_counter=0;

            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
            while(timer_reset_counter<3){

            }
            timer_reset_counter=0;
        }else if(sequence[i]==5){
            //printf("magenta");
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN2);//magenta
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN1);
            while(timer_reset_counter<5){

            }
            timer_reset_counter=0;

            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
            while(timer_reset_counter<3){

            }
            timer_reset_counter=0;
        }
        i++;
    }
}

void Port4_ISR(){
    __delay_cycles(240e3); // 10 ms delay (24 MHz clock)

        // Next: get the list of pins that may have triggered the interrupt
    uint8_t active_pins = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);

        // Check to see if GPIO_PIN1 interrupt is active

            // Clear the GPIO_PIN1 interrupt

        if(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN0)){
            GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN0);
            while (!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN0)){



            //printf("BMP0");
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN1|GPIO_PIN2);
            }
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
        }



        // Repeat for GPIO_PIN3 (P3.3).


        if(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN2)){
            GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN2);
            // P3.3 Press detected, do whatever is needed
            while(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN2)){

            //printf("BMP1");
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN2);
            }
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
        }

        if(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN3)){
            GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN3);

            while(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN3)){
                //printf("BMP2");
                GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN2);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1);
            }
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
        }

        if(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN5)){
            GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN5);

            while(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN5)){
                //printf("BMP3");
                GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1);//yellow
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
            }
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
        }

        if(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN6)){
            GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN6);

            while(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN6)){
                //printf("BMP4");
                GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1|GPIO_PIN2);//cyan
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
            }
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
        }

        if(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN7)){
            GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN7);

            while(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN7)){
                //printf("BMP5");
                GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN2);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN1);
            }
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
        }

}


uint8_t Button_Press(){
    __delay_cycles(240e3); // 10 ms delay (24 MHz clock)


    uint8_t active_pins = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);

        // Check to see if GPIO_PIN1 interrupt is active

            // Clear the GPIO_PIN1 interrupt

        if(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN0)){
            GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN0);
            while (!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN0)){



            //printf("BMP0");
            bmp0=0;

            var=0;
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN1|GPIO_PIN2);
            }
            //GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
            return 0;
        }



        // Repeat for GPIO_PIN3 (P3.3).


        if(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN2)){
            GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN2);
            // P3.3 Press detected, do whatever is needed
            while(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN2)){

            //printf("BMP1");
            bmp1=0;

            var=1;
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN2);
            }
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
            return 1;

        }

        if(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN3)){
            GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN3);

            while(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN3)){
                //printf("BMP2");
                bmp2=0;

                var=2;
                GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN2);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1);
            }
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
            return 2;

        }

        if(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN5)){
            GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN5);

            while(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN5)){
                //printf("BMP3");
                bmp3=0;

                var=3;
                GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1);//yellow
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);

            }

            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2|GPIO_PIN0|GPIO_PIN1);
            return 3;

        }

        if(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN6)){
            GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN6);

            while(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN6)){
                //printf("BMP4");
                bmp4=0;

                var=4;
                GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1|GPIO_PIN2);//cyan
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);

            }

            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
            return 4;

        }

        if(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN7)){
            GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN7);

            while(!GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN7)){
                //printf("BMP5");
                bmp5=0;
                GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN2);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN1);


            }

            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN1|GPIO_PIN2|GPIO_PIN0);
            return 5;
        }
        if(GPIO_getInputPinValue(GPIO_PORT_P3,GPIO_PIN3)==1){
            return 7;
        }
        else{
            return 6;
        }

}

void Timer_ISR(){
    Timer_A_clearInterruptFlag(TIMER_A0_BASE);
    timer_reset_counter++;




}
void Timer_Init(){
    test.clockSource=TIMER_A_CLOCKSOURCE_SMCLK;
    test.clockSourceDivider=TIMER_A_CLOCKSOURCE_DIVIDER_64;
    test.timerPeriod=46875;
    //printf("%d", test.timerPeriod);
    test.timerClear=TIMER_A_DO_CLEAR;
    test.timerInterruptEnable_TAIE=TIMER_A_TAIE_INTERRUPT_ENABLE;

    Timer_A_configureUpMode(TIMER_A0_BASE, &test);
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
    Timer_A_registerInterrupt(TIMER_A0_BASE,TIMER_A_CCRX_AND_OVERFLOW_INTERRUPT,Timer_ISR);

}

void show_once(){
    uint8_t i=0;
    while(i<5){
        if(sequence[i]==0){
            //printf("red");
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0);//red
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN1|GPIO_PIN2);
            while(timer_reset_counter<5){

            }
            timer_reset_counter=0;
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
            while(timer_reset_counter<3){

            }
            timer_reset_counter=0;
        }else if(sequence[i]==1){
            //printf("green");
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1);//green
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN2);
            while(timer_reset_counter<5){

            }
            timer_reset_counter=0;
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
            while(timer_reset_counter<3){

            }
            timer_reset_counter=0;
        }else if(sequence[i]==2){
            //printf("blue");
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN2);//blue
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1);
            while(timer_reset_counter<5){

            }
            timer_reset_counter=0;
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
            while(timer_reset_counter<3){

            }
            timer_reset_counter=0;
        }else if(sequence[i]==3){
            //printf("yellow");
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1);//yellow
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN2);
            while(timer_reset_counter<5){

            }
            timer_reset_counter=0;

            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
            while(timer_reset_counter<3){

            }
            timer_reset_counter=0;
        }else if(sequence[i]==4){
            //printf("cyan");
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1|GPIO_PIN2);//cyan
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0);
            while(timer_reset_counter<5){

            }
            timer_reset_counter=0;

            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
            while(timer_reset_counter<3){

            }
            timer_reset_counter=0;
        }else if(sequence[i]==5){
            //printf("magenta");
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN2);//magenta
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN1);
            while(timer_reset_counter<5){

            }
            timer_reset_counter=0;

            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
            while(timer_reset_counter<3){

            }
            timer_reset_counter=0;
        }
        i++;
    }
}

void toggle(){
    GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN4|GPIO_PIN6);
    while(timer_reset_counter<3){

    }
    timer_reset_counter=0;
    GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN4);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN6);
    while(timer_reset_counter<3){

    }
    timer_reset_counter=0;
}

void toggle_green(){
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4|GPIO_PIN6);
    while(timer_reset_counter<3){

    }
    timer_reset_counter=0;
    GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN6);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN4);
    while(timer_reset_counter<3){

    }
    timer_reset_counter=0;
}



