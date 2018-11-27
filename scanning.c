/*
 * scanning.c
 *
 *  Created on: Oct 16, 2018
 *      Author: smpick
 */

#include "stdio.h"
#include "lcd.h"
#include "timer.h"
#include "inc/tm4c123gh6pm.h"
#include "stdbool.h"
#include "math.h"
#include "driverlib/interrupt.h"
#include "uart.h"
#include "movement.h"
#include "stdlib.h"
#include "open_interface.h"

#define TIMER3B_PRESCALER 0xFF
//#include "SERVO 2.h"

void TIMER1_init(void){
    SYSCTL_RCGCGPIO_R |= 0b00000010; //turn on clk for port B
    SYSCTL_RCGCTIMER_R |= 0x00000002; //turn on clk for port B
    timer_waitMillis(200);

    GPIO_PORTB_AFSEL_R |= 0b00100000;  //alternate function select
    GPIO_PORTB_PCTL_R |= 0x00700000;    //selects peripheral
    GPIO_PORTB_DEN_R |= 0b00100000;
    GPIO_PORTB_DIR_R |= 0b00100000;  //sets pin5 as output

    TIMER1_TBPR_R = 0;
    TIMER1_CTL_R &= ~(TIMER_CTL_TBEN);  //clears TbEN bit for configuring
    TIMER1_TBMR_R |= 0x0000000A; //periodic and PWM enable -- sets TBAMS to 1, TBCMR to 0, TBME to 2
    TIMER1_CFG_R |= 0x00000004; //according to pwm config, this needs to be set this way
    TIMER1_TBPR_R = 320000 >> 16; //set the upper 8 bits of the interval
    TIMER1_TBILR_R = 320000; //lower 16 bits of the interval
    TIMER1_TBPMR_R = (320000 - 16000) >> 16;
    TIMER1_CTL_R |= 0xC00;
    TIMER1_TBMATCHR_R = (320000 - 6500);  //condition to move bot to zero degrees position

    //TIMER1_TBMATCHR_R = pulse_period - mid_width; // if you want to move servo  to the middle
    //TIMER1_TBMATCHR_R = pulse_period - min_width; // if you want to move servo  to 0 degrees
    //TIMER1_TBMATCHR_R = pulse_period - max_width; // if you want to move servo  to 180 degrees

    TIMER1_CTL_R |= TIMER_CTL_TBEN;   //enable timer
}

volatile int flag = 0;  //variable to tell when rise or fall occurs
volatile unsigned long last_Event = 0; //tracks when the last flag occurred
volatile unsigned long current_Event = 0;  //tracks the time the new event occurs
volatile int rising = 1;

void TIMER3B_Handler(void) {
    if ( TIMER3_MIS_R & TIMER_MIS_CBEMIS) {  //port b input interrupt flag check

        TIMER3_ICR_R = TIMER_ICR_CBECINT; //clears port b flag

        if (rising)   {
            last_Event = TIMER3_TBR_R | (TIMER3_TBPS_R << 16);  //compares stored value to presacaler, shifted 16 bits
            rising = 0;   //change from rise to fall
        }
        else     {
            current_Event = TIMER3_TBR_R | (TIMER3_TBPS_R << 16);
            //TIMER3_CTL_R &= ~( TIMER_CTL_TBEN);    //disable timerB to allow configuring
            rising = 1;         //change from fall to rise
        }
        flag++;
    }
}

