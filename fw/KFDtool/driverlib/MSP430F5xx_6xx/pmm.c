/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//*****************************************************************************
//
// pmm.c - Driver for the pmm Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup pmm_api pmm
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_PMM__
#include "pmm.h"

#include <assert.h>

void PMM_enableSvsL (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_SVSMLCTL) |= SVSLE;
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_disableSvsL (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_SVSMLCTL) &= ~SVSLE;
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_enableSvmL (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_SVSMLCTL) |= SVMLE;
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_disableSvmL (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_SVSMLCTL) &= ~SVMLE;
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_enableSvsH (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_SVSMHCTL) |= SVSHE;
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_disableSvsH (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_SVSMHCTL) &= ~SVSHE;
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_enableSvmH (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_SVSMHCTL) |= SVMHE;
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_disableSvmH (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_SVSMHCTL) &= ~SVMHE;
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_enableSvsLSvmL (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_SVSMLCTL) |= (SVSLE + SVMLE);
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_disableSvsLSvmL (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_SVSMLCTL) &= ~(SVSLE + SVMLE);
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_enableSvsHSvmH (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_SVSMHCTL) |= (SVSHE + SVMHE);
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_disableSvsHSvmH (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_SVSMHCTL) &= ~(SVSHE + SVMHE);
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_enableSvsLReset (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_PMMRIE) |= SVSLPE;
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_disableSvsLReset (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_PMMRIE) &= ~SVSLPE;
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_enableSvmLInterrupt (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_PMMRIE) |= SVMLIE;
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_disableSvmLInterrupt (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_PMMRIE) &= ~SVMLIE;
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_enableSvsHReset (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_PMMRIE) |= SVSHPE;
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_disableSvsHReset (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_PMMRIE) &= ~SVSHPE;
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_enableSvmHInterrupt (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_PMMRIE) |= SVMHIE;
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_disableSvmHInterrupt (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_PMMRIE) &= ~SVMHIE;
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_clearPMMIFGS (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_PMMIFG) = 0;
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_enableSvsLInLPMFastWake (void)
{
    //These settings use SVSH/LACE = 0
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_SVSMLCTL) |= (SVSLFP + SVSLMD);
    HWREG16(PMM_BASE + OFS_SVSMLCTL) &= ~SVSMLACE;
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_enableSvsLInLPMSlowWake (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_SVSMLCTL) |= SVSLMD;
    HWREG16(PMM_BASE + OFS_SVSMLCTL) &= ~(SVSLFP + SVSMLACE);
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_disableSvsLInLPMFastWake (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_SVSMLCTL) |= SVSLFP;
    HWREG16(PMM_BASE + OFS_SVSMLCTL) &= ~(SVSLMD + SVSMLACE);
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_disableSvsLInLPMSlowWake (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_SVSMLCTL) &= ~(SVSLFP + SVSMLACE + SVSLMD);
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_enableSvsHInLPMNormPerf (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_SVSMHCTL) |= SVSHMD;
    HWREG16(PMM_BASE + OFS_SVSMHCTL) &= ~(SVSMHACE + SVSHFP);
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_enableSvsHInLPMFullPerf (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_SVSMHCTL) |= (SVSHMD + SVSHFP);
    HWREG16(PMM_BASE + OFS_SVSMHCTL) &= ~SVSMHACE;
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_disableSvsHInLPMNormPerf (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_SVSMHCTL) &= ~(SVSMHACE + SVSHFP + SVSHMD);
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_disableSvsHInLPMFullPerf (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_SVSMHCTL) |= SVSHFP;
    HWREG16(PMM_BASE + OFS_SVSMHCTL) &= ~(SVSMHACE + SVSHMD);
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_optimizeSvsLInLPMFastWake (void)
{
    //These setting use SVSH/LACE = 1
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_SVSMLCTL) |= (SVSLFP + SVSLMD + SVSMLACE);
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

void PMM_optimizeSvsHInLPMFullPerf (void)
{
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;
    HWREG16(PMM_BASE + OFS_SVSMHCTL) |= (SVSHMD + SVSHFP + SVSMHACE);
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
}

