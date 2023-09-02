income_from_record:
#function to return numerical income from a specific record
#e.g. for record "Microsoft,34\r\n", income to return is 34(for which name is Microsoft)

#arguments:	a0 contains pointer to start of numerical income in record 

#function RETURNS income numerical value of the asci income in a0 (34 in our example)
	
# Start your coding from here!
addi sp, sp, -4
sw ra, 0(sp)

	lw t2, 0(a0) #t2 contains buffer address
	li t1, 0 #IncomeLoop counter
	li t3, 13 #carriage return ascii
	
AddressNotEmpty:
	bnez t2, IncomeLoop
	li a0, 0
	ret

IncomeLoop:	
	beq t0, t3, IncomeLoop_exit
	lb t0, 0(t2) #t0 contains char read
	addi t1, t1, 1
	addi t2, t2, 1
	b IncomeLoop

IncomeLoop_exit:
	li t0, 10
	li t3, 0 #DigitLoop counter
	lw t2, 0(a0) #t2 contains reset buffer address
	li a0, 0
	addi t1, t1, -1 #t1 is number of digits
	addi t6, t1, -1 #t6 is number of exponents needed for 10^n
	
DigitLoop:
	beq t3, t1, DigitLoop_exit
	li t5, 0 #ExponentLoop counter
	lb t4,(t2)
	addi t4, t4, -48 #t4 contains digit
	b TensExponents
	
TensExponents:
	beq t5, t6, DigitLoopCont
	mul t4, t4, t0 #digit * 10^n
	addi t5, t5, 1
	b TensExponents
	
DigitLoopCont:
	addi t3, t3, 1
	addi t2, t2, 1
	addi t6, t6, -1
	add a0, a0, t4
	b DigitLoop
	
DigitLoop_exit:

lw ra, 0(sp)
addi sp, sp, 4
# End your  coding  here!
	ret
	
#######################end of income_from_record###############################################	
