/*
 * servo_timer_init.c
 *
 *  Created on: Nov 27, 2018
 *      Author: smpick
 */
#include <servo.h>

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

void servo_sweep(void) {
    TIMER1_init();      //initializes servo motor
    clock_init();       //initializes servo clock
    sonar_init();
    ir_init();
    /* STEP ONE: Rotate Servo around from 0 to 180 */
    int clockDeg = 6500;   //clock ticks for 0 degrees
    int degrees = 0;       //actual degree value
    for (clockDeg = 6500; clockDeg < 35159; clockDeg = clockDeg + 318) {        //moves servo from 0 to 180 degrees
        if (degrees <= 0) {       //zero degree limit
            degrees = 0;
        }
        if (degrees >= 180) {       //180 degree limit
            degrees = 180;
        }
        degrees++;
        TIMER1_TBMATCHR_R = (320000 - clockDeg);
        timer_waitMillis(200);

        /* STEP TWO: Use IR to detect objects */
        int quantization;
        int irDistance;
        char string[50];
        ADC0_PSSI_R = ADC_PSSI_SS1;     //initiate SS1 conversion
        while((ADC0_RIS_R & ADC_RIS_INR1) == 0)  {
            //wait for ADC conversion to be complete
        }
        quantization = ADC0_SSFIFO1_R;  //digital value of voltage that is between 0 and 4096
        irDistance = 87293 * pow(quantization, -1.132); //IR sensor distance
        ADC0_ISC_R |= ADC_ISC_IN1;     //clears ????

        //sprintf(string, "\n%-24d", irDistance);
       // uart_sendStr(string);

        /*STEP THREE: Use Sonar to determine how far the object is away from the roomba */
        int dist;
        send_pulse();
        if (flag == 2) {    //volatile variable
            dist = (current_Event - last_Event)*0.0010625;
            sprintf(string, "\n%-24d %-16d %d", irDistance, dist, degrees*2);
            uart_sendStr(string);
        }
        flag = 0;

        /*IDEA: program such that the 's' key on the keyboard runs this function, this way
         * after the driver detects an object, they can let go of the 's' key, the servo
         * motor will stop, and the driver can have time to look at the sonar and ir data and
         * mark on the GUI where the object is on the course */


    }
}
