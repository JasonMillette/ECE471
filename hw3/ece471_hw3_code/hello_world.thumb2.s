//.syntax unified

@ Syscall Definitions
.equ SYSCALL_EXIT,     1
.equ SYSCALL_WRITE,    4

@ Other Definitions
.equ STDOUT,	       1

        .globl _start
_start:

	mov	r0,#0
        MOV     R6,#0   //used for loop counter

	@==========================================
	@ Your code for part 3d goes here =\
	@                                  \/
	@==========================================
print:

	@==========================================

	blx	print_number		@ print r0 as a decimal number

	ldr	r1,=message		@ load message
	blx	print_string		@ print it

	@==========================================
	@ Your Code for part 3d Also goes Here  =\
	@                                        \/
	@==========================================
        CMP R6, #15     //Checks for last loop
        BEQ exit        //Exits on last loop
        ADD R6, R6, #1  //Increments loop number
        MOV R0, R6      //sets R0 to loop number
        BLX print         //loops for next line
	@==========================================



        @================================
        @ Exit
        @================================
exit:
	mov	r0,#0			@ Return a zero
        mov	r7,#SYSCALL_EXIT	@
        swi	0x0			@ Run the system call


	@====================
	@ print_string
	@====================
	@ Null-terminated string to print pointed to by r1
	@ the value in r1 is destroyed by this routine


.thumb

print_string:

	PUSH    {r0,r2,r7,lr}	@ Save r0,r2,r7,lr on stack

	mov	r2,#3			@ The wrong value of r2
					@ Just so the code runs
					@ Your code below will overwrite
					@ with the proper version

	@==========================================
	@ Your code for part 3b goes here =\
	@                                  \/
	@==========================================
        MOV     R2, #0                  //sets R2 to zero for counting
loop:
        LDRB    R5, [R1]                //Loads the character
        CMP     R5, #0                  //checks for NULL termination
        BEQ     end                     //If NULL end
        ADD     R2, R2, #1              //increment size
        ADD     R1, R1, #1              //increment pointer
        BLX     loop                    //repeat loop

end:
        SUB R1, R2

	@==========================================
	@ The length of the string pointed to by r1
	@ Should be put in r2 by your code above
	@==========================================

	mov	r0,#STDOUT		@ R0 Print to stdout
					@ R1 points to our string
					@ R2 is the length
	mov	r7,#SYSCALL_WRITE	@ Load syscall number
	swi	0x0			@ System call

	pop	{r0,r2,r3,r7,pc}       @ pop r0,r2,r3,pc from stack

.arm

        @#############################
	@ print_number
	@#############################
	@ r0 = value to print
	@ r0 is destroyed by this routine

print_number:
	push	{R3,LR}	@ Save R3 and return address on stack
        ldr	R3,=buffer	@ Point to beginning of buffer
        add	R3,R3,#10	@ #1

divide:
	blX	divide_by_10	@ divide by 10
	add	r8,r8,#0x30	@ #2
	strb	r8,[R3],#-1	@ store to buffer, increment pointer
	adds	r0,r7,#0	@ move quotient to r0, update status flag
	bne	divide		@ if quotient not zero then loop

write_out:
        add	r1,R3,#1	@ adjust pointer to print to beginning

        blx	print_string	@ print the string

        pop	{R3,LR}	@ restore saved values from stack

        mov	pc,lr		@ return from function

	##############################
	# divide_by_10
	##############################
	# r0=numerator
        # r4,r5 trashed
	# r7 = quotient, r8 = remainder

divide_by_10:
	ldr	r4,=429496730		@ 1/10 * 2^32
	sub	r5,r0,r0,lsr #30
	umull	r8,r7,r4,r5		@ {r8,r7}=r4*r5

	mov	r4,#10			@ calculate remainder
	mul	r8,r7,r4
	sub	r8,r0,r8

	mov	pc,lr

.data
message:	.string ": ECE471 is cool\n"

@ BSS
.lcomm buffer,11
