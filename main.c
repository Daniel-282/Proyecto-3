//---------------------------------
//Universidad del Valle de Guatemala
//
//Proyecto 3
///********************************
//librerias
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
//***************************************
#ifdef DEBUG
void
__error_(char *pcfilename, uint32_t ui32Line)
{
    while(1);

}
#endif

#define XTAL 16000000

uint32_t push1;
uint32_t push2;
uint32_t push3;
uint32_t push4;

uint32_t bandera1;
uint32_t bandera2;
uint32_t bandera3;
uint32_t bandera4;

uint32_t suma;
uint8_t indicadorP = 0;


void display(void);
void InitUART(void);
void parqueo(void);
void initGPIOF(void);

int main(void)
{

    SysCtlClockSet(
            SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); // Reloj de 40MHz
    InitUART(); // Configuracion de UART 0
    initGPIOF(); //configuracion de leds y puerto F

    while (1)
    {
        parqueo();
        display();
        UARTCharPut(UART5_BASE, indicadorP);
    }
}

void InitUART(void)
{
    /*Enable the GPIO Port A*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    /*Enable the peripheral UART Module 0*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);

    /* Make the UART pins be peripheral controlled. */
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    /* Sets the configuration of a UART. */
    UARTConfigSetExpClk(
            UART5_BASE, SysCtlClockGet(), 115200,
            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    //UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
}


void parqueo(void)
{
    //PRIMER PAR DE LEDS
    push1 = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4);
    if (((push1 & GPIO_PIN_4) == 0))
    {
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_PIN_1); //rojo-ocupado
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0);
        bandera1 = 0;
        indicadorP = indicadorP - 1;
    }
    if ((push1 & GPIO_PIN_4) != 0)
    {
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_PIN_0);  //azul-libre
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0);
        bandera1 = 1;
        indicadorP = indicadorP + 1;
    }
    //SEGUNDO PAR DE LEDS
    push2 = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7);
    if (((push2 & GPIO_PIN_7) == 0))
    {
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2); //rojo-ocupado
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0);
        bandera2 = 0;
        indicadorP = indicadorP - 2;
    }
    if ((push2 & GPIO_PIN_7) != 0)
    {
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3);  //azul-libre
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0);
        bandera2 = 1;
        indicadorP = indicadorP + 2;
    }
    //TERCER PAR DE LEDS
    push3 = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_6);
    if (((push3 & GPIO_PIN_6) == 0))
    {
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_PIN_0); //rojo-ocupado
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, 0);
        bandera3 = 0;
        indicadorP = indicadorP - 4;
    }
    if ((push3 & GPIO_PIN_6) != 0)
    {
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_PIN_6);  //azul-libre
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0);
        bandera3 = 1;
        indicadorP = indicadorP + 4;
    }
    //CUARTO PAR DE LEDS
    push4 = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_5);
    if (((push4 & GPIO_PIN_5) == 0))
    {
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1); //rojo-ocupado
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0);
        bandera4 = 0;
        indicadorP = indicadorP - 8;
    }
    if ((push4 & GPIO_PIN_5) != 0)
    {
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2);  //azul-libre
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);
        bandera4 = 1;
        indicadorP = indicadorP + 8;
    }
    suma = (bandera1 + bandera2 + bandera3 + bandera4);
}

void display(void)
{
    //MOSTRAR 4
    if (suma == 0)
    {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3| GPIO_PIN_4| GPIO_PIN_5| GPIO_PIN_6| GPIO_PIN_7, 0x8D);
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2| GPIO_PIN_3, 8);
    }
    //MOSTRAR 3
    if (suma == 1)
    {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3| GPIO_PIN_4| GPIO_PIN_5| GPIO_PIN_6| GPIO_PIN_7, 0xCB);
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2| GPIO_PIN_3, 12);
    }
    //MOSTRAR 2
    if (suma == 2)
    {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3| GPIO_PIN_4| GPIO_PIN_5| GPIO_PIN_6| GPIO_PIN_7, 0xDA);
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2| GPIO_PIN_3, 12);
    }
    //MOSTRAR 1
    if (suma == 3)
    {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3| GPIO_PIN_4| GPIO_PIN_5| GPIO_PIN_6| GPIO_PIN_7, 0x09);
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2| GPIO_PIN_3, 0);
    }
    //MOSTRAR 0
    if (suma == 4)
    {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0| GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3| GPIO_PIN_4| GPIO_PIN_5| GPIO_PIN_6| GPIO_PIN_7, 0x5F);
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2| GPIO_PIN_3, 4);
    }
}
//**************************INICIALIZA PUERTO F Y LEDS OUTPUTS-------------------
void initGPIOF(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,
    GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE,
    GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(
            GPIO_PORTD_BASE,
            GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_6
                    | GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(
            GPIO_PORTE_BASE,
            GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(
            GPIO_PORTB_BASE,
            GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4
                    | GPIO_PIN_5);
    GPIOPinTypeGPIOOutput(
            GPIO_PORTA_BASE,
            GPIO_PIN_2 | GPIO_PIN_3);
}
