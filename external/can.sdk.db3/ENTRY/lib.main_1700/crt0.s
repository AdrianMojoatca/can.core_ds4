;/*****************************************************************************/
;/* <<< Use Configuration Wizard in Context Menu >>>                          */
;/*****************************************************************************/

;------------------------------------------------------------------------------;
; $Id: crt0.s 33258 2015-08-19 12:46:35Z martin.bouchard $
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

Stack_Size_MSP  equ     0x0400

                area    STACK , NOINIT , READWRITE , ALIGN=3
os_heap_mem_beg

Stack_Mem_MSP   space   Stack_Size_MSP

os_heap_mem_end

;------------------------------------------------------------------------------;

                export os_heap_mem_beg
                export os_heap_mem_end

;------------------------------------------------------------------------------;

__initial_sp     ; Satisfy MicroLib
__initial_sp_msp ; Satisfy Compiler

;------------------------------------------------------------------------------;

;// <h> 'C' Heap Configuration (malloc)
;//   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
;// </h>

Heap_Size       equ     0x1000

                area    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base

Heap_Mem        space   Heap_Size

__heap_limit

;------------------------------------------------------------------------------;

NVIC_CCR        equ 0xE000ED14
SCB_VTOR        equ 0xE000ED08
SCB_SHCSR       equ 0xE000ED24

;------------------------------------------------------------------------------;

                preserve8

                thumb


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

				


; User Initial Stack & Heap

                if      :DEF:__MICROLIB
                
                export  __initial_sp
                export  __initial_sp_msp
                export  __heap_base
                export  __heap_limit
                
                else
                
                import  __use_two_region_memory
                export  __user_initial_stackheap

__user_initial_stackheap

                ldr     r0, =  Heap_Mem
                ldr     r1, = (Stack_Mem_MSP + Stack_Size_MSP)
                ldr     r2, = (Heap_Mem +  Heap_Size)
                ldr     r3, = Stack_Mem_MSP
                bx      lr

                align

                endif

;------------------------------------------------------------------------------;

                align

                end

