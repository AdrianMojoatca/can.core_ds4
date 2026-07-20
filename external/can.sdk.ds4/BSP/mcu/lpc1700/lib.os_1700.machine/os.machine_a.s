; /****************************************************************************/
; /*                                                                          */
; /*                         A-Task Real-Time Kernel                          */
; /*                                                                          */
; /*                  Copyright (C) 2000-2009 - Acacetus Inc                  */
; /*                                                                          */
; /*                           All rights reserved                            */
; /*                                                                          */
; /*                             www.acacetus.com                             */
; /*                                                                          */
; /*--------------------------------------------------------------------------*/
; /*                                                                          */
; /*           S O F T W A R E   L I C E N S E   A G R E E M E N T            */
; /*                                                                          */
; /* This software is the property of Acacetus Inc. and its use is governed   */
; /* by the software license agreement stipulated in the following file:      */
; /*                                                                          */
; /*                          "Software License.pdf"                          */
; /*                                                                          */
; /****************************************************************************/
; /*                                                                          */
; /*                                NOTES                                     */
; /*                                -----                                     */
; /*                                                                          */
; /*                       Task Stack Frame Layout                            */
; /*                                                                          */
; /*                              +-------+                                   */
; /*                       HI-MEM |  xPSR | <<-- Task xPSR status             */
; /*                              +-------+                                   */
; /*                              |  R15  | <<-- Task PC                      */
; /*                              +-------|                                   */
; /*                              |  R14  | <<-- Task LR                      */
; /*                              +-------+                                   */
; /*                              |  R12  |                                   */
; /*                              +-------+                                   */
; /*                              |  R3   |                                   */
; /*                              +-------+                                   */
; /*                              |  R2   |                                   */
; /*                              +-------+                                   */
; /*                              |  R1   |                                   */
; /*                              +-------+                                   */
; /*                              |  R0   | <<-- Task Argument 'tsk_arg'      */
; /*                              +-------+                                   */
; /*                              |BASEPRI| <<-- Stack Frame points here      */
; /*                              +-------+                                   */
; /*                              |  R11  |                                   */
; /*                              +-------+                                   */
; /*                              |  R10  |                                   */
; /*                              +-------+                                   */
; /*                              |  R9   |                                   */
; /*                              +-------+                                   */
; /*                              |  R8   |                                   */
; /*                              +-------+                                   */
; /*                              |  R7   |                                   */
; /*                              +-------+                                   */
; /*                              |  R6   |                                   */
; /*                              +-------+                                   */
; /*                              |  R5   |                                   */
; /*                              +-------+                                   */
; /*                       LO-MEM |  R4   | <<-- Stack Frame points here      */
; /*                              +-------+                                   */
; /*                                                                          */
; /*  Notes:                                                                  */
; /*                                                                          */
; /*  - All IRQs that interact with the OS must be at level 31                */
; /*                                                                          */
; /*  - SVC instruction is used by kernel to enter 'handler' mode and         */
; /*    must always be assigned a priority of level 30                        */
; /*                                                                          */
; /*  - PRIBASE must never be any other value except level 0 or 31.           */
; /*                                                                          */
; /*  - Interrupts that do not interact with the OS use the MSP stack.        */
; /*                                                                          */
; /*  - The OS stack must be large enough to support nested interrupts        */
; /*    for IRQ priorities higher than level 31.                              */
; /*                                                                          */
; /*  - Tasks use the PSP stack.                                              */
; /*                                                                          */
; /*  - On Cortex-M3, R13 is either MSP or USP, so not part of context.       */
; /*                                                                          */
; /*  - IRQs that interact with the OS *must* be level 31.                    */
; /*                                                                          */
; /*  - IRQs that don't can be any level, but only level 29 and higher will   */
; /*    preempt the kernel (level 30 can preempt kernel only of kernel        */
; /*    was not entered by SVC call.                                          */
; /*                                                                          */
; /*  - Use IRQ levels 0 to 29 for implementing virtual peripherals and       */
; /*    signal OS with PENDSV.                                                */
; /*                                                                          */
; /*  - When signaling OS with PENDSV, its priority level *must* be set       */
; /*    to level 31.                                                          */
; /*                                                                          */
; /****************************************************************************/

;------------------------------------------------------------------------------;
; $Id$
;------------------------------------------------------------------------------;

                import nvic_set_vector


                import os_sys_stk
                import os_idle_stk
                import os_stk_frame
                import os_irq_enter_c
                import os_irq_leave_c
                import os_sys_idle_func
                import os_kern_process_cmd
                import os_kern_tsk_schedule