void clock_init(void) {
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3;  //enables TIMER3
    SYSCTL_RCGCGPIO_R |= 0b00000010;  //port b
    TIMER3_CTL_R &= ~(TIMER_CTL_TBEN);  //disable for config
    TIMER3_CFG_R |= TIMER_CFG_16_BIT;  //sets to 16-bit mode
    TIMER3_CTL_R |= TIMER_CTL_TBEVENT_BOTH;   ///enable and edge select
    //TIMER3_MIS_R |= 0xFF;
    //TIMER3_TBPR_R = TIMER3B_PRESCALER;
    TIMER3_TBMR_R |= 0x17;    //chooses count direction as up, capture mode enabled, edge-time mode enabled, periodic timer mode
    TIMER3_TBILR_R |= 0xFFFF;  //timer b interval load
    TIMER3_IMR_R |= TIMER_IMR_CBEIM;  //timer b capture mode interrupt mask
    TIMER3_ICR_R |= TIMER_ICR_CBECINT;  //timer b capture mode event interrupt clear

    NVIC_EN1_R = 0x00000010;  //interrupt enable

    IntRegister(INT_TIMER3B, TIMER3B_Handler);
    IntMasterEnable();

    TIMER3_CTL_R |= TIMER_CTL_TBEN;  //enable
}

void sonar_init(void) {

    GPIO_PORTB_DEN_R |= 0b00001000; //port b bit 3

    GPIO_PORTB_AFSEL_R |= 0b00001000; //bit 3  enabling alt functions
    GPIO_PORTB_PCTL_R |= 0x00007000;

}

void send_pulse(void) {
    /* send out a pulse on PB3 */
    TIMER3_CTL_R &= ~(TIMER_CTL_TBEN);  //starts timer b
    //GPIO_PORTB_PCTL_R &= ~0x0000700;  //chooses alternate function???????

    GPIO_PORTB_AFSEL_R &= ~0b00001000;  //pin 3 on port b set as alternate function disabled
    GPIO_PORTB_DIR_R |= 0x08; // set PB3 as output
    GPIO_PORTB_DATA_R |= 0x08; // set PB3 to high

    // wait at least 5 microseconds based on data sheet
    timer_waitMicros(10);

    GPIO_PORTB_DATA_R &= 0xF7; // set PB3 to low
    GPIO_PORTB_DIR_R &= 0xF7; // set PB3 as input

    GPIO_PORTB_AFSEL_R |= 0b00001000; //enables alt function
    GPIO_PORTB_PCTL_R |= 0x00007000;

    TIMER3_CTL_R |= TIMER_CTL_TBEN;

    //timer_waitMillis(1000);

}

void ir_init (void) {
    //enable ADC 0 module on port PB4
    // SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    SYSCTL_RCGCGPIO_R |= 0b000010;

    //enable clock for ADC
    SYSCTL_RCGCADC_R |= 0x1;

    //enable port B pin 4 to work as alternate function
    GPIO_PORTB_AFSEL_R |= 0x10;

    //set pin to input - 0
    GPIO_PORTB_DEN_R &= 0xEF;

    //disable analog isolation for the pin
    GPIO_PORTB_AMSEL_R |= 0x10;

    //set direction of pin
    // GPIO_PORTB_DIR_R = 0xEF;

    //initialize the port trigger source as processor (default)
    GPIO_PORTB_ADCCTL_R = 0x00;

    //disable SS1 sample sequencer to configure it
    // ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN1;
    ADC0_ACTSS_R &= ~(0x00000002);

    //initialize the ADC trigger source as processor (default)
    // ADC0_EMUX_R = ADC_EMUX_EM1_PROCESSOR;
    ADC0_EMUX_R = ~(0x000000F0);

    // Sample sequence Control
    // ADC0_ADCSSCTL_R = 0x0007;

    // set 1st sample to use the AIN10 ADC pin
    ADC0_SSMUX1_R |= 0x000A;

    //enable raw interrupt staus
    ADC0_SSCTL1_R |= (0x0004 | 0x0002);

    // enable oversampling to average
    // ADC0_SAC_R |= ADC_SAC_AVG_64X;
    ADC0_SAC_R |= 0x4;

    // re-enable ADC0 SS1
    // ADC0_ACTSS_R |= ADC_ACTSS_ASEN1;
    ADC0_ACTSS_R |= 0x00000002;
}


