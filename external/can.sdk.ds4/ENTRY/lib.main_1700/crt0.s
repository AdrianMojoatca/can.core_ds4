;/*****************************************************************************/
;/* <<< Use Configuration Wizard in Context Menu >>>                          */
;/*****************************************************************************/

;------------------------------------------------------------------------------;
; $Id$
;------------------------------------------------------------------------------;

;// <h> Initial Run-Time Stack & OS Heap Configuration
;//   <o> Heap size (in Bytes) <0x0200-0xFFFFFFFF:8>
;//   <i> The initial run-time stack pointer is assigned to
;//   <i> the top of the OS heap space.  This allows the startup
;//   <i> code to execute with a valid stack in order to initialize
;//   <i> the OS.  Once initialized, the OS will allocate all
;//   <i> stacks dynamically from the heap, thus re-using the
;//   <i> initial run-time stack space.
;//   <i> So long as the heap is large enough to support the initial
;//   <i> run-time stack requirements, everything works fine.
;// </h>


UND_Stack_Size  EQU     0x00000008
ABT_Stack_Size  EQU     0x00000010
FIQ_Stack_Size  EQU     0x00000160
IRQ_Stack_Size  EQU     0x00000160

                EXPORT UND_Stack_Size
                EXPORT ABT_Stack_Size
                EXPORT FIQ_Stack_Size
                EXPORT IRQ_Stack_Size

ISR_Stack_Size  EQU     (UND_Stack_Size + ABT_Stack_Size + FIQ_Stack_Size + IRQ_Stack_Size)

                AREA    STACK, NOINIT, READWRITE, ALIGN=3

Stack_Mem       SPACE   256

__initial_sp    ; Used during 'main()' and 'os_execute()' initialization.

                SPACE   ISR_Stack_Size
Stack_Top

                EXPORT Stack_Mem
                EXPORT Stack_Top
                EXPORT __initial_sp


;// <h> Heap Configuration
;//   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
;// </h>

;Heap_Size       EQU     0x00000000
 
;                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
;__heap_base
;Heap_Mem        SPACE   Heap_Size
;__heap_limit
 


;                EXPORT __heap_base
;                EXPORT __heap_limit


;------------------------------------------------------------------------------;

NVIC_CCR        equ 0xE000ED14
SCB_VTOR        equ 0xE000ED08
SCB_SHCSR       equ 0xE000ED24

;------------------------------------------------------------------------------;

                preserve8

                thumb

; Vector Table Mapped to Address 0 at Reset

				
                AREA    APP_VECTOR, CODE, READONLY

				export	ISR_Handler_0
				import	__main
				
U32SIZE EQU 4				

                align 4
				
My_crt0			proc
				B.W		__main
				
				endp
				
IRQ_Handler		proc		
				
				import isr_user_func
				
				ltorg
				
                align 4
				
				LDR   r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 14*U32SIZE

				LDR   r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 15*U32SIZE
				
ISR_Handler_0
				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 16*U32SIZE
				
				
				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 17*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 18*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 19*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 20*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 21*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 22*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 23*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 24*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 25*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 26*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 27*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 28*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 29*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 30*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 31*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 32*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 33*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 34*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 35*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 36*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 37*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 38*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 39*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 40*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 41*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 42*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 43*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 44*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 45*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 46*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				DCD		isr_user_func + 47*U32SIZE

				ldr		r1,[PC,#0]
				b		com_isr_handling
				
				ltorg
				
				
				DCD		isr_user_func + 48*U32SIZE

com_isr_handling 		
				import	isr_user_func_vector
				import	os_irq_enter
				import	os_irq_leave

				ldr		r0,[r1]       ;read user vector


				ldr		r1,=isr_user_func_vector
				str		r0,[r1]		; save into temp vector variable
			
				ldmfd	r13!,{r0, r1, lr}		;restore registers

				stmfd   r13!,{lr}
				
                bl 		os_irq_enter

				ldr		r1,=isr_user_func_vector
				bl      irq_dispatch
				
				b 		os_irq_leave
		
				ENDP
				
irq_dispatch 
				LDR 	PC, [ r1 ]			; jump to C user routine

				

                ;ELSE

; User Initial Stack & Heap

                ;AREA    |.text|, CODE, READONLY

                ;IMPORT  __use_two_region_memory
                ;EXPORT  __user_initial_stackheap
;__user_initial_stackheap

                ;LDR     R0, =  Heap_Mem
                ;LDR     R1, = __initial_sp
                ;LDR     R2, = (Heap_Mem +      Heap_Size)
                ;LDR     R3, = Stack_Mem
                ;BX      LR

                ;ENDIF


                align

                end

