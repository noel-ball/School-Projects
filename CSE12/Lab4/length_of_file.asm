length_of_file:
#function to find length of data read from file
#arguments: a1=bufferAdress holding file data
#return file length in a0
	
#Start your coding here

li a0, 0
#if no student code provided, this function just returns 0 in a0
addi sp, sp, -4
sw ra, 0(sp)

li t1, 1 #sets loop counter
addi t3, a1, 0 #t3 contains buffer address

LengthLoop:	
	beq t0, zero, LengthLoop_exit
	lbu t0, 0(t3) #t0 contains char read
	addi t1, t1, 1
	addi t3, t3, 1 #moves forward in buffer address
	b LengthLoop

LengthLoop_exit:
	addi a0, t1, -2

lw ra, 0(sp)
addi sp, sp, 4

#End your coding here
	
	ret
#######################end of length_of_file###############################################	