;------------------------------------------------------------------------------;

SCB_SHPR1       equ 0xE000ED18
SCB_SHPR2       equ 0xE000ED1C
SCB_SHPR3       equ 0xE000ED20

;------------------------------------------------------------------------------;

                area    |.bss| , data , readwrite
                align

;------------------------------------------------------------------------------;

                area    |.text| , code , readonly , align=3

;------------------------------------------------------------------------------;

; void os_kern_init_hw_1 (void)

                align

                preserve8

                export  os_kern_init_hw_1

os_kern_init_hw_1 proc 

                push    {r0-r1,lr}              ; Save regs

                mov     r0,#-5                  ; Index to SVC vector
			;	mov 	r1,#30					; IRQ PRIORITY 
			;	mov		r2,#1					; IRQ TYPE fast
                ldr     r1,=os_svc_handler      ; Get SVC handler address
                bl      nvic_set_vector         ; Set SVC handler vector

                pop     {r0-r1,pc}              ; Restore regs and return

                endp

;------------------------------------------------------------------------------;

; void os_kern_init_hw_2 (void)

                align

                preserve8

                export  os_kern_init_hw_2

os_kern_init_hw_2 proc 

                push    {r0-r3,r12,lr}          ; Save regs

                ;;; Point PSP to IDLE stack ;;;

                ldr     r0,=os_idle_stk         ; Fetch address of IDLE stack
                ldr     r0,[r0]                 ; 
                msr     PSP,r0                  ; Setup PSP stack pointer

                ;;; Enable DUAL stack operation ;;;

                isb                             ; Instruction sequence barrier

                mrs     r0,CONTROL              ; Enable dual stack mode
                orr     r0,#0x02                ;
                msr     CONTROL,r0              ; PSP is now active

                isb                             ; Instruction sequence barrier

                ;;; Call SVC to initialize SVC & PENDSV priority levels and return in 'handler' mode ;;;

                svc     #0x00                   ; Cmd = 0x00 : OS Initialize

                ;;; Restore regs, and return to caller in 'handler' mode ;;;

                pop     {r0-r3,r12,pc}          ; Restore regs and return

                endp

;------------------------------------------------------------------------------;

; void os_kern_tsk_execute_hw (void)

                align

                preserve8

                export os_kern_tsk_execute_hw

os_kern_tsk_execute_hw proc

                ldr     r0,=os_sys_stk          ; Fetch OS stack
                ldr     sp,[r0]                 ; And setup MSP

                ldr     r0,=os_stk_frame        ; Fetch Task stack frame
                ldr     r0,[r0]                 ;

                ldmia   r0!,{r4-r12}            ; Restore R4-R11, R12=BASEPRI

                msr     PSP,r0                  ; Setup PSP for return 

                msr     BASEPRI,r12             ; Restore tasks BASEPRI (level 31 interrupts can now possibly happen)

                mov     lr,#0xFFFFFFFD          ; Return from IRQ (Thread,PSP)
                bx      lr                      ; Restore exception context from PSP
                
                endp

;------------------------------------------------------------------------------;

; void os_kern_process_cmd_hw (K_Func k_func , void * arg1,va_list args)

                align

                preserve8

                export  os_kern_process_cmd_hw

os_kern_process_cmd_hw proc

                svc     #0x01                   ; Enter kernel to process command

                bx      lr                      ; Return to caller

                endp
        
;------------------------------------------------------------------------------;

; void os_kern_tsk_idle (void)

                align

                preserve8

                export os_kern_tsk_idle

os_kern_tsk_idle proc

                ;;; Setup IDLE context for execution in 'thread' state ;;;

                mov     r7,#0x01000000          ; xPSR onto PSP stack 

                ldr     r6,=os_sys_idle_func    ; PC address is user-defined idle function
                ldr     r6,[r6]                 ;
                orr     r6,#0x01                ; Ensure odd address

                ldr     r5,=idle_return         ; LR address (thread state)

                ;;; Put the context onto the PSP stack ;;;

                ldr     r8,=os_idle_stk         ; Fetch OS idle stack
                ldr     r8,[r8]                 ;

                stmdb   r8!,{r0-r7}             ; Setup exception context (r0-r4 = 'don't care)

                msr     PSP,r8                  ; Setup IDLE thread stack pointer

                ;;; Maximize OS stack and call idle function in 'thread' mode ;;;

                ldr     r0,=os_sys_stk          ; Fetch OS system stack
                ldr     sp,[r0]                 ;
                
                mov     lr,#0xFFFFFFFD          ; Return from IRQ (Thread,PSP)
                bx      lr                      ; Execute IDLE handler in 'thread' state
