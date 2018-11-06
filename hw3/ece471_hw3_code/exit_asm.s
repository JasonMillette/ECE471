
@ Syscall defines
.equ SYSCALL_EXIT,     1


        .globl _start
_start:

        @================================
        @ Exit
        @================================
exit:

	@ YOUR CODE HERE
        MOV R0,#42              //loads 42 into the return register
        MOV R7, #SYSCALL_EXIT   //Loads exit into R7
        SWI 0x0                 @exits and looks at R7