void main() {
    uart_init();  //initialize uart1
    TIMER1_init();
    lcd_init();
    clock_init();
    sonar_init();
    ir_init();
    rising = 1;

    float dist;         //variable storing sonar distance in clock ticks
    float sonarDistance;
    int degrees;  //number clock ticks for degrees
    int deg = 0;
    int quantization = 0;  //quantization value
    int irDistance = 0;     //distance in cm
    int arrayDegrees[90];
    int arrayIr[90];
    int arraySonar[90];
    //char string[50];
    //uart_sendStr("\nDegrees   IR Distance (cm)      Sonar Distance (cm) \n");

    while(1) {
        int i = 0;
        for (degrees = 6500; degrees < 35159; degrees = degrees + 318) {        //moves servo from 0 to 180 degrees
            if (deg <= 0) {       //zero degree limit
                deg = 0;
            }
            if (deg >= 180) {       //180 degree limit
                deg = 0;
            }
            TIMER1_TBMATCHR_R = (320000 - degrees);
            send_pulse();
            timer_waitMillis(75);

            if (flag == 2) {    //volatile variable
                dist = current_Event - last_Event;
                sonarDistance = (dist/32000000)*34000;      //calcs and stores sonar distance
                flag = 0;       //resets flag
                ADC0_PSSI_R = ADC_PSSI_SS1;     //initiate SS1 conversion
                while((ADC0_RIS_R & ADC_RIS_INR1) == 0)  {
                    //wait for ADC conversion to be complete
                }
                //grab result
                quantization = ADC0_SSFIFO1_R;  //digital value of voltage that is between 0 and 4096
                irDistance = 87293 * pow(quantization, -1.132); //IR sensor distance
                ADC0_ISC_R |= ADC_ISC_IN1;     //clears ????
                GPIO_PORTB_DATA_R = quantization >> 9;  //ensuring it only measures an object 9-50cm away
                timer_waitMillis(75);
                //degrees = degrees + 318;        //increments two degrees
                deg = (degrees - 6500) / 159;   //converts clocktick degrees to actual degrees
                //deg = deg +2;
                //sprintf(string, "%-10d %-24d %-13f\n", deg, irDistance, sonarDistance);
                // uart_sendStr(string);
                //lcd_printf("Count is =%d \nDegrees = %d\nIR = %d\nPing = %3.2f", degrees, deg, irDistance, sonarDistance);

                arrayDegrees[i] = deg;      //stores degree value
                arrayIr[i] = irDistance;    //stores ir distance at that degree value
                arraySonar[i] = sonarDistance;      //stores sonar distance at that degree value
                i++;
            }
        }
        //  float coss;
        int edge = 0;
        int widthDeg[10];       //array for tracking object widths in degrees
        int widthCM[10];
        int dis[10];
        int j = 0;
        int degree1;        //degree at start of object
        int degree2;        //degree at end of object
        int smallest = 1;
        int dir[10];        //the center of the object in degree value
        int numObjects;
        int centerDist[10];     //distance to center of object in cm

        for (i = 1; i <= 89; i++)   {
            if (arrayIr[i - 1] > 80 && arrayIr[i] <= 80 && edge == 0)    {     //hit an object
                degree1 = arrayDegrees[i];
                dis[j] = arraySonar[i];          //dis is distance in cm from first edge
                edge = 1;
            }
            if (arrayIr[i - 1] < 80 && arrayIr[i] >= 80 && edge == 1)   {     //moving away from an object
                degree2 = arrayDegrees[i];
                widthDeg[j] = degree2 - degree1;
                dir[j] = ((degree2 + degree1)/2);       //stores center degree location of object
                j++;
                edge = 0;
            }
        }

        numObjects = j;
        for (i=2; i<=j; i++) {
            if (widthDeg[smallest] > widthDeg[i])   {
                smallest = i;
            }
        }
        widthCM[smallest] = abs(2*(dis[smallest])*sin(dir[smallest]));
        centerDist[smallest] = abs(dis[smallest]*cos(widthDeg[smallest]/2));  //distance to center of object

        lcd_printf("Object Count: %d \nDist(cm): %d\n", numObjects, centerDist[smallest]);
     //   move_servo(dir[smallest]);

        timer_waitMillis(100000);

    }

}