uint16_t PMM_setVCoreUp ( uint8_t level){
    uint32_t PMMRIE_backup, SVSMHCTL_backup, SVSMLCTL_backup;

    //The code flow for increasing the Vcore has been altered to work around
    //the erratum FLASH37.
    //Please refer to the Errata sheet to know if a specific device is affected
    //DO NOT ALTER THIS FUNCTION

    //Open PMM registers for write access
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;

    //Disable dedicated Interrupts
    //Backup all registers
    PMMRIE_backup = HWREG16(PMM_BASE + OFS_PMMRIE);
    HWREG16(PMM_BASE + OFS_PMMRIE) &= ~(SVMHVLRPE | SVSHPE | SVMLVLRPE |
                                          SVSLPE | SVMHVLRIE | SVMHIE |
                                          SVSMHDLYIE | SVMLVLRIE | SVMLIE |
                                          SVSMLDLYIE
                                          );
    SVSMHCTL_backup = HWREG16(PMM_BASE + OFS_SVSMHCTL);
    SVSMLCTL_backup = HWREG16(PMM_BASE + OFS_SVSMLCTL);

    //Clear flags
    HWREG16(PMM_BASE + OFS_PMMIFG) = 0;

    //Set SVM highside to new level and check if a VCore increase is possible
    HWREG16(PMM_BASE + OFS_SVSMHCTL) = SVMHE | SVSHE | (SVSMHRRL0 * level);

    //Wait until SVM highside is settled
    while ((HWREG16(PMM_BASE + OFS_PMMIFG) & SVSMHDLYIFG) == 0) ;

    //Clear flag
    HWREG16(PMM_BASE + OFS_PMMIFG) &= ~SVSMHDLYIFG;

    //Check if a VCore increase is possible
    if ((HWREG16(PMM_BASE + OFS_PMMIFG) & SVMHIFG) == SVMHIFG){
        //-> Vcc is too low for a Vcore increase
        //recover the previous settings
        HWREG16(PMM_BASE + OFS_PMMIFG) &= ~SVSMHDLYIFG;
        HWREG16(PMM_BASE + OFS_SVSMHCTL) = SVSMHCTL_backup;

        //Wait until SVM highside is settled
        while ((HWREG16(PMM_BASE + OFS_PMMIFG) & SVSMHDLYIFG) == 0) ;

        //Clear all Flags
        HWREG16(PMM_BASE +
            OFS_PMMIFG) &= ~(SVMHVLRIFG | SVMHIFG | SVSMHDLYIFG |
                             SVMLVLRIFG | SVMLIFG |
                             SVSMLDLYIFG
                             );

        //Restore PMM interrupt enable register
        HWREG16(PMM_BASE + OFS_PMMRIE) = PMMRIE_backup;
        //Lock PMM registers for write access
        HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
        //return: voltage not set
        return ( STATUS_FAIL) ;
    }

    //Set also SVS highside to new level
    //Vcc is high enough for a Vcore increase
    HWREG16(PMM_BASE + OFS_SVSMHCTL) |= (SVSHRVL0 * level);

    //Wait until SVM highside is settled
    while ((HWREG16(PMM_BASE + OFS_PMMIFG) & SVSMHDLYIFG) == 0) ;

    //Clear flag
    HWREG16(PMM_BASE + OFS_PMMIFG) &= ~SVSMHDLYIFG;

    //Set VCore to new level
    HWREG8(PMM_BASE + OFS_PMMCTL0_L) = PMMCOREV0 * level;

    //Set SVM, SVS low side to new level
    HWREG16(PMM_BASE + OFS_SVSMLCTL) = SVMLE | (SVSMLRRL0 * level) |
                                         SVSLE | (SVSLRVL0 * level);

    //Wait until SVM, SVS low side is settled
    while ((HWREG16(PMM_BASE + OFS_PMMIFG) & SVSMLDLYIFG) == 0) ;

    //Clear flag
    HWREG16(PMM_BASE + OFS_PMMIFG) &= ~SVSMLDLYIFG;
    //SVS, SVM core and high side are now set to protect for the new core level

    //Restore Low side settings
    //Clear all other bits _except_ level settings
    HWREG16(PMM_BASE + OFS_SVSMLCTL) &= (SVSLRVL0 + SVSLRVL1 + SVSMLRRL0 +
                                           SVSMLRRL1 + SVSMLRRL2
                                           );

    //Clear level settings in the backup register,keep all other bits
    SVSMLCTL_backup &=
        ~(SVSLRVL0 + SVSLRVL1 + SVSMLRRL0 + SVSMLRRL1 + SVSMLRRL2);

    //Restore low-side SVS monitor settings
    HWREG16(PMM_BASE + OFS_SVSMLCTL) |= SVSMLCTL_backup;

    //Restore High side settings
    //Clear all other bits except level settings
    HWREG16(PMM_BASE + OFS_SVSMHCTL) &= (SVSHRVL0 + SVSHRVL1 +
                                           SVSMHRRL0 + SVSMHRRL1 +
                                           SVSMHRRL2
                                           );

    //Clear level settings in the backup register,keep all other bits
    SVSMHCTL_backup &=
        ~(SVSHRVL0 + SVSHRVL1 + SVSMHRRL0 + SVSMHRRL1 + SVSMHRRL2);

    //Restore backup
    HWREG16(PMM_BASE + OFS_SVSMHCTL) |= SVSMHCTL_backup;

    //Wait until high side, low side settled
    while (((HWREG16(PMM_BASE + OFS_PMMIFG) & SVSMLDLYIFG) == 0) ||
           ((HWREG16(PMM_BASE + OFS_PMMIFG) & SVSMHDLYIFG) == 0)) ;

    //Clear all Flags
    HWREG16(PMM_BASE + OFS_PMMIFG) &= ~(SVMHVLRIFG | SVMHIFG | SVSMHDLYIFG |
                                          SVMLVLRIFG | SVMLIFG | SVSMLDLYIFG
                                          );

    //Restore PMM interrupt enable register
    HWREG16(PMM_BASE + OFS_PMMRIE) = PMMRIE_backup;

    //Lock PMM registers for write access
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;

    return ( STATUS_SUCCESS) ;
}

