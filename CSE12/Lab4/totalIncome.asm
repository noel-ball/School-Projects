totalIncome:
#finds the total income from the file
#arguments:	a0 contains the file record pointer array location (0x10040000 in our example) But your code MUST handle any address value
#		a1:the number of records in the file
#return value a0:the total income (add up all the record incomes)

	#if empty file, return 0 for  a0
	bnez a1, totalIncome_fileNotEmpty
	li a0, 0
	ret

totalIncome_fileNotEmpty:
	
	# Start your coding from here!
addi sp, sp, -12
sw ra, 0(sp)
sw s1, 4(sp)
sw s2, 8(sp)


	li s1, 0 #TotalLoop counter
	li s2, 0 #Total income
	addi a0, a0, 4 #a0 contains pointer to first income

TotalLoop:
	beq s1, a1, TotalLoop_exit
	addi sp, sp, -4
	sw a0, 0(sp)
	jal income_from_record 
	add s2, s2, a0 #adds income return to total
	lw a0, 0(sp)
	addi sp, sp, 4

	addi s1, s1, 1
	addi a0, a0, 8 #moves to next income pointer
	b TotalLoop
	
TotalLoop_exit:
	addi a0, s2, 0
	

lw ra, 0(sp)
lw s1, 4(sp)
lw s2, 8(sp)
addi sp, sp, 12
# End your  coding  here!
	
	ret
#######################end of nameOfMaxIncome_totalIncome###############################################
