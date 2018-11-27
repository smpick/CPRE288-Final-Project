
//use UART1
//receive characters from Putty
//display on LCD when 20 characters received
//ignore enter key \r
//as putty receives characters, display number of stored characters in buffer on LCD (index: number characters stored)
//as putty receives characters, display the character being received (char: display char)


#include "uart.h"
#include "button.h"
#include "lcd.h"

void uart_init(void){
    //enable clock to GPIO, port B = R1 datasheet pg340
     SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;

     //enable clock to UART1, R1 = UART1. ***Must be done before setting Rx and Tx (See DataSheet)
     SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;

     //enable alternate functions on port b pins 0 and 1
     GPIO_PORTB_AFSEL_R |= (BIT0 | BIT1);

     //enable Rx and Tx on port B on pins 0 and 1
     GPIO_PORTB_PCTL_R |= 0x00000011;

     //set pin 0 and 1 to digital
     GPIO_PORTB_DEN_R |= (BIT0 | BIT1);

     //set pin 0 to Rx or input
     GPIO_PORTB_DIR_R &= ~BIT0;

     //set pin 1 to Tx or output
     GPIO_PORTB_DIR_R |= BIT1;

     //calculate baudrate
     uint16_t iBRD =   8;  //104;
     uint16_t fBRD =  44;   //11;

     //turn off uart1 while we set it up
     UART1_CTL_R &= ~(UART_CTL_UARTEN);

     //set baud rate
     UART1_IBRD_R = iBRD;
     UART1_FBRD_R = fBRD;

     //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
     UART1_LCRH_R = UART_LCRH_WLEN_8 ;

     //use system clock as source
     UART1_CC_R = UART_CC_CS_SYSCLK;

     //re-enable enable RX, TX, and uart1
     UART1_CTL_R = (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);
}


//Blocking call that sends 1 char over UART 1
void uart_sendChar(char data){
      //wait until there is room to send data
     while(UART1_FR_R & 0x20)
    {
    }
     //send data
     UART1_DR_R = data;

}

//Blocking call to receive over uart1
//returns char with data
char uart_receive(void){
    char data = 0;
    int button;
     //wait to receive
     while(UART1_FR_R & UART_FR_RXFE)
    {
         if (button != button_getButton()) {       //if a button is being pressed, go through this loop
                     button = button_getButton();
                     if (button == 6) {
                         lcd_printf("Yes");
                         uart_sendStr("Yes");
                     }
                     if (button == 5) {
                                     lcd_printf("No");
                                     uart_sendStr("No");
                                 }
                     if (button == 4) {
                                     lcd_printf("Blue, no green, Ahhhhh!!!");
                                     uart_sendStr("Blue, no green, Ahhhhh!!!");
                                 }
                     if (button == 3) {
                                     lcd_printf("Shelby is cool.");
                                     uart_sendStr("Shelby is cool.");
                                 }
                     if (button == 2) {
                                     lcd_printf("Amos is cool too.");
                                     uart_sendStr("Amos is cool too.");
                                 }
                     if (button == 1) {
                                     lcd_printf("2cooL4U");
                                     uart_sendStr("2cooL4U");
                                 }
                 }
    }
    //mask the 4 error bits and grab only 8 data bits
    data = (char)(UART1_DR_R & 0xFF);

    return data;
}

//sends string (char array) to uart
void uart_sendStr(const char *data){
    //While not equal to null
        while(*data != '\0')
        {
            //send char to uart
            uart_sendChar(*data);
            data++;
        }
}
