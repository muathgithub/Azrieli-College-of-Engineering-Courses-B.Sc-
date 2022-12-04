
.data
	.word  13, 15, 3, 30, 20, 35, 40, 12, 0# Array A # NOTE: [ 0 (Zero) Dosen't consider as element in the code ]

.text
# NOTE: [ 0 (Zero) Dosen't considered as element in the code ]
MAIN:
	lui $s0, 0x1001 # A[0]*
	add $a0, $a0, $s0 # $a0 = A[0]*
	addi $a1, $zero, 5 # $a1 = 5 divider
	
	jal LENGTH_FUN # calling the LENGTH_FUN (function) to count the length of the array And saving it in register $t5 
	
	jal count_multiple # calling the count_multiple (function) to figure/count how many element in the array divided by 5 without rest the result saved in $v1
	
	bne $t5, $v1, EXIT # if A.length($t5) == $v1(number of elements in the array A that can be divided by 5 without rest)
	
	jal ALL_TO_ZERO # else convert all the elements in A to 0(Zero)
	
	j EXIT # jump to EXIT finish the program
	
ALL_TO_ZERO: # This Recurstion method converts all the element of the array to zeros
	addi $sp, $sp, -8 #saving the values of $ra(return addres) && $a0(A[0]*) in the stack, so we can return to them when we make the back traking
	sw $a0, 0($sp) # explaind above ^
	sw $ra, 4($sp) # explaind above ^
	
	lw $t0, 0($a0) # loading the value of the elementof the addres $a0 to the register $t0
	
	bne $t0, $zero, ZERO_GO_TO_NEXT # if the array has next (the current element != 0) call  ZERO_GO_TO_NEXT to get the next elemen

	jr $ra # back to the previous call

ZERO_GO_TO_NEXT: #This function moves the recursion to the next element by adding 4 to $a0 (the previous address)
	addi $a0, $a0, 4
	jal ALL_TO_ZERO # calling ALL_TO_ZERO (recursion)

	lw $a0, 0($sp) # after reaching the vlue 0 (which is the end of the array) we start reloading $a0 and $ra, so we can get the addres and convert the value that it points on to zero
	lw $ra, 4($sp)	# explaind above ^
	
	sw $zero, 0($a0) # explaind above ^
	addi $sp, $sp, 8 # adding 8 to the stack after we have finished using the cells 
	
	jr $ra # back to the previous call
	
LENGTH_FUN: # This Recurstion function counts the number of elements in the array
	addi $sp, $sp, -8 #saving the values of $ra(return addres) && $a0(A[0]*) in the stack, so we can return to them when we make the back traking
	sw $a0, 0($sp) # explaind above ^
	sw $ra, 4($sp) # explaind above ^
	
	lw $t0, 0($a0) # loading the value of the elementof the addres $a0 to the register $t0
	
	bne $t0, $zero, GO_TO_NEXT # if the array has next (the current element != 0) call GO_TO_NEXT to get the next elemen
	
	add $t5, $zero, $zero # reset the value of $t5 to 0 (Zero) for the new result
	addi $sp, $sp, 8 #adding 8 to the stack because we didn't make a step forward so the valuse of $a0&& $ra didn't changed
	jr $ra # back to the previous call
	
GO_TO_NEXT: #This function moves the recursion to the next element by adding 4 to $a0 (the previous address)
	addi $a0, $a0, 4
	jal LENGTH_FUN # calling LENGTH_FUN (recursion)
	
	addi $t5, $t5, 1 # add one to the register $t5 ($t5++) [counting the elements]
	lw $a0, 0($sp) # after reaching the vlue 0 (which is the end of the array) we start reloading $a0 and $ra, so we can get the addres and convert the value that it points on to z
	lw $ra, 4($sp) # explaind above ^
	addi $sp, $sp, 8 # adding 8 to the stack after we have finished using the cells 
	
	jr $ra # back to the previous call

