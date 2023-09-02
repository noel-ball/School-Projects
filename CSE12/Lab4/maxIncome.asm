maxIncome:
#finds the total income from the file
#arguments:	a0 contains the file record pointer array location (0x10040000 in our example) But your code MUST handle any address value
#		a1:the number of records in the file
#return value a0: heap memory pointer to actual  location of the record stock name in the file buffer

	#if empty file, return 0 for both a0, a1
	bnez a1, maxIncome_fileNotEmpty
	li a0, 0
	ret

 maxIncome_fileNotEmpty:
	
# Start your coding from here!
addi sp, sp, -16
sw ra, 0(sp)
sw s1, 4(sp)
sw s2, 8(sp)
sw s3, 12(sp)

	li s1, 0 #MaxLoop counter
	li s2, 0 #Max income
	li s3, 0 #Stock name with max income
	addi a0, a0, 4 #a0 contains pointer to first income

MaxLoop:
	beq s1, a1, MaxLoop_exit
	addi sp, sp, -4
	sw a0, 0(sp)
	jal income_from_record 
	bge a0, s2, IsGreater
	lw a0, 0(sp)
	addi sp, sp, 4

MaxLoopCont:
	addi s1, s1, 1
	addi a0, a0, 8 #moves to next income pointer
	b MaxLoop
	
IsGreater:
	addi s2, a0, 0
	lw a0, 0(sp)
	addi sp, sp, 4
	addi a0, a0, -4 #sets pointer of greatest income to its stock name
	addi s3, a0, 0
	addi a0, a0, 4 #resets pointer to income
	b MaxLoopCont
	
MaxLoop_exit:
	addi a0, s3, 0
	
lw ra, 0(sp)
lw s1, 4(sp)
lw s2, 8(sp)
lw s3, 12(sp)
addi sp, sp, 16
# End your  coding  here!
	
	ret
#######################end of maxIncome###############################################
