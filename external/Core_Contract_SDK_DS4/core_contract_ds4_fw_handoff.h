#ifndef __CORE_CONTRACT_DS4_FW_HANDOFF_H__
#define __CORE_CONTRACT_DS4_FW_HANDOFF_H__

#include <stdint.h>

#include "device.h"
#include "core_contract_ds4_image_layout.h"

typedef void (*core_ds4_fw_entry_fn_t)(void);

#if defined(__CC_ARM)
static __asm void core_contract_ds4_fw_handoff_branch_no_return(uint32_t entry_addr)
{
    ; r0 = entry_addr (FW entry | 1). Tear down the live CORE RTOS state so FW can
    ; re-boot from a reset-like context: the FW __main scatter-loads over the shared
    ; RAM (incl. the vector table @0x10002000), so any live IRQ/SysTick vectoring
    ; through the old table mid-scatter would HardFault. Mask + disable everything,
    ; switch to a private MSP outside the scatter region, then restore PRIMASK=0
    ; (FW svc/BASEPRI code needs reset masking) and branch.

    CPSID   i                       ; mask IRQs during teardown

    MOVS    r1, #0
    LDR     r2, =0xE000E010         ; SysTick->CTRL
    STR     r1, [r2, #0x0]          ; CTRL = 0 (stop counter + tick IRQ)
    STR     r1, [r2, #0x8]          ; VAL  = 0

    MVNS    r1, #0                  ; 0xFFFFFFFF
    LDR     r2, =0xE000E180         ; NVIC->ICER[0] (disable all IRQs)
    STR     r1, [r2, #0x0]
    STR     r1, [r2, #0x4]          ; ICER[1]
    LDR     r2, =0xE000E280         ; NVIC->ICPR[0] (clear all pending)
    STR     r1, [r2, #0x0]
    STR     r1, [r2, #0x4]          ; ICPR[1]

    LDR     r2, =0xE000ED04         ; SCB->ICSR
    LDR     r1, =0x0A000000         ; PENDSVCLR(27) | PENDSTCLR(25)
    STR     r1, [r2, #0x0]

    MOVS    r1, #0
    MSR     BASEPRI, r1             ; clear any CORE critical-section masking

    MRS     r1, CONTROL
    BIC     r1, r1, #0x2            ; SPSEL = 0 -> use MSP
    MSR     CONTROL, r1
    ISB

    LDR     r1, =0x10007F00         ; safe MSP: above FW scatter (<=0x10004EFF),
    MSR     MSP, r1                 ; below IAP reserve (0x10007F00)

    DSB
    ISB

    CPSIE   i                       ; PRIMASK=0 (reset-like); no source enabled now

    BX      r0                      ; -> FW entry (B.W __main)
}
#elif defined(__GNUC__)
static inline void core_contract_ds4_fw_handoff_branch_no_return(uint32_t entry_addr)
{
    __asm volatile (
        "cpsie i\n"
        "bx %0\n"
        :
        : "r" (entry_addr)
        : "memory");
}
#else
static void core_contract_ds4_fw_handoff_branch_no_return(uint32_t entry_addr)
{
    core_ds4_fw_entry_fn_t fw_entry = (core_ds4_fw_entry_fn_t)entry_addr;
    fw_entry();
}
#endif

static uint32_t core_contract_ds4_fw_handoff_is_valid(void)
{
    const uint32_t* fw_image_header = (const uint32_t*)(CORE_DS4_FLASH_FW_START_ADDR);
    uint32_t first_word = fw_image_header[0];
    uint32_t second_word = fw_image_header[1];

    /*
     * For this image format APP_VECTOR starts with code words (not MSP/Reset vectors),
     * therefore validation checks for non-empty and executable Thumb entry at address C.
     */
    uint32_t fw_entry_addr = (CORE_DS4_FLASH_FW_START_ADDR | 0x1UL);

    if ((first_word == 0U) || (first_word == 0xFFFFFFFFUL))
    {
        return 0U;
    }

    if ((second_word == 0U) || (second_word == 0xFFFFFFFFUL))
    {
        return 0U;
    }

    if ((fw_entry_addr < CORE_DS4_FLASH_FW_START_ADDR) || (fw_entry_addr > CORE_DS4_FLASH_FW_END_ADDR))
    {
        return 0U;
    }

    if ((fw_entry_addr & 0x1UL) == 0U)
    {
        return 0U;
    }

    return 1U;
}

static void core_contract_ds4_fw_handoff_jump(void)
{
    uint32_t fw_entry_addr = (CORE_DS4_FLASH_FW_START_ADDR | 0x1UL);

    core_contract_ds4_fw_handoff_branch_no_return(fw_entry_addr);

    while (1)
    {
    }
}

#endif /* __CORE_CONTRACT_DS4_FW_HANDOFF_H__ */