count_multiple: # This Recurstion function countshow many elements in the array divided by the number in $a1 without rest
	addi $sp, $sp, -8 #saving the values of $ra(return addres) && $a0(A[0]*) in the stack, so we can return to them when we make the back traki
	sw $a0, 0($sp) # explaind above ^
	sw $ra, 4($sp) # explaind above ^
	
	lw $s0, 0($a0) # loding which in the addres $a0 in the register $s0
	bne $s0, $zero, GET_NEXT # if the array has next (the current element != 0) call GO_TO_NEXT to get the next elemen
	
	add $v1, $zero, $zero # reset the value of $v1 to 0 (Zero) for the new result
	addi $sp, $sp, 8 #adding 8 to the stack because we didn't make a step forward so the valuse of $a0&& $ra didn't changed
	jr $ra # back to the previous call
	
GET_NEXT: #This function moves the recursion to the next element by adding 4 to $a0 (the previous address)
	addi $a0, $a0, 4
	jal count_multiple # calling count_multiple (recursion)
	
	lw $a0, 0($sp) # after reaching the vlue 0 (which is the end of the array) we start reloading $a0 and $ra, so we can get the addres and convert the value that it points on to z
	lw $a0, 0($a0) # here we laod the value which is in the address $a0 to the same rigister $a0
	
	jal remainder # calling the remainder function which returns the rest of the divistion of the number in $a0 by the number in $a1
	lw $a0, 0($sp) # here I reload the value of the address from the stack
	lw $ra, 4($sp) # explaind above ^(4 lines above)
	addi $sp, $sp, 8 #adding 8 to the stack because we didn't make a step forward so the valuse of $a0&& $ra didn't changed
	bne $v0, $zero, SKIP_AND_CONTINUE # calling the function SKIP_AND_CONTINUE to skip the operation of adding 1 to the result $v1 because the number has rest after the division
	
	addi $v1, $v1, 1 # adding 1 to the result $v1 when the nuber has no rest after the disvision
	jr $ra # back to the previous call
	
SKIP_AND_CONTINUE:
	jr $ra # back to the previous call
	
	
remainder: # This function  return the rest from dividing the number in $a0 by the number in $a1
	slt $t0, $zero, $a1 # checks if the number in $a1 (divider) == 0 (Zero) to avoid any kind of Arithmatical Errors
	beq $t0, $zero, EXIT # if true it Exit From the program
	
	add $v0, $zero, $zero # reset the value of $v0 to 0 (Zro) for the new result
	
	addi $sp, $sp, -4 # adding (-4) to the stack in order to save the value of $a0 becaue i decided to apply the changes on it
	sw $a0, 0($sp) # explaind above ^
	
	slt $t0, $a1, $a0 #chicking if the value of $a1 is smaller than or equale to the value of $a0, which means that we have to decrease the value by $a1 by through calling/using the while loop
	bne $t0, $zero, WHILE_LOOP # explaind above ^
	beq $a0, $a1, WHILE_LOOP # explaind above ^
	
	# END_WHILE_LOOP is written before WHILE_LOOP and after theremainder because it needed for finishing the work of bothe the remainder and the WHILE_LOOP
	####### POSITION OF  **END_WHILE_LOOP** is important there is no jump at the end of the reminder so it continue to the  END_WHILE_LOOP As Needed  #####
END_WHILE_LOOP: #This lable/ function initialize the value of the $v0 (result register) with the rest of the division which is in $a0
	add $v0, $v0, $a0
	lw $a0, 0($sp) # This line reload the original value of $a0 from the stack 
	addi $sp, $sp, 4 # adding 4 to the stack after I finished using the cell
	jr $ra # back to the previous call
	
WHILE_LOOP: # This while loop keeps going and decreasing the value in $a0 by $a1 till $a1 is < $a0, which means that the value of $a0 now is the rest of the division
	sub $a0, $a0, $a1 # explaind above ^
	slt $t0, $a1, $a0 # explaind above ^
	bne $t0, $zero, WHILE_LOOP #recalling the while loop lable/ function
	beq $a0, $a1, WHILE_LOOP
	j END_WHILE_LOOP # jumping the END_WHILE_LOOP after the loop have been finished to finish the work
	
EXIT:
