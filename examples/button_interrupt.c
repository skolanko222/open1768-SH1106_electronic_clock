#include "LPC17xx.h"


#include <cstring> // std::memcpy
#include <array>  // 

#ifdef __cplusplus
extern "C" {
#endif



void delay_ms(unsigned int ms)
{
    volatile unsigned int i,j;
    for(i=0;i<ms;i++)
    for(j=0;j<6000;j++);
}


void ToggleLED()
{
    #define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

    // check if bit is set, then toggle it depending on result.
    if(CHECK_BIT(LPC_GPIO1->FIOPIN, 28))
    {
        LPC_GPIO1->FIOCLR |= (1 << 28);
    }
    else
    {
        LPC_GPIO1->FIOSET |= (1 << 28);
    }
}

inline void ClearInterruptFlagEINTX(uint32_t X)
{
    LPC_SC->EXTINT |= (1<<X);  // Clear Interrupt Flag
}


void InitPower()
{
    LPC_SC->PCONP |= (1 << 15);                     // Enable power
}

void InitLED()
{
    LPC_PINCON->PINSEL3 &= ~(0 << 25) | ~(0 << 24);   // LED connected to P1.28 is in GPIO mode (see Table 83)
    LPC_GPIO1->FIODIR   = (1<< 28);                   // Configure LED pins as OUTPUT - P1.28*/
    LPC_GPIO1->FIOCLR |= (1 << 28);                   // set P1.28 to LOW
}

void InitPushButton()
{

    LPC_PINCON->PINSEL4     =     (1<<20);      // Configure P2_10, as EINT0
    LPC_GPIO2->FIODIR       &= ~(0 << 10);      // P2.10 is an input pin
    LPC_GPIOINT->IO2IntEnF  |=  (1 << 10);      // P2.10 reads the falling edges to generate an interrupt

}



void InitRAMInterruptVectorTable()
{
    constexpr uint32_t VECTORTABLE_SIZE = 256;
    /**
    * @brief Aligment is in bytes!
    *  Global static vector table variable
    * 
    * static -> will stay the same after the first initialization.
    */
    alignas(VECTORTABLE_SIZE * sizeof(uint32_t)) static std::array<uint32_t, VECTORTABLE_SIZE> g_VectorTable;

    uint32_t *vectors = (uint32_t *)SCB->VTOR;

    // copy vector table to ram location
    std::memcpy(g_VectorTable.data(), vectors, sizeof(uint32_t) * VECTORTABLE_SIZE);


    /* relocate vector table into RAM*/ 
    // disable global interrupts
    __disable_irq();

    // change vectortable location
    SCB->VTOR = (uint32_t) g_VectorTable.data();
    
    // wait for memory operations to finish
    __DSB();

    // enable interrupts again.
    __enable_irq();
}


void PushButton_Handler()
{
    ClearInterruptFlagEINTX(0);
    ToggleLED(); 
}

void InitEINT0()
{
    LPC_SC->EXTINT      = (1<<0);	    // Clear Pending interrupts
    LPC_SC->EXTMODE     = (1<<0);       // Configure EINTx as Edge Triggered
    LPC_SC->EXTPOLAR    = (1<<0);       // Configure EINTx as Falling Edge
}

void BindEINT0Handler()
{
    // this can also be done by directly accessing the static array, if it was in the global scope
    // but at a position offset by NVIC_USER_IRQ_OFFSET (16 system interrupts with negative indices)
    NVIC_SetVector((IRQn)(18), (uint32_t)PushButton_Handler); 
}

void EnableEINT0()
{
    // EINT0 = 18
    NVIC_EnableIRQ((IRQn_Type)(18));    /* Enable the EINT0 interrupt */
}

void FireInterrupt(uint32_t index)
{
    if (index < 0 || index > 111)
        return;
    
    /**
     * Bits 8:0 - 256 Interrupt values
     * Values of bits 31:9 are reserved, so should not be read or touched
     * Manual UM10360 - Page 92
     */
    NVIC->STIR |= (0xFF & index);
}


int main()
{   
    SystemInit();                   // predefined startup stuff, clock etc.

    InitPower();                    // explicitly enable power
    InitPushButton();               // configure pushbutton to trigger the EINT0 interrupt
    InitLED();                      // configure LED's GPIO pin to be an output & set its value to LOW

    
    InitRAMInterruptVectorTable();  // move vector table to ram
    InitEINT0();                    // init eint0
    BindEINT0Handler();             // make EINT0 execute PushButtonHandler on interrupt
    EnableEINT0();                  // enable interrupt

    while (1)
    {
        FireInterrupt(18);          // 18 = EINT0
        delay_ms(500);
    }
        
}


#ifdef __cplusplus
}
#endif