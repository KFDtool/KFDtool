// KFDtool
// Copyright 2019 Daniel Dugger

#include "msp430.h"
#include "driverlib.h"

#include "hal.h"

void halInit(void)
{
    // use XT2 for MCLK and SMCLK
    PMM_setVCore(PMM_CORE_LEVEL_3);

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN2);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN3);

    UCS_turnOnXT2(UCS_XT2_DRIVE_24MHZ_32MHZ);

    UCS_initClockSignal(UCS_MCLK, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_SMCLK, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_1);

    // P1.0 LED1
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

    // P1.2 MCU_DATA_OUT_3V3
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN2);

    // P1.3 MCU_DATA_IN_3V3
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN3, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN3);

    // P2.4 MCU_SENSE_OUT_3V3
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);

    // P2.5 MCU_SENSE_IN_3V3
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN5);

    // P3.1 GPIO1
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN1);

    // P3.7 GPIO2
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN7);

    // P4.0 EEPROM_CS
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN0); // cs idle high
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN0);

    // P4.1 EEPROM_MOSI
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN1); // mosi idle high
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN1);

    // P4.2 EEPROM_MISO
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN2); // miso idle high

    // P4.3 EEPROM_SCK
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN3); // spi mode 0, sck idle low
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN3);

    // set all unused pins output low
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);

    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN6|GPIO_PIN7);

    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN0|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6);

    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);

    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0|GPIO_PIN1|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN0|GPIO_PIN1|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);

    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);

    GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);

    GPIO_setOutputLowOnPin(GPIO_PORT_P8, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P8, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);

    GPIO_setOutputLowOnPin(GPIO_PORT_PJ, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_PJ, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
}

void halDelayUs(uint16_t us)
{
    while (us)
    {
        __delay_cycles(FCPU/1000000);
        us--;
    }
}

void halDelayMs(uint16_t ms)
{
    while (ms)
    {
        __delay_cycles(FCPU/1000);
        ms--;
    }
}

void halEnterBsl(void)
{
    ((void (*)())0x1000)(); // Call BSL
}

void halReset(void)
{
    //PMMCTL0 = PMMPW | PMMSWPOR; // trigger software POR

    PMMCTL0 = PMMPW | PMMSWBOR; // trigger software BOR
}

void halLed1On(void)
{
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
}

void halLed1Off(void)
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
}

void halLed1Toggle(void)
{
    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
}

void halGpio1High(void)
{
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN1);
}

void halGpio1Low(void)
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN1);
}

void halGpio1Toggle(void)
{
    GPIO_toggleOutputOnPin(GPIO_PORT_P3, GPIO_PIN1);
}

void halGpio2High(void)
{
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN7);
}

void halGpio2Low(void)
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN7);
}

void halGpio2Toggle(void)
{
    GPIO_toggleOutputOnPin(GPIO_PORT_P3, GPIO_PIN7);
}

void halKfdTxBusy(void)
{
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN2);
}

void halKfdTxIdle(void)
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN2);
}

void halSenTxConn(void)
{
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);
}

void halSenTxDisc(void)
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);
}