idle_return
                ;;; Ensure BASEPRI is at level-0 and wait for interrupt ;;;

                mov     r0,#0x00                ; Enable all interrupts
                msr     BASEPRI,r0              ;

                b       .                       ; Wait with interrupts enabled

                endp

;------------------------------------------------------------------------------;

; void os_irq_enter (void)

                align

                preserve8

                export os_irq_enter

os_irq_enter    proc

                ;;; Save BASEPRI then set it to level-31 ;;;

                mrs     r12,BASEPRI             ; Fetch current BASEPRI to save

                mov     r0,#0xF8                ; Level-31
                msr     BASEPRI,r0              ; Disable level 15 IRQ

                ;;; Save remaining context on stack and save frame pointer for OS ;;;

                mrs     r1,PSP                  ; Fetch PSP address (tasks stack)
                stmdb   r1!,{r4-r12}            ; Save R4-R11 , R12 = BASEPRI

                ldr     r0,=os_stk_frame        ; Get address of stk frame var
                str     r1,[r0]                 ; Save tasks stack frame

                b       os_irq_enter_c          ; Call 'C' part of IRQ enter (calle will return to our caller)

                endp

;------------------------------------------------------------------------------;

; void os_irq_leave (void)

                align

                preserve8

                export  os_irq_leave

os_irq_leave    proc

                ldr     r0,=os_irq_leave_c      ; Call 'C' part of IRQ leave
                        blx     r0                      ;

                b       os_kern_tsk_schedule    ; Go schedule next available task (does not return)

                endp

;------------------------------------------------------------------------------;

                align

                preserve8

                export  os_svc_handler

os_svc_handler  proc

                ;;; Determine command and execute ;;;

                mrs     r3,PSP                  ; Fetch callers SP

                ldr     r2,[r3,#4*6]            ; Fetch callers return address
                ldrb    r12,[r2,#-2]            ; Fetch SVC command byte from SVC instruction

                tbb.w   [pc,r12]                ; Perform table branch based on command byte
cmd_table
                dcb (cmd_00 - cmd_table)/2      ; 00 = OS Initialization
                dcb (cmd_01 - cmd_table)/2      ; 01 = Task Service Request

                ;; R3  = PSP
                ;; R2  = Return address
                ;; R12 = Command byte

                align

;-[ SVC Cmd = 0x00 ]-----------------------------------------------------------;

                ;;; Cmd = 0x00 : Enter 'Handler' mode to perform OS initialization ;;; 

                align
cmd_00
                cpsid   i                       ; Disable global IRQ

                ;;; Set BASEPRI to level 31 IRQ priority ;;;

                mov     r0,#0xF8                ; Level  31
                msr     BASEPRI,r0              ; Update BASEPRI

                ;;; Configure SYS Call to execute at IRQ level 30 ;;;

                mov     r0,#0xF0000000          ; Get IRQ Level 30 in 31:27
                ldr     r1,=SCB_SHPR2           ;
                ldr     r3,[r1]                 ;
                orr     r3,r0                   ;
                str     r3,[r1]                 ; SYS Call is now level 30

                cpsie   i                       ; Enable global IRQ

                ;;; Return to caller in 'handler' state ;;;

                orr     r2,#01                  ; Force return address to odd
                bx      r2                      ; return

;-[ SVC Cmd = 0x01 ]-----------------------------------------------------------;

                ;;; Cmd = 0x01 : Task request for service ;;; 

                align
cmd_01
                mrs     r12,BASEPRI             ; Get tasks BASEPRI value

                mov     r2,#0xF8                ; Disable level-31 IRQ
                msr     BASEPRI,r2              ;

                stmdb   r3!,{r4-r12}            ; Save remaining context R4-R11 , R12 = BASEPRI

                ldr     r2,=os_stk_frame        ; Save tasks stack frame
                str     r3,[r2]                 ;

                add     lr,r3,#4*9             	; Fetch address of arguments from PSP stack
                ldm     lr,{r0,r1,r2,r3,r12}    ; Setup arguments for OS call
				add     lr,#4*(11-9)

                b       os_kern_process_cmd     ; Call OS to do operation

                endp

;------------------------------------------------------------------------------;

                align

                end