uint16_t PMM_setVCoreDown ( uint8_t level){
    uint32_t PMMRIE_backup, SVSMHCTL_backup, SVSMLCTL_backup;

    //The code flow for decreasing the Vcore has been altered to work around
    //the erratum FLASH37.
    //Please refer to the Errata sheet to know if a specific device is affected
    //DO NOT ALTER THIS FUNCTION

    //Open PMM registers for write access
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0xA5;

    //Disable dedicated Interrupts
    //Backup all registers
    PMMRIE_backup = HWREG16(PMM_BASE + OFS_PMMRIE);
    HWREG16(PMM_BASE + OFS_PMMRIE) &= ~(SVMHVLRPE | SVSHPE | SVMLVLRPE |
                                          SVSLPE | SVMHVLRIE | SVMHIE |
                                          SVSMHDLYIE | SVMLVLRIE | SVMLIE |
                                          SVSMLDLYIE
                                          );
    SVSMHCTL_backup = HWREG16(PMM_BASE + OFS_SVSMHCTL);
    SVSMLCTL_backup = HWREG16(PMM_BASE + OFS_SVSMLCTL);

    //Clear flags
    HWREG16(PMM_BASE + OFS_PMMIFG) &= ~(SVMHIFG | SVSMHDLYIFG |
                                          SVMLIFG | SVSMLDLYIFG
                                          );

    //Set SVM, SVS high & low side to new settings in normal mode
    HWREG16(PMM_BASE + OFS_SVSMHCTL) = SVMHE | (SVSMHRRL0 * level) |
                                         SVSHE | (SVSHRVL0 * level);
    HWREG16(PMM_BASE + OFS_SVSMLCTL) = SVMLE | (SVSMLRRL0 * level) |
                                         SVSLE | (SVSLRVL0 * level);

    //Wait until SVM high side and SVM low side is settled
    while ((HWREG16(PMM_BASE + OFS_PMMIFG) & SVSMHDLYIFG) == 0 ||
           (HWREG16(PMM_BASE + OFS_PMMIFG) & SVSMLDLYIFG) == 0) ;

    //Clear flags
    HWREG16(PMM_BASE + OFS_PMMIFG) &= ~(SVSMHDLYIFG + SVSMLDLYIFG);
    //SVS, SVM core and high side are now set to protect for the new core level

    //Set VCore to new level
    HWREG8(PMM_BASE + OFS_PMMCTL0_L) = PMMCOREV0 * level;

    //Restore Low side settings
    //Clear all other bits _except_ level settings
    HWREG16(PMM_BASE + OFS_SVSMLCTL) &= (SVSLRVL0 + SVSLRVL1 + SVSMLRRL0 +
                                           SVSMLRRL1 + SVSMLRRL2
                                           );

    //Clear level settings in the backup register,keep all other bits
    SVSMLCTL_backup &=
        ~(SVSLRVL0 + SVSLRVL1 + SVSMLRRL0 + SVSMLRRL1 + SVSMLRRL2);

    //Restore low-side SVS monitor settings
    HWREG16(PMM_BASE + OFS_SVSMLCTL) |= SVSMLCTL_backup;

    //Restore High side settings
    //Clear all other bits except level settings
    HWREG16(PMM_BASE + OFS_SVSMHCTL) &= (SVSHRVL0 + SVSHRVL1 + SVSMHRRL0 +
                                           SVSMHRRL1 + SVSMHRRL2
                                           );

    //Clear level settings in the backup register, keep all other bits
    SVSMHCTL_backup &=
        ~(SVSHRVL0 + SVSHRVL1 + SVSMHRRL0 + SVSMHRRL1 + SVSMHRRL2);

    //Restore backup
    HWREG16(PMM_BASE + OFS_SVSMHCTL) |= SVSMHCTL_backup;

    //Wait until high side, low side settled
    while (((HWREG16(PMM_BASE + OFS_PMMIFG) & SVSMLDLYIFG) == 0) ||
           ((HWREG16(PMM_BASE + OFS_PMMIFG) & SVSMHDLYIFG) == 0)) ;

    //Clear all Flags
    HWREG16(PMM_BASE + OFS_PMMIFG) &= ~(SVMHVLRIFG | SVMHIFG | SVSMHDLYIFG |
                                          SVMLVLRIFG | SVMLIFG | SVSMLDLYIFG
                                          );

    //Restore PMM interrupt enable register
    HWREG16(PMM_BASE + OFS_PMMRIE) = PMMRIE_backup;
    //Lock PMM registers for write access
    HWREG8(PMM_BASE + OFS_PMMCTL0_H) = 0x00;
    //Return: OK
    return ( STATUS_SUCCESS) ;
}

bool PMM_setVCore ( uint8_t level){
    uint8_t actlevel;
    bool status = STATUS_SUCCESS;
    uint16_t interruptState;

    //Set Mask for Max. level
    level &= PMMCOREV_3;

    //Get actual VCore
    actlevel = (HWREG16(PMM_BASE + OFS_PMMCTL0) & PMMCOREV_3);

    //Disable interrupts because certain peripherals will not
    //work during VCORE change
    interruptState = __get_interrupt_state();
    __disable_interrupt();
    __no_operation();

    //step by step increase or decrease
    while ((level != actlevel) && (status == STATUS_SUCCESS))
    {
        if (level > actlevel){
            status = PMM_setVCoreUp(++actlevel);
        } else   {
            status = PMM_setVCoreDown(--actlevel);
        }
    }

    //Re-enable interrupt state to whatever it was before
    if(interruptState & GIE)
    {
        __enable_interrupt();
    }

    return ( status) ;
}

uint16_t PMM_getInterruptStatus (uint16_t mask)
{
    return ( (HWREG16(PMM_BASE + OFS_PMMIFG)) & mask );
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for pmm_api
//! @}
//
//*****************************************************************************
